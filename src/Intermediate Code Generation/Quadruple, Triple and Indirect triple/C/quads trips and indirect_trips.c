/*The input format here is a collection Three aaddress codes*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INSTRUCTIONS 100

//struct for quadruples
struct Quadruple {
    char op;
    char arg1[20];
    char arg2[20];
    char result[20];
};

//struct for triples
struct Triple {
    char op;
    char arg1[20];
    char arg2[20];
};

// we dont need a new struct for indirect trips because they can reuse the struct for normal trips

int main() {
    struct Quadruple quads[MAX_INSTRUCTIONS];
    struct Triple triples[MAX_INSTRUCTIONS];
    int execution_order[MAX_INSTRUCTIONS]; //for indirect trips
    int n = 0; //no.of instructions

    //input: no.of three-address-codes
    printf("Enter number of Three-Address Code instructions: ");
    scanf("%d", &n);
    getchar();

    //input: read each TAC instruction
    for (int i = 0; i < n; i++) {
        char line[100];
        printf("Enter instruction %d (e.g., 't1 = a + b'): ", i+1);
        fgets(line, sizeof(line), stdin);
        
        //parse line into the form " result = arg1 op arg2 "
        char result[20], arg1[20], arg2[20], op;
        sscanf(line, "%s = %s %c %s", result, arg1, &op, arg2);

        //store Quadruple
        quads[i].op = op;
        strcpy(quads[i].arg1, arg1);
        strcpy(quads[i].arg2, arg2);
        strcpy(quads[i].result, result);

        //store Triple (with placeholder for references for indirect trips)
        triples[i].op = op;
        strcpy(triples[i].arg1, arg1);
        strcpy(triples[i].arg2, arg2);

        //track execution order (for Indirect Triples)
        execution_order[i] = i;
    }

    

    //quads
    printf("\nQuadruples:\n");
    for (int i = 0; i < n; i++) {
        printf("(%c, %s, %s, %s)\n", 
               quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }

    //trips (convert temp variables to pointers to earlier trip that computed their value)
    printf("\nTriples:\n");
    for (int i = 0; i < n; i++) {
        //format: " result = arg1 op arg2 "
        //check if arg1 is a temp (we assume that it starts with 't' according to textbook convention)  
        char arg1[20], arg2[20];
        strcpy(arg1, triples[i].arg1);
        strcpy(arg2, triples[i].arg2);

        //Replace temps with pointers (for example, "t1" -> "(0)")
        for (int j = 0; j < i; j++) {
            if (strcmp(quads[j].result, arg1) == 0) {
                sprintf(arg1, "(%d)", j);
            }
            if (strcmp(quads[j].result, arg2) == 0) {
                sprintf(arg2, "(%d)", j);
            }
        }

        printf("(%c, %s, %s)\n", triples[i].op, arg1, arg2);
    }

    //indirect trips
    printf("\nIndirect Triples:\n");
    printf("Triples Table:\n");
    for (int i = 0; i < n; i++) {
        printf("[%d]: (%c, %s, %s)\n", 
               i, triples[i].op, triples[i].arg1, triples[i].arg2);
    }
    printf("\nExecution Order:\n");
    for (int i = 0; i < n; i++) {
        printf("%d -> ", execution_order[i]);
    }
    printf("END\n");

    return 0;
}