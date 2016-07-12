int FIFO(int refList[], int refCount, int frameCount);
int LFU(int refList[], int refCount, int frameCount);
int LRU_8BIT(int refList[], int refCount, int frameCount);
int OPTIMAL(int refList[], int refCount, int frameCount);
int lruClock(int framenum, int pagenum, int pages[]);
int lruStack(int framenum, int pagenum, int pages[]);
