/*
 * USAGE - Include the header file LRUSTACK_LRUCLOCK.h, then:
 * 	FIFO(int refList[], int refCount, in frameCount) and LFU (int refList[], int refCount, int frameCount) are used with the same arguments:
 * 		refList is the references read from the input file as an int array.
 * 		refCount is the amount of elements in refList.
 * 		frameCount is the amount of frames available as requested by command line arg
 *
 *	hasRef(int frameArray[], int frameCount, int ref) is meant to be used within this code only: given an int array, the size of said array, and an
 *	int, it either tells you 1 + the index of that int, or 0 if that int is not in the array.
 *
 * That's all there is to it.
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <ctype.h> 
#include <assert.h> 
#include <string.h> 

#include "virtualmem.h"



int FIFO(int refList[], int refCount, int frameCount){
    int frameArray[frameCount];
    for (int i = 0; i < frameCount; i++){//zero all the frames
        frameArray[i]=0;
    }
    int curRef = 0; //init curRef reference
    int missCount = 0; //init page table miss counter for answer
    int refHit = 1; //refHit has 3 stages: look for frame, look for victim frame, or frame found
    int foundFrame;

    for (int i = 0; i<refCount; i++){ //go through all references one after the other
        foundFrame=0;
        curRef=refList[i]; // track current reference
        //printf("Current Array:");
        //for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameArray[printer]); }
        //printf("\n");

        for (int x = 0; x < frameCount; x++){ // look for the frame
            if (frameArray[x]==curRef && foundFrame!=1){ // if a frame is found, push the rest of the stack down on it, and put it on top
                foundFrame=1;
                x++;
            }
        }

        if (foundFrame==0){ //if the frame is never found, remove bottom of stack, push new frame on top.
            for (int x = 1; x < frameCount; x++){
                frameArray[x-1]=frameArray[x];
                if (x==frameCount-1){
                    frameArray[x]=curRef;
                }
            }
            missCount++;
        }
    }

    return missCount;
}

int LFU(int refList[], int refCount, int frameCount){
    int frameArray[frameCount];
    int frameUsageCounter[frameCount];
    for (int i = 0; i < frameCount; i++){//zero all the frames and frame counters
        frameArray[i]=0;
        frameUsageCounter[i]= -1;
    }
    int vPointer=0;//init victim pointer to zero
    int curRef = 0; //init curRef reference
    int missCount = 0; //init page table miss counter for answer
    int refHit = 0; //refHit has 2 stages, look and found.
    int hasEmptyFrame = 1;

    // Before all frames are full
    for (int i = 0; i<refCount; i++){ //go through all references one after the other
        //printf("Current Array:");
        //for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameArray[printer]); }
        //printf("\nCurrent Posit:");
        //for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameUsageCounter[printer]); }
        //printf("\n");
        curRef=refList[i]; //track current reference
        int x;
        if (x=hasRef(frameArray, frameCount, curRef)!=0){
            x--;
            frameUsageCounter[x]++;
        }
        else {
            missCount++;
            int victimUsageCount=refCount;
            int victimFrame = 0;
            for (int x = 0; x < frameCount; x++){// find the least referenced frame
                if (frameUsageCounter[x]<victimUsageCount){
                    victimUsageCount = frameUsageCounter[x];
                    victimFrame = x;
                }
            }
            frameArray[victimFrame] = curRef; //replace the least referenced frame
            frameUsageCounter[victimFrame] = 0;
        }
    }

    return missCount;
}
