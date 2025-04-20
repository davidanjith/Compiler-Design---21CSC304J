#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_BLOCKS 10
#define MAX_DEFS 50
#define MAX_VAR_LEN 10

//a struct that stores variable name, the brock in which its present and which definition it is.
typedef struct {
    char var;
    int block;
    int def_id;
} Definition;

Definition defs[MAX_DEFS];
int def_count = 0;

int gen[MAX_BLOCKS][MAX_DEFS];
int kill[MAX_BLOCKS][MAX_DEFS];

int same_var_defs[MAX_DEFS];  //tracks the defs of same var

void add_definition(char var, int block) {
    defs[def_count].var = var;
    defs[def_count].block = block;
    defs[def_count].def_id = def_count;
    def_count++;
}

void compute_gen_kill(int num_blocks) {
    for (int b = 0; b < num_blocks; b++) {
        for (int i = 0; i < def_count; i++) {
            if (defs[i].block == b) {
                gen[b][defs[i].def_id] = 1;

                //KILL = All other defs to same var
                for (int j = 0; j < def_count; j++) {
                    if (defs[j].block != b && defs[j].var == defs[i].var) {
                        kill[b][defs[j].def_id] = 1;
                    }
                }
            }
        }
    }
}

void print_sets(int num_blocks) {
    printf("\nAll Definitions\n");
    for (int i = 0; i < def_count; i++) {
        printf("d%d: %c (Block %d)\n", defs[i].def_id, defs[i].var, defs[i].block);
    }

    for (int b = 0; b < num_blocks; b++) {
        printf("\nBlock %d:\n", b);
        printf("GEN[%d] = { ", b);
        for (int i = 0; i < def_count; i++)
            if (gen[b][i]) printf("d%d ", i);
        printf("}\n");

        printf("KILL[%d] = { ", b);
        for (int i = 0; i < def_count; i++)
            if (kill[b][i]) printf("d%d ", i);
        printf("}\n");
    }
}

int main() {
    int num_blocks;
    printf("Enter number of basic blocks: ");
    scanf("%d", &num_blocks);
    getchar();  //eat da newline boii

    for (int b = 0; b < num_blocks; b++) {
        int defs_in_block;
        printf("\nEnter number of definitions in Block %d: ", b);
        scanf("%d", &defs_in_block);
        getchar();  

        for (int i = 0; i < defs_in_block; i++) {
            char line[100];
            printf("Enter definition %d (e.g., a = 5;): ", i + 1);
            fgets(line, sizeof(line), stdin);

            //extract the variable name (assuming that the first char before '=' is the variable)
            for (int j = 0; line[j]; j++) {
                if (isalpha(line[j])) {
                    add_definition(line[j], b);
                    break;
                }
            }
        }
    }

    compute_gen_kill(num_blocks);
    print_sets(num_blocks);

    return 0;
}
