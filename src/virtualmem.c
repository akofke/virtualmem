#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "virtualmem.h"


int main(int argc, char *argv[]) {
    
    policy = FIFO;
    frames = 10;

    int c;
    while((c = getopt(argc, argv, "hf:r:i:")) != -1) {
        switch(c) {
            case 'f':
                break;
            case 'r':
                if(strcasecmp(optarg, "FIFO") == 0) { policy = FIFO; }
                else if(strcasecmp(optarg, "LFU") == 0) { policy = LFU; }
                else if(strcasecmp(optarg, "LRU-STACK") == 0) { policy = LRU_STACK; }
                else if(strcasecmp(optarg, "LRU-CLOCK") == 0) { policy = LRU_CLOCK; }
                else if(strcasecmp(optarg, "LRU-REF8") == 0) { policy = LRU_REF8; }
                break;
            case 'i':
                break;
            case 'h':
                break;
            case '?':
                break;
            default:
                abort();

        }
    }

}
