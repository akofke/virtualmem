#ifndef VIRTUALMEM_H
#define VIRTUALMEM_H


typedef enum {
    P_FIFO,
    P_LFU,
    P_LRU_STACK,
    P_LRU_CLOCK,
    P_LRU_REF8
} replacement_policy;

replacement_policy policy;
int frames;

int hasRef(int frameArray[], int frameCount, int ref);

#endif
