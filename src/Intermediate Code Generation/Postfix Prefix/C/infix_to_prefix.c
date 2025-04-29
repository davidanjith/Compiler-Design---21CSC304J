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

void reverseString(char* str) {
    int n = strlen(str);
    for(int i = 0; i < n/2; i++) {
        char temp = str[i];
        str[i] = str[n-i-1];
        str[n-i-1] = temp;
    }
}

void infixToPrefix(char* infix) {
    printf("\nInfix Expression: %s\n", infix);
    printf("Conversion Steps:\n");
    
    //reverse the infix expression
    reverseString(infix);
    printf("Reversed Infix: %s\n", infix);
    
    char prefix[MAX*2]; //since we are using a space after every token( for a nicer output formatting), we are using a buffer twice the size of MAX
    int j = 0;
    int prevWasDigit = 0;
    
    for(int i = 0; infix[i] != '\0'; i++) {
        printf("\nProcessing: %c\n", infix[i]);
        
        if(isdigit(infix[i])) {
            if(prevWasDigit) {
                prefix[j++] = infix[i]; //multi-digit number support
            } else {
                if(j > 0) prefix[j++] = ' '; //add space before new number for proper formatting
                prefix[j++] = infix[i];
            }
            prevWasDigit = 1;
            printf("Added to output: %c\n", infix[i]);
        }
        else if(isalpha(infix[i])) {
            if(j > 0) prefix[j++] = ' ';
            prefix[j++] = infix[i];
            prevWasDigit = 0;
            printf("Added to output: %c\n", infix[i]);
        }
        else if(infix[i] == ')') {
            push(infix[i]);
            prevWasDigit = 0;
        }
        else if(infix[i] == '(') {
            while(top != -1 && stack[top] != ')') {
                prefix[j++] = ' ';
                prefix[j++] = pop();
            }
            if(top != -1 && stack[top] == ')') {
                pop(); //remove ')' from stack
            }
            prevWasDigit = 0;
        }
        else {
            while(top != -1 && precedence(stack[top]) > precedence(infix[i])) {
                prefix[j++] = ' ';
                prefix[j++] = pop();
            }
            push(infix[i]);
            prevWasDigit = 0;
        }
        
        printf("Current Stack: ");
        for(int k = 0; k <= top; k++) printf("%c ", stack[k]);
        
        printf("\nCurrent Output: ");
        for(int k = j-1; k >= 0; k--) printf("%c", prefix[k]);
        printf("\n");
    }
    
    while(top != -1) {
        prefix[j++] = ' ';
        prefix[j++] = pop();
    }
    prefix[j] = '\0';
    
    //reverse the prefix to get final result
    reverseString(prefix);
    
    printf("\nFinal Prefix Expression: %s\n", prefix);
}

int main() {
    char infix[MAX];
    printf("Enter infix expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = '\0';
    infixToPrefix(infix);
    return 0;
}