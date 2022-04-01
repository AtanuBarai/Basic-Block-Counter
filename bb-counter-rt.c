#include <stdio.h>
#include <stdlib.h>

unsigned *bb_counts = NULL;

// unsigned *bb_counts;
unsigned max_bb_id = 0;

void initCounter(unsigned n_bb) {
    max_bb_id = n_bb;
    bb_counts = (unsigned*)calloc(n_bb, sizeof(unsigned));
    // printf("initCounter Called -------- n_bb = %u\n", n_bb);
}

void bbCounter(unsigned id) {
    if(id >= max_bb_id) {
        printf("Reached Max BB_LIMIT\nAborting!\n");
        exit(1);
    }
    bb_counts[id]++;
    // printf("bbCounter called with id %u, count = %u, max_bb = %u\n", id, bb_counts[id], max_bb_id);
}

void dumpCounts(unsigned a) {
    for(unsigned id = 0; id < max_bb_id; id++) {
        printf("%u, ", bb_counts[id]);
    }
    printf("\n");
    free(bb_counts);
}