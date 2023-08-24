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
#include <limits.h>
#include <byteInt.h>

char *ret;

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

static void btoi_bench(void){
    btoi("\xff",1);
}

static void btoi2_bench(void){
    btoi("\xff\xfe\xfd\xfc",4);
}

static void itob_bench(void){
    ret = itob(50);
}

static void itob2_bench(void){
    ret = itob(INT_MAX);
}

static void byteSyncintDecode_bench(void){
    byteSyncintDecode(999);
}

static void byteSyncintEncode_bench(void){
    byteSyncintEncode(999);
}

int main(){

    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    printf("| %-26s | %-40s | %-15s | %-15s |\n", "Function Name", "Description", "Run Time (ms)", "Instructions");
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //btoi
    functionRunner("btoi", "converts FF", btoi_bench);
    functionRunner("btoi", "converts FFFEFDFC", btoi2_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //itob
    functionRunner("itob", "converts 50", itob_bench);
    free(ret);
    functionRunner("itob", "converts INT_MAX", itob2_bench);
    free(ret);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteSyncintDecode
    functionRunner("byteSyncintDecode", "converts 999", byteSyncintDecode_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteSyncintEncode
    functionRunner("byteSyncintEncode", "converts 999", byteSyncintEncode_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

}