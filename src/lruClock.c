#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



void lruClock(int framenum, int pagenum,  int pages[]){
    int rep=0;bool acq=false; int vic=0;
    int frames[framenum];
    bool seccha[framenum];
    for(int i=0;i<framenum;i++) frames[i] = -1;
    for(int i=0;i<framenum;i++) seccha[i] = false;
    
    for(int i=0;i<pagenum;i++){			//for each page
        for(int j=0;j<framenum;j++){		//check them against each page in the frame
            if(frames[j]==pages[i]){
                seccha[j] = true;
                acq = true;
            }
        }
        while(!acq){
            if(seccha[vic]==true){
                seccha[vic]=false;
                vic++;
                if(vic>=framenum) vic=0;
            }
            if(seccha[vic]==false){
                frames[vic]=pages[i];
                vic++;
                rep++;
                acq=true;
            }
        }
    acq=false;
    }
    return rep;
}
