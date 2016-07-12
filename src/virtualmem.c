#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

#include "virtualmem.h"
#include "algos.h"

#define DEFAULT_FRAMES 5
#define DEFAULT_POLICY P_FIFO

#define TIME(alg) start=clock();pfs=alg;stop=clock()

void usage(int is_err);
int *read_file(FILE *fp, int *numrefs);
void time_algo(replacement_policy policy, int *refs, int numrefs, int frames, double *time, int *pagefaults);

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
    /* int i; */
    /* for(i = 0; i < numrefs; i++) { */
    /*     printf("%d\n", refs[i]); */
    /* } */

    // chosen policy and optimal algo time and pagefaults
    double poltime, opttime;
    int polpfs, optpfs;

    time_algo(policy, refs, numrefs, frames, &poltime, &polpfs);
    time_algo(P_OPTIMAL, refs, numrefs, frames, &opttime, &optpfs);

    const char *format =
        "Number of page replacements with %1$s: %2$d\n"
        "Number of page replacements with Optimal: %3$d\n"
        "Percent page replacement penalty using %1$s: %4$f%%\n\n"
        "Total time to run %1$s: %5$f msec\n"
        "Total time to run Optimal algorithm: %6$f msec\n"
        "%1$s is %7$f%% faster than Optimal algorithm.\n";

    printf(
            format,
            pol_name(policy),
            polpfs,
            optpfs,
            (double)(polpfs - optpfs) / (double)(polpfs) * 100.0,
            poltime,
            opttime,
            (opttime - poltime) / (opttime) * 100.0
          );

    




}

void time_algo(replacement_policy policy, int *refs, int numrefs, int frames, double *time, int *pagefaults) {
    clock_t start, stop;

    /*
     * faster than writing to a potentially distant memory address inside the timer
     */
    int pfs;

    switch(policy) {
        case P_FIFO:
            TIME(FIFO(refs, numrefs, frames));
            break;
        case P_LFU:
            TIME(LFU(refs, numrefs, frames));
            break;
        case P_LRU_STACK:
            TIME(lruStack(frames, numrefs, refs));
            break;
        case P_LRU_CLOCK:
            TIME(lruClock(frames, numrefs, refs));
            break;
        case P_LRU_REF8:
            TIME(LRU_8BIT(refs, numrefs, frames));
            break;
        case P_OPTIMAL:
            TIME(OPTIMAL(refs, numrefs, frames));
            break;
        default:
            abort();

    }

    *time = ((double)(stop-start) * 1000.0) / CLOCKS_PER_SEC;
    *pagefaults = pfs;
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
