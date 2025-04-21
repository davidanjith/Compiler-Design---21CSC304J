#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char item) {
    if(top >= MAX-1) {
        printf("Stack Overflow\n");
        return;
    }
    stack[++top] = item;
    printf("Pushed: %c\n", item);
}

char pop() {
    if(top < 0) {
        printf("Stack Underflow\n");
        return -1;
    }
    char item = stack[top--];
    printf("Popped: %c\n", item);
    return item;
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

void infixToPostfix(char* infix) {
    printf("\nInfix Expression: %s\n", infix);
    printf("Conversion Steps:\n");
    
    char postfix[MAX*2]; //adding a larger buffer since we are adding spaces for the output to look properly formatted.
    int j = 0;
    int prevWasDigit = 0;
    
    for(int i = 0; infix[i] != '\0'; i++) {
        printf("\nProcessing: %c\n", infix[i]);
        
        if(isdigit(infix[i])) {
            if(prevWasDigit) {
                postfix[j++] = infix[i]; //treat multi_digit numbers as a single number
            } else {
                if(j > 0) postfix[j++] = ' '; //add space before new number
                postfix[j++] = infix[i];
            }
            prevWasDigit = 1;
            printf("Added to output: %c\n", infix[i]);
        }
        else if(isalpha(infix[i])) {
            if(j > 0) postfix[j++] = ' ';
            postfix[j++] = infix[i];
            prevWasDigit = 0;
            printf("Added to output: %c\n", infix[i]);
        }
        else if(infix[i] == '(') {
            push(infix[i]);
            prevWasDigit = 0;
        }
        else if(infix[i] == ')') {
            while(top != -1 && stack[top] != '(') {
                postfix[j++] = ' ';
                postfix[j++] = pop();
            }
            if(top != -1 && stack[top] == '(') {
                pop(); //remove '(' from stack
            }
            prevWasDigit = 0;
        }
        else {
            while(top != -1 && precedence(stack[top]) >= precedence(infix[i])) {
                postfix[j++] = ' ';
                postfix[j++] = pop();
            }
            push(infix[i]);
            prevWasDigit = 0;
        }
        
        printf("Current Stack: ");
        for(int k = 0; k <= top; k++) printf("%c ", stack[k]);
        
        printf("\nCurrent Output: %.*s\n", j, postfix);
    }
    
    while(top != -1) {
        postfix[j++] = ' ';
        postfix[j++] = pop();
    }
    postfix[j] = '\0';
    
    printf("\nFinal Postfix Expression: %s\n", postfix);
}

int main() {
    char infix[MAX];
    printf("Enter infix expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = '\0'; 
    
    infixToPostfix(infix);
    return 0;
}