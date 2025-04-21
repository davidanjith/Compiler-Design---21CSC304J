/*
this is a hardcoded version that takes input as quadruples
the main goal here is building a DAG that merges duplicate subexpressions
and remove redundant code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100

//struct for quads
struct Quadruple {
    char op;
    char arg1[20];
    char arg2[20];
    char result[20];
};


//DAG node struct
typedef struct Node {
    char id[20];          // e.g., "t1", "a"
    char op;              // '+', '-', '*', '/', or 'v' (variable/constant)
    struct Node *left, *right;
} Node;

Node* dag[MAX_NODES];
int node_count = 0;

Node* create_node(char op, char* id, Node* left, Node* right) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->id, id);
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    dag[node_count++] = new_node;
    return new_node;
}

Node* find_existing_node(char op, Node* left, Node* right) {
    for (int i = 0; i < node_count; i++) {
        if (dag[i]->op == op && dag[i]->left == left && dag[i]->right == right) {
            return dag[i];  // Reuse existing node
        }
    }
    return NULL;
}

void build_dag(struct Quadruple quads[], int n) {
    for (int i = 0; i < n; i++) {
        Node* left = find_operand_node(quads[i].arg1);
        Node* right = find_operand_node(quads[i].arg2);
        Node* existing = find_existing_node(quads[i].op, left, right);
        
        if (existing) {
            printf("Reused node %s for %s\n", existing->id, quads[i].result);
        } else {
            Node* new_node = create_node(quads[i].op, quads[i].result, left, right);
            printf("Created node %s = %s %c %s\n", 
                   new_node->id, left->id, new_node->op, right->id);
        }
    }
}

Node* find_operand_node(char* id) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(dag[i]->id, id) == 0) return dag[i];
    }
    return create_node('v', id, NULL, NULL);  // Variable/constant
}

int main() {
    struct Quadruple quads[] = {
        {'+', "a", "b", "t1"},  //first a + b
    {'+', "a", "b", "t2"},  // Redundant a + b 
    {'*', "t1", "t2", "t3"}, // t3 = (a+b) * (a+b)
    {'*', "t2", "t3", "t4"}
    };
    int n = sizeof(quads) / sizeof(quads[0]);
    build_dag(quads, n);
    return 0;
}