#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS 4
#define MAX_DEFS 5  // total number of unique definitions

// Bit vector utility functions
void union_set(int* result, int* a, int* b) {
    for (int i = 0; i < MAX_DEFS; i++)
        result[i] = a[i] || b[i];
}

void subtract_set(int* result, int* a, int* b) {
    for (int i = 0; i < MAX_DEFS; i++)
        result[i] = a[i] && !b[i];
}

int equal_set(int* a, int* b) {
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

// Hardcoded data

// gen[B][def] == 1 if definition def is generated in block B
int gen[MAX_BLOCKS][MAX_DEFS] = {
    {1, 0, 0, 0, 0},  // Block 1: d1
    {0, 1, 0, 0, 0},  // Block 2: d2
    {0, 0, 1, 0, 0},  // Block 3: d3
    {0, 0, 0, 1, 0},  // Block 4: d4
};

// kill[B][def] == 1 if definition def is killed in block B
int kill[MAX_BLOCKS][MAX_DEFS] = {
    {0, 0, 1, 0, 0},  // Block 1 kills d3
    {0, 0, 0, 1, 0},  // Block 2 kills d4
    {1, 0, 0, 0, 0},  // Block 3 kills d1
    {0, 1, 0, 0, 0},  // Block 4 kills d2
};

// Predecessors for each block
int preds[MAX_BLOCKS][MAX_BLOCKS] = {
    {},           // Block 1 has no preds
    {1},          // Block 2 <- Block 1
    {2, 4},       // Block 3 <- Block 2, 4
    {2}           // Block 4 <- Block 2
};

int num_preds[MAX_BLOCKS] = {0, 1, 2, 1};

// IN[B] and OUT[B]
int in[MAX_BLOCKS][MAX_DEFS];
int out[MAX_BLOCKS][MAX_DEFS];

int main() {
    // Initialize OUT[B] = empty for all B
    for (int b = 0; b < MAX_BLOCKS; b++)
        for (int d = 0; d < MAX_DEFS; d++)
            out[b][d] = 0;

    int changed;
    do {
        changed = 0;
        for (int b = 0; b < MAX_BLOCKS; b++) {
            int new_in[MAX_DEFS] = {0};
            // IN[B] = union of OUT[P] for all preds P of B
            for (int p = 0; p < num_preds[b]; p++) {
                int pred = preds[b][p] - 1; // index from 0
                union_set(new_in, new_in, out[pred]);
            }

            // OUT[B] = gen[B] ∪ (IN[B] - kill[B])
            int temp[MAX_DEFS], new_out[MAX_DEFS];
            subtract_set(temp, new_in, kill[b]);
            union_set(new_out, gen[b], temp);

            // Check if IN or OUT changed
            if (!equal_set(in[b], new_in) || !equal_set(out[b], new_out)) {
                copy_set(in[b], new_in);
                copy_set(out[b], new_out);
                changed = 1;
            }
        }
    } while (changed);

    // Final output
    printf("Reaching Definitions:\n");
    for (int b = 0; b < MAX_BLOCKS; b++) {
        printf("Block %d:\n", b + 1);
        printf("  IN  = "); print_set(in[b]); printf("\n");
        printf("  OUT = "); print_set(out[b]); printf("\n");
    }

    return 0;
}
