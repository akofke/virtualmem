#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "virtualmem.h"

#define DEFAULT_FRAMES 5
#define DEFAULT_POLICY P_FIFO

void usage(int is_err);
int *read_file(FILE *fp, int *numrefs);

int main(int argc, char *argv[]) {
    
    policy = DEFAULT_POLICY;
    frames = DEFAULT_FRAMES;
    FILE *infile = stdin;

    int c;
    while((c = getopt(argc, argv, "hf:r:i:")) != -1) {
        switch(c) {
            case 'f':
                frames = atoi(optarg);
                break;
            case 'r':
                if(strcasecmp(optarg, "FIFO") == 0) { policy = P_FIFO; }
                else if(strcasecmp(optarg, "LFU") == 0) { policy = P_LFU; }
                else if(strcasecmp(optarg, "LRU-STACK") == 0) { policy = P_LRU_STACK; }
                else if(strcasecmp(optarg, "LRU-CLOCK") == 0) { policy = P_LRU_CLOCK; }
                else if(strcasecmp(optarg, "LRU-REF8") == 0) { policy = P_LRU_REF8; }
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

    int numrefs = 0;
    int *refs = read_file(infile, &numrefs);
    int i;
    for(i = 0; i < numrefs; i++) {
        printf("%d\n", refs[i]);
    }

}

int *read_file(FILE *fp, int *num_read) {
    char *line = NULL;
    size_t len = 0;

    getline(&line, &len, fp);

    /*
     * count the number of spaces in the line so we know
     * how large to allocate the array. Number of elements
     * is 1 more than number of spaces.
     */
    int count = 0;
    int i;
    for(i = 0; i < strlen(line); i++) {
        if(line[i] == ' ') { count++; }
    }

    int *refs_buf = (int *)malloc((sizeof(int)) * (count + 1));
    i = 0;
    char *token;
    while((token = strsep(&line, " ")) != NULL) {
        refs_buf[i] = atoi(token);
        i++;
    }
    free(line);
    *num_read = count + 1;
    return refs_buf;
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

int hasRef(int frameArray[], int frameCount, int ref){
    //printf("Looking for ref %i \n", ref);
    for (int zebra = 0; zebra <frameCount; zebra++){ //look to see if reference already in there
        if (frameArray[zebra]==ref){
            //printf("Found ref %i in ref %i \n", ref, zebra);
            return zebra+1;
        }
    }
    //printf("Did not find ref %i \n", ref);
    return 0;
}
