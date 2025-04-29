#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS 4 //total no of blocks
#define MAX_DEFS 5  //total number of unique definitions

//for generating the IN sets
void union_set(int* result, int* a, int* b) {
    for (int i = 0; i < MAX_DEFS; i++)
        result[i] = a[i] || b[i];
}

//used in finding the OUT sets to see which defs leave that block witghout being killed
void subtract_set(int* result, int* a, int* b) {
    for (int i = 0; i < MAX_DEFS; i++)
        result[i] = a[i] && !b[i];
}

//just checks if any sets changed(if they were killed or not)
int macha_ithu_same_set_ah(int* a, int* b) {
    for (int i = 0; i < MAX_DEFS; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

void copy_set(int* dest, int* src) {
    for (int i = 0; i < MAX_DEFS; i++)
        dest[i] = src[i];
}

void print_set(int* set) {
    printf("{ ");
    for (int i = 0; i < MAX_DEFS; i++)
        if (set[i]) printf("d%d ", i + 1);
    printf("}");
}

//Hardcoded data

//gen set
//here gen[B][def] == 1 if definition def is generated in block B
int gen[MAX_BLOCKS][MAX_DEFS] = {
    {1, 0, 0, 0, 0},  // a new def d1 in Block 1
    {0, 1, 0, 0, 0},  // a new def d2 in block 2 and so on
    {0, 0, 1, 0, 0},  
    {0, 0, 0, 1, 0},  
};

//kill set
//kill[B][def] == 1 if definition def is killed in block B
int kill[MAX_BLOCKS][MAX_DEFS] = {
    {0, 0, 1, 0, 0},  // Block 1 kills d3
    {0, 0, 0, 1, 0},  // Block 2 kills d4 etc. etc.
    {1, 0, 0, 0, 0},  
    {0, 1, 0, 0, 0},  
};

//predecessors for each block
int preds[MAX_BLOCKS][MAX_BLOCKS] = {
    {}, //REMINDER TO FIX THIS LATER          //Block 1 has no preds
    {1},          //Block 1 is the pred of Block 2 and so on
    {2, 4},       
    {2}          
};

int num_preds[MAX_BLOCKS] = {0, 1, 2, 1}; //used to specify the no of loop iterations

// IN[B] and OUT[B]
int in[MAX_BLOCKS][MAX_DEFS];
int out[MAX_BLOCKS][MAX_DEFS];

int main() {
    //initialize OUT[B] = empty for all B at the beginning
    for (int b = 0; b < MAX_BLOCKS; b++)
        for (int d = 0; d < MAX_DEFS; d++)
            out[b][d] = 0;

    int changed;
    do {
        changed = 0;
        for (int b = 0; b < MAX_BLOCKS; b++) {
            int new_in[MAX_DEFS] = {0};
            //IN[B] = union of OUT[P] for all preds P of B
            for (int p = 0; p < num_preds[b]; p++) {
                int pred = preds[b][p] - 1; //index from 0
                union_set(new_in, new_in, out[pred]);
            }

            //OUT[B] = gen[B] ∪ (IN[B] - kill[B])
            int temp[MAX_DEFS], new_out[MAX_DEFS];
            subtract_set(temp, new_in, kill[b]);
            union_set(new_out, gen[b], temp);

            //check if IN or OUT changed
            if (!macha_ithu_same_set_ah(in[b], new_in) || !macha_ithu_same_set_ah(out[b], new_out)) {
                copy_set(in[b], new_in);
                copy_set(out[b], new_out);
                changed = 1;
            }
        }
    } while (changed);

    //Final output
    printf("Reaching Definitions:\n");
    for (int b = 0; b < MAX_BLOCKS; b++) {
        printf("Block %d:\n", b + 1);
        printf("  IN  = "); print_set(in[b]); printf("\n");
        printf("  OUT = "); print_set(out[b]); printf("\n");
    }

    return 0;
}
