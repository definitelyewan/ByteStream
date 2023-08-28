#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <byteEndian.h>

#ifdef __linux__
#define _GNU_SOURCE
#include <unistd.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <fcntl.h>

//example from man page but only works with linux
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

#endif

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#endif

void benchPrintf(const char *functionName, const char *desc, double time, unsigned long long instructions){
        printf("| %-26s | %-40s | %-15f | %-15llu |\n", functionName, 
                                                        desc, 
                                                        time,
                                                        instructions);
}


void functionRunner(const char *name, const char *desc, void (*functionPtr)(void)){

    double time = 0.0;
    unsigned long long instrcutions = 0;

#ifdef __linux__
    clock_t start;
    clock_t end;
    struct perf_event_attr pe;
    int fd;

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

    time = ((double) end - start) / (CLOCKS_PER_SEC / 1000);
#endif
#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    unsigned long long istart = 0;
    unsigned long long iend = 0;

    //for time
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    
    //for instructions on x86
    istart = __rdtsc();
    functionPtr();
    iend = __rdtsc();
    QueryPerformanceCounter(&end);
    time = (double) ((end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;
    instrcutions = iend - istart;

#endif
    benchPrintf(name, desc, time, instrcutions);
}

static void byteEndianess_bench(void){
    byteEndianess();
}

static void byteSetEndianess_bench(void){
    byteSetEndianess(false);
}

int main(){
    printf("%s\n",__FILE__);
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