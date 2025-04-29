#include <stdio.h>
#define B 4 //blocks
#define D 5 //definitions

void set(int *r, int *a, int *b, int op) {
    for (int i = 0; i < D; i++) 
        r[i] = op ? a[i] && !b[i] : a[i] || b[i];
}

int eq(int *a, int *b) {
    for (int i = 0; i < D; i++) if (a[i] != b[i]) return 0;
    return 1;
}

int main() {
    int gen[B][D] = {{1,0,0,0,0},{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0}};
    int kill[B][D] = {{0,0,1},{0,0,0,1},{1},{0,1}};
    int pred[B][B] = {{0},{1},{2,4},{2}}, np[B] = {0,1,2,1};
    int in[B][D] = {0}, out[B][D] = {0}, c, t[D], o[D], b;

    do {
        c = 0;
        for (b = 0; b < B; b++) {
            int i[D] = {0};
            for (int p = 0; p < np[b]; p++) set(i, i, out[pred[b][p]-1], 0);
            set(t, i, kill[b], 1); set(o, gen[b], t, 0);
            if (!eq(in[b], i) || !eq(out[b], o)) {
                for (int j = 0; j < D; j++) in[b][j] = i[j], out[b][j] = o[j];
                c = 1;
            }
        }
    } while (c);

    printf("Reaching Definitions:\n");
    for (b = 0; b < B; b++) {
        printf("Block %d:\n  IN = { ", b+1);
        for (int i = 0; i < D; i++) if (in[b][i]) printf("d%d ", i+1);
        printf("}\n  OUT = { ");
        for (int i = 0; i < D; i++) if (out[b][i]) printf("d%d ", i+1);
        printf("}\n");
    }
    return 0;
}