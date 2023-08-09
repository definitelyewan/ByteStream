#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>

#include <byteStream.h>
ByteStream *stream;


void benchPrintf(const char *functionName, const char *desc, double start, double end, unsigned long long instructions){
        printf("| %-20s | %-40s | %-15f | %-15llu |\n", functionName, 
                                                       desc, 
                                                       ((double) end - start) / (CLOCKS_PER_SEC / 1000),
                                                       instructions);
}


//example from man page
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void functionRunner(const char *name, const char *desc, void (*functionPtr)()) {
    struct perf_event_attr pe;
    int fd;
    unsigned long long instrcutions;
    clock_t start;
    clock_t end;

    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if(fd == -1){
        perror("Error opening perf event\n");
        exit(1);
    }

    
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
    start = clock();
    functionPtr();
    end = clock();
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    

    read(fd, &instrcutions, sizeof(unsigned long long));
    close(fd);

    benchPrintf(name, desc, start, end, instrcutions);

}


static void byteStreamFromFile_Bench(void){
    stream = byteStreamFromFile("utf16.txt");
}

static void byteStreamFromFileV2_Bench(void){
    stream = byteStreamFromFile("latin1.txt");
}

static void byteStreamCreate_Bench(void){
    stream = byteStreamCreate(NULL, 1000);
}

static void byteStreamCreateV2_Bench(void){
    stream = byteStreamCreate((unsigned char *)"abcdefghijklmnopqrstuvwxyz1234567890-=[]\\;',./<>?:\"{}|+_()*&^$#@!", 66);
}


int main() {

    /**
     * Note that there will be no benchmarks for the functions byteStreamFree and byteStreamDestroy
     * this is cuz they just free memory and im 99% sure its impossible for me to make them faster
     */

    printf("| %-20s | %-40s | %-15s | %-15s |\n", "Function Name", "Description", "Execution Time", "Instructions");
    printf("|----------------------|------------------------------------------|-----------------|-----------------|\n");
    
    //byteStreamFromFile benchmark
    functionRunner("byteStreamFromFile","Used utf16.txt to create a stream",byteStreamFromFile_Bench);
    byteStreamDestroy(stream);
    functionRunner("byteStreamFromFile","Used latin1.txt to create a stream", byteStreamFromFileV2_Bench);
    byteStreamDestroy(stream);

    //byteStreamCreate benchmark
    functionRunner("byteStreamCreate","Create an empty stream with 1000 bytes", byteStreamCreate_Bench);
    byteStreamDestroy(stream);
    functionRunner("byteStreamCreate","Create an ascii stream", byteStreamCreateV2_Bench);
    byteStreamDestroy(stream);


    return 0;
}
