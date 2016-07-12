/*
 * USAGE - Include the header file LRU8BIT_OPTIMAL.h, then:
 * 	LRU_8BIT(int refList[], int refCount, in frameCount) and OPTIMAL (int refList[], int refCount, int frameCount) are used with the same arguments:
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


int hasRef(int frameArray[], int frameCount, int ref){
	printf("Looking for ref %i \n", ref);
	for (int zebra = 0; zebra <frameCount; zebra++){ //look to see if reference already in there
		if (frameArray[zebra]==ref){
			printf("Found ref %i in ref %i \n", ref, zebra);
			return zebra+1;
		}
	}
	printf("Did not find ref %i \n", ref);
	return 0;
}

int LRU_8BIT(int refList[], int refCount, int frameCount){
	int frameArray[frameCount];
	int frameCounter[frameCount];
	for (int i = 0; i < frameCount; i++){//zero all the frames and frame counters
		frameArray[i]=0;
		frameCounter[i]=0;
	}
	int vPointer=0;//init victim pointer to zero
	int curRef = 0; //init curRef reference
	int missCount = 0; //init page table miss counter for answer
	int refHit = 1; //refHit has 3 stages: look for frame, look for victim frame, or frame found
	int hasEmptyFrame = 1;
	
	for (int i = 0; i<refCount; i++){ //go through all references one after the other
		curRef=refList[i]; // track current reference
		printf("Current Array:");
		for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameArray[printer]); }
		printf("\nCurrent Posit:");
		for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameCounter[printer]); }
		printf("\n");
		int x = vPointer;
		if (hasEmptyFrame){// fill initial frames
			if(hasRef(frameArray, frameCount, curRef)==0){//if it's not there, replace empty frame with it.
				frameArray[x]=curRef;
				missCount++;
			}
			for (int z = 0; z < frameCount; z++){//update frame counters
				frameCounter[z]=((frameCounter[z]-(frameCounter[z]%2))/2);
				if (frameArray[z]==curRef)
					frameCounter[z] = frameCounter[z]+128;
			}
			if (vPointer<frameCount-1)
				vPointer++;
			else{
				hasEmptyFrame = 0;
				vPointer=0;
			}
		}
		else{
			refHit=1;
			while (refHit!=0){ //for all frames in the table
				if (refHit==1){
					if (hasRef(frameArray, frameCount, curRef)){
						refHit=0;
					}
					else{ //if end is reached before frame is found, switch to state 2, and increment missCount
						refHit=2;
						missCount++;
					}
				}
				else if (refHit==2){ //in state 2, find the frame with the lowest counter and replace it, then go to state 0
					int victimFrame = 0;
					int victimFrameCounter = 256;
					for (int z = 0; z < frameCount; z++){ // find the frame with the lowest counter
						if (frameCounter[z]<victimFrameCounter){
							victimFrame = z;
							victimFrameCounter = frameCounter[z];
						}
					} //replace ref in victim frame with curRef
					frameArray[victimFrame] = curRef;
					frameCounter[victimFrame]=0; //reset victim frame counter
					refHit=0;
				}
				if (refHit==0){ //if stage 0 is entered, decrement the counters, add 128 to the page requested, and exit loop (get the next reference)
					for (int z = 0; z < frameCount; z++){
						frameCounter[z]=((frameCounter[z]-(frameCounter[z]%2))/2);
						if (frameArray[z]==curRef)
							frameCounter[z] = frameCounter[z]+128;
					}
				}
			}
		}
	}
	
	return missCount;
}

int OPTIMAL (int refList[], int refCount, int frameCount){
	int frameArray[frameCount];
	int frameNextRef[frameCount];
	for (int i = 0; i < frameCount; i++){//zero all the frames and frame counters
		frameArray[i]=0;
		frameNextRef[i]=0;
	}
	int vPointer=0;//init victim pointer to zero
	int curRef = 0; //init curRef reference
	int missCount = 0; //init page table miss counter for answer
	int refHit = 1; //refHit has 3 stages: look for frame, look for victim frame, or frame found
	int hasEmptyFrame = 1;
	
	// Before all frames are full
	for (int i = 0; i<refCount; i++){ //go through all references one after the other
		printf("Current Array:");
		for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameArray[printer]); }
		printf("\nCurrent Posit:");
		for (int printer = 0; printer<frameCount; printer++){ printf(" %i", frameNextRef[printer]); }
		printf("\n");
		curRef=refList[i]; // track current reference
		int x = vPointer;
		int y;
		if (hasEmptyFrame){// fill initial frames
			if(hasRef(frameArray, frameCount, curRef)==0){//if it's not there, replace empty frame with it.
				frameArray[x]=curRef;
				frameNextRef[x]=1; //update counter (use a loop) to the distance to next ref
				missCount++;
				if (vPointer<frameCount-1){
					vPointer++;
				}
				else{
					hasEmptyFrame = 0;
					vPointer=0;
				}
			}
			
			for (int z = 0; z<frameCount; z++){
				frameNextRef[z]--;
				if (frameNextRef[z]==0 && frameArray[z]!=0){
							for (int g = i+1; g<refCount; g++){
								frameNextRef[z]++;
								if (refList[g]==curRef){
									g = refCount; // break loop
								}
								else if (g==refCount-1){
									frameNextRef[z]++;
								}
							}
						}
			}
			
		}
		// After all frames are full
		else{
			refHit=1;
			printf("starting in state 1 \n");
			while (refHit!=0){ //for all frames in the table
				if (refHit==1){
					if (hasRef(frameArray, frameCount, curRef)!=0){
						refHit=0;
						printf("found ref, entering state 0 \n");
					}
					else { //if end is reached before frame is found, switch to state 2, and increment missCount
						refHit=2;
						printf("never found ref, entering state 2 \n");
						missCount++;
					}
				}
				else if (refHit==2){ //in state 2, find the frame with the highest counter and replace it, then go to state 0
					int victimFrame = 0;
					int victimFrameNextRef = 0;
					for (int z = 0; z < frameCount; z++){ // find the frame with the highest counter
						if (frameNextRef[z]>victimFrameNextRef){
							victimFrame = z;
							victimFrameNextRef = frameNextRef[z];
						}
					} //replace ref in victim frame with curRef
					printf("selected ref %i in frame %i as victim \n", frameArray[victimFrame], victimFrame);
					frameArray[victimFrame] = curRef;
					frameNextRef[victimFrame]=1; //reset victim frame counter
					printf("replaced ref, entering state 0 \n");
					refHit=0;
				}
				if (refHit==0){ //if stage 0 is entered, decrement the counters. if 0 after update, update nextRef for that frame
					for (int z = 0; z < frameCount; z++){
						frameNextRef[z]--;
						if (frameNextRef[z]==0){
							for (int g = i+1; g<refCount; g++){
								frameNextRef[z]++;
								if (refList[g]==curRef){
									g = refCount; // break loop
								}
								else if (g==refCount-1){
									frameNextRef[z]++;
								}
							}
						}
					}
				}
			}
		}
	}
	
	return missCount;
}

int
main(int argc,char *argv[])//for debug, remove if you see me
{ 
	int r[5];
	int framenum = 3;
	
	r[0]=1;
	r[1]=2;
	r[2]=4;
	r[3]=3;
	r[4]=1;
	
	printf("LRU_8BIT: %i \nOPTIMAL: %i \n", LRU_8BIT(r, 5, 3), OPTIMAL(r, 5, 3));
	return 0; 
}

