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
#include <byteEndian.h>

void benchPrintf(const char *functionName, const char *desc, double start, double end, unsigned long long instructions){
        printf("| %-26s | %-40s | %-15f | %-15llu |\n", functionName, 
                                                        desc, 
                                                        ((double) end - start) / (CLOCKS_PER_SEC / 1000),
                                                        instructions);
}

//example from man page
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void functionRunner(const char *name, const char *desc, void (*functionPtr)(void)){
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

    //run and count function
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

static void byteEndianess_bench(void){
    byteEndianess();
}

static void byteSetEndianess_bench(void){
    byteSetEndianess(false);
}

int main(){

    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    printf("| %-26s | %-40s | %-15s | %-15s |\n", "Function Name", "Description", "Run Time (ms)", "Instructions");
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteEndianess
    functionRunner("byteEndianess", "calls the function", byteEndianess_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    //byteSetEndianess
    functionRunner("byteSetEndianess", "sets big endian", byteSetEndianess_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

}