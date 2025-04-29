#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define RULES 10

//struct to store grammar rule
struct prodn {
    char lhs;
    char rhs[20];
};

struct prodn p[RULES];
int count = 0;

char leading[RULES][MAX];
char trailing[RULES][MAX];

//remove spaces from CFG input
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

//check if the symbol is a non terminal
int is_terminal(char c) {
    return !isupper(c);
}

//add only unique symbols to the output set
void add_to_set(char *set, char symbol) {
    if (strchr(set, symbol) == NULL) {
        int len = strlen(set);
        set[len] = symbol;
        set[len + 1] = '\0';
    }
}

//compute leading set for nt
void compute_leading(char nt, char *result) {
    for (int i = 0; i < count; i++) {
        if (p[i].lhs == nt) {
            for (int j = 0; j < strlen(p[i].rhs); j++) {
                char sym = p[i].rhs[j];
                if (sym == nt) continue; //skip left-recursive to prevent infinite loop
                if (is_terminal(sym)) {
                    add_to_set(result, sym);
                    break;
                } else {
                    compute_leading(sym, result);
                    break;
                }
            }
        }
    }
}

//compute trailing set for nt
void compute_trailing(char nt, char *result) {
    for (int i = 0; i < count; i++) {
        if (p[i].lhs == nt) {
            for (int j = strlen(p[i].rhs) - 1; j >= 0; j--) {
                char sym = p[i].rhs[j];
                if (sym == nt) continue; //skip right-recursive to prevent infinite loop
                if (is_terminal(sym)) {
                    add_to_set(result, sym);
                    break;
                } else {
                    compute_trailing(sym, result);
                    break;
                }
            }
        }
    }
}

//format the final set with a space between characters
void format_set(char *original, char *formatted) {
    int len = strlen(original);
    for (int i = 0; i < len; i++) {
        formatted[2 * i] = original[i];
        if (i < len - 1)
            formatted[2 * i + 1] = ' ';
    }
    formatted[2 * len - 1] = '\0';
}

//check if a non-terminal is already processed before
int seen(char ch, char *done, int len) {
    for (int i = 0; i < len; i++) {
        if (done[i] == ch) return 1;
    }
    return 0;
}

int main() {
    printf("Enter number of grammar rules: ");
    scanf("%d", &count);
    getchar(); //this is to consume newline
               //(basically the '\n, in the input buff. after u press enter to input the next cfg rule)

    printf("Enter rules in form 'E->E+T':\n");
    for (int i = 0; i < count; i++) {
        char buffer[MAX];
        fgets(buffer, MAX, stdin);
        remove_spaces(buffer);
        p[i].lhs = buffer[0];
        strcpy(p[i].rhs, buffer + 3);
        leading[i][0] = trailing[i][0] = '\0';
    }

    char done[RULES];
    int done_count = 0;

    //output formatting block. feel free to use your own table style to format
    printf("\n%-10s | %-15s | %-15s\n", "Non-Term", "LEADING", "TRAILING");
    printf("---------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (!seen(p[i].lhs, done, done_count)) {
            compute_leading(p[i].lhs, leading[i]);
            compute_trailing(p[i].lhs, trailing[i]);
            done[done_count++] = p[i].lhs;

            //this part makes the o/p look nicer. calls format_set to add a space btw the terminals if it is not the first in that set. 
            char formatted_leading[MAX] = "", formatted_trailing[MAX] = "";
            format_set(leading[i], formatted_leading);
            format_set(trailing[i], formatted_trailing);

            printf("%-10c | %-15s | %-15s\n", p[i].lhs, formatted_leading, formatted_trailing);
        }
    }

    return 0;
}
