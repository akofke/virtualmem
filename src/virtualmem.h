#ifndef VIRTUALMEM_H
#define VIRTUALMEM_H


typedef enum {
    FIFO,
    LFU,
    LRU_STACK,
    LRU_CLOCK,
    LRU_REF8
} replacement_policy;

replacement_policy policy;
int frames;

#endif
