#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define RULES 10

//struct to store grammar rule
typedef struct {
    char lhs;
    char rhs[MAX];
} Rule;

Rule grammar[RULES];
int rule_count = 0;
char input[MAX];
char stack[MAX] = "$";
int top = 1;

//formatting block for creaating the table. feel free to tweak this to get the output format that is required
void print_status(const char *action, const char *inp_ptr) {
    printf("%-15s|%-15s|%s\n", stack, inp_ptr, action);
}

//remove spaces from input or RHS expressions
void remove_spaces(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace(*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

//normally you would need <std.bool.h> to use the bool datatype in C.
//this is a small workaround to create a bool datatype since C without importing <stdbool.h>.
typedef int bool;
#define true 1
#define false 0

bool try_reduce(const char *inp_ptr) {
    for (int i = 0; i < rule_count; i++) {
        int len = strlen(grammar[i].rhs);
        if (len <= top - 1 && strncmp(&stack[top - len], grammar[i].rhs, len) == 0) {
            stack[top - len] = grammar[i].lhs;
            stack[top - len + 1] = '\0';
            top = top - len + 1;
            char action[50];
            sprintf(action, "Reduce %c->%s", grammar[i].lhs, grammar[i].rhs);
            print_status(action, inp_ptr);
            return true;
        }
    }
    return false;
}

int main() {
    printf("Enter number of grammar rules: ");
    scanf("%d", &rule_count);
    getchar(); // consume newline

    printf("Enter rules in form E->E+T (no spaces):\n");
    for (int i = 0; i < rule_count; i++) {
        char buffer[MAX];
        fgets(buffer, MAX, stdin);
        remove_spaces(buffer);
        grammar[i].lhs = buffer[0];
        strcpy(grammar[i].rhs, buffer + 3); //skip the 'X->'part from the input
    }

    printf("Enter a string to parse (example: i+i*i): ");
    scanf("%s", input);

    char *inp_ptr = input;

    printf("%-15s|%-15s|%s\n", "Stack", "Input Buffer", "Parsing Action");
    printf("--------------------------------------------------\n");

    while (1) {
        while (try_reduce(inp_ptr)) {
            //greedy reduction
        }

        if (*inp_ptr == '\0') {
            if (strcmp(stack, "$E") == 0 || stack[top-1] == grammar[0].lhs) {
                print_status("Accepted", inp_ptr);
            } else {
                print_status("Rejected", inp_ptr);
            }
            break;
        }

        //shift operation
        stack[top++] = *inp_ptr;
        stack[top] = '\0';
        inp_ptr++;
        print_status("Shift", inp_ptr);
    }

    return 0;
}
