#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int lruStack(int framenum, int pagenum, int pages[]){
    bool there=false;				//set up for nested loop
    int rep=0;int index=0; 			//record replacements for output
    int frames[framenum];	 		//create the frames
    for(int i=0;i<framenum;i++) frames[i] = -1; //set all frames to "empty"
    int temp[framenum];				//set up for nested loop
    for(int i=0;i<pagenum;i++){
        for(int j=0; j<framenum;j++){
            if(pages[i]==frames[j]){
                there=true;
                index=j;
            }
        }
        if(there){
            for(int k=0;k<index;k++){temp[k+1]=frames[k];}
            for(int k=0;k<index;k++){frames[k+1]=temp[k+1];}
            frames[0]=pages[i];
        }
        if(!there){
            for(int k=0;k<framenum-1;k++){temp[k+1]=frames[k];}
            for(int k=0;k<framenum;k++){frames[k]=temp[k];}
            frames[0]=pages[i];
            rep++;
        }
        there=false;
    
    }
    return rep;
}
