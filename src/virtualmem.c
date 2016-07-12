#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include "virtualmem.h"


int main(int argc, char *argv[]) {
    
    policy = FIFO;
    frames = 10;
    FILE *infile = stdin;

    int c;
    while((c = getopt(argc, argv, "hf:r:i:")) != -1) {
        switch(c) {
            case 'f':
                frames = atoi(optarg);
                break;
            case 'r':
                if(strcasecmp(optarg, "FIFO") == 0) { policy = FIFO; }
                else if(strcasecmp(optarg, "LFU") == 0) { policy = LFU; }
                else if(strcasecmp(optarg, "LRU-STACK") == 0) { policy = LRU_STACK; }
                else if(strcasecmp(optarg, "LRU-CLOCK") == 0) { policy = LRU_CLOCK; }
                else if(strcasecmp(optarg, "LRU-REF8") == 0) { policy = LRU_REF8; }
                else { usage(1); }
                break;
            case 'i':
                assert(strlen(optarg) < PATH_MAX);
                if ((infile = fopen(optarg, "r")) == NULL) {
                    fprintf(stderr, "Error opening file\n\n");
                    usage(1);
                }

                break;
            case 'h':
                usage(0);
                break;
            case '?':
                usage(1);
                break;
            default:
                abort();

        }
    }

}

int *read_file(FILE *fp) {
    char *line = NULL;
    size_t len = 0;

    getline(&line, &len, fp);
}

void usage(int is_err) {
    /* prints usage and help. Prints to stdout or stderr depending on if its invoked
     * as an error or from -h.
     * TODO: add more help
     */
    char *msg = "Usage: virtualmem "
        "[-h] [-f available-frames] [-r replacement-policy] [-i input-file]"; 

    fprintf((is_err) ? stderr : stdout, "%s\n", msg);
    exit(is_err);
}
