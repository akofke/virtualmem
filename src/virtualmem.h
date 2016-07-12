#ifndef VIRTUALMEM_H
#define VIRTUALMEM_H


typedef enum {
    P_FIFO,
    P_LFU,
    P_LRU_STACK,
    P_LRU_CLOCK,
    P_LRU_REF8,
    P_OPTIMAL
} replacement_policy;

static inline char *pol_name(replacement_policy p) {
    static const char *strings[] = {
        "FIFO",
        "LFU",
        "LRU-STACK",
        "LRU-CLOCK",
        "LRU-REF8"
    };
    return strings[p];
}

replacement_policy policy;
int frames;

int hasRef(int frameArray[], int frameCount, int ref);

#endif
