#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char item) {
    if (top >= MAX - 1) return;
    stack[++top] = item;
}

char pop() {
    if (top < 0) return '\0';
    return stack[top--];
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

void infixToPostfix(char* infix) {
    char postfix[MAX * 2] = {0}; // extra space for separators
    int j = 0;
    int prevWasDigit = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if (isspace(ch)) continue;

        if (isdigit(ch)) {
            if (!prevWasDigit && j > 0) postfix[j++] = ' ';
            postfix[j++] = ch;
            prevWasDigit = 1;
        } else if (isalpha(ch)) {
            if (j > 0) postfix[j++] = ' ';
            postfix[j++] = ch;
            prevWasDigit = 0;
        } else if (ch == '(') {
            push(ch);
            prevWasDigit = 0;
        } else if (ch == ')') {
            while (top != -1 && stack[top] != '(') {
                postfix[j++] = ' ';
                postfix[j++] = pop();
            }
            if (top != -1 && stack[top] == '(') {
                pop();
            }
            prevWasDigit = 0;
        } else {
            while (top != -1 && precedence(stack[top]) >= precedence(ch)) {
                postfix[j++] = ' ';
                postfix[j++] = pop();
            }
            push(ch);
            prevWasDigit = 0;
        }
    }

    while (top != -1) {
        postfix[j++] = ' ';
        postfix[j++] = pop();
    }
    postfix[j] = '\0';

    printf("Final Expression : %s\n", postfix);
}

int main() {
    char infix[MAX];
    printf("Enter infix expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = '\0'; // remove newline

    top = -1; // reset stack before conversion
    infixToPostfix(infix);
    return 0;
}
