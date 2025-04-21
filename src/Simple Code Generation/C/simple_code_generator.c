/* The input format are indirect representations
(like quadruples, triples for example) */
#include <stdio.h>
#include <string.h>

#define MAX_QUADS 100

//struct to store the input quads
struct Quadruple {
    char op;
    char arg1[20];
    char arg2[20];
    char result[20];
};


void generate_simple_code(struct Quadruple quads[], int n) {
    printf("Generated Assembly:\n");
    for (int i = 0; i < n; i++) {
        switch (quads[i].op) {
            case '+':
                printf("LOAD %s, R%d\n", quads[i].arg1, i+1);
                printf("ADD %s, R%d\n", quads[i].arg2, i+1);
                printf("STORE R%d, %s\n", i+1, quads[i].result);
                break;
            case '-':
                printf("LOAD %s, R%d\n", quads[i].arg1, i+1);
                printf("SUB %s, R%d\n", quads[i].arg2, i+1);
                printf("STORE R%d, %s\n", i+1, quads[i].result);
                break;
            case '*':
                printf("LOAD %s, R%d\n", quads[i].arg1, i+1);
                printf("MUL %s, R%d\n", quads[i].arg2, i+1);
                printf("STORE R%d, %s\n", i+1, quads[i].result);
                break;
            case '/':
                printf("LOAD %s, R%d\n", quads[i].arg1, i+1);
                printf("DIV %s, R%d\n", quads[i].arg2, i+1);
                printf("STORE R%d, %s\n", i+1, quads[i].result);
                break;
        }
        printf("\n");
    }
}

int main() {
    struct Quadruple quads[MAX_QUADS];
    int n;

    printf("Enter number of quadruples: ");
    scanf("%d", &n);
    getchar();

    //read quadruples
    for (int i = 0; i < n; i++) {
        printf("Enter quadruple %d (op, arg1, arg2, result): ", i+1);
        scanf(" %c %s %s %s", &quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }

    //generate assembly code
    generate_simple_code(quads, n);

    return 0;
}

/*
sample io

Enter number of quadruples: 3
Enter quadruple 1 (op, arg1, arg2, result): + a b t1
Enter quadruple 2 (op, arg1, arg2, result): * t1 c t2
Enter quadruple 3 (op, arg1, arg2, result): / t2 3 d

*/