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

unsigned char buffer[500];
unsigned char *ret;
size_t size;

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

void functionRunner(const char *name, const char *desc, ByteStream **stream, void (*functionPtr)(ByteStream **)){
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
    functionPtr(stream);
    end = clock();

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &instrcutions, sizeof(unsigned long long));
    close(fd);

    benchPrintf(name, desc, start, end, instrcutions);
}


static void byteStreamFromFile_Bench(ByteStream **stream){
    *stream = byteStreamFromFile("utf16.txt");
}

static void byteStreamFromFileV2_Bench(ByteStream **stream){
    *stream = byteStreamFromFile("latin1.txt");
}

static void byteStreamCreate_Bench(ByteStream **stream){
    *stream = byteStreamCreate(NULL, 1000);
}


static void byteStreamCreateV2_Bench(ByteStream **stream){
    *stream = byteStreamCreate((unsigned char *)"abcdefghijklmnopqrstuvwxyz1234567890-=[]\\;',./<>?:\"{}|+_()*&^$#@!", 66);
}


static void byteStreamRead_Bench(ByteStream **stream){
    byteStreamRead(*stream, buffer, 5);
}

static void byteStreamReadV2_Bench(ByteStream **stream){
    byteStreamRead(*stream, buffer, 1000);
}

static void byteStreamSeek_Bench(ByteStream **stream){
    byteStreamSeek(*stream, 500, SEEK_SET);
}

static void byteStreamSeekV2_Bench(ByteStream **stream){
    byteStreamSeek(*stream, 400, SEEK_CUR);
}

static void byteStreamSeekV3_Bench(ByteStream **stream){
    byteStreamSeek(*stream, 100, SEEK_END);
}

static void byteStreamReadUntil_Bench(ByteStream **stream){
    ret = byteStreamReadUntil(*stream, 0xef);
}

static void byteStreamReadUntilV2_Bench(ByteStream **stream){
    ret = byteStreamReadUntil(*stream, 'p');
}

static void byteStreamSearchAndReplace_Bench(ByteStream **stream){
    byteStreamSearchAndReplace(*stream, (unsigned char *)"Cyrillic", 8, NULL, 0);
}

static void byteStreamSearchAndReplaceV2_Bench(ByteStream **stream){
    byteStreamSearchAndReplace(*stream, (unsigned char *)"€", 3,(unsigned char *)"$$$", 3);
}

static void byteStreamCursor_Bench(ByteStream **stream){
    byteStreamCursor(*stream);
}

static void byteStreamGetCh_Bench(ByteStream **stream){
    byteStreamGetCh(*stream);
}

static void byteStreamWrite_Bench(ByteStream **stream){
    byteStreamWrite(*stream, (unsigned char *)"onions", 6);
}

static void byteStreamWriteV2_Bench(ByteStream **stream){
    byteStreamWrite(*stream, (unsigned char *)"▁▂▃▄▅▆▇█", 24);
}

static void byteStreamWriteAtPosition_Bench(ByteStream **stream){
    byteStreamWriteAtPosition(*stream, (unsigned char *)"pos 100", 7, 100);
}

static void byteStreamWriteAtPositionV2_Bench(ByteStream **stream){
    byteStreamWriteAtPosition(*stream, (unsigned char *)"⡌⠁⠧⠑ ⠼⠁⠒  ⡍⠜⠇⠑⠹⠰⠎ ⡣⠕⠌", 55, 160);
}

static void byteStreamReturnAscii_Bench(ByteStream **stream){
    ret = byteStreamReturnAscii(*stream, &size);
}

static void byteStreamReturnUtf16_Bench(ByteStream **stream){
    ret = byteStreamReturnUtf16(*stream, &size);
}

int main(){

    ByteStream *stream = NULL;

    /**
     * Note: that there will be no benchmarks for the functions byteStreamFree and byteStreamDestroy
     * this is cuz they just free memory and im 99% sure its impossible for me to make them faster
     * 
     * Note: byteStreamReturnUtf8 and byteStreamReturnLatin1 are not benched because they are wrappers
     * for byteStreamReturnAscii
     */

    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    printf("| %-26s | %-40s | %-15s | %-15s |\n", "Function Name", "Description", "Run Time (ms)", "Instructions");
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamFromFile benchmark
    functionRunner("byteStreamFromFile","Used utf16.txt to create a stream", &stream, byteStreamFromFile_Bench);
    byteStreamDestroy(stream);
    functionRunner("byteStreamFromFile","Used latin1.txt to create a stream", &stream, byteStreamFromFileV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamCreate benchmark
    functionRunner("byteStreamCreate","Create an empty stream with 1000 bytes", &stream, byteStreamCreate_Bench);
    byteStreamDestroy(stream);
    functionRunner("byteStreamCreate","Create an ascii stream", &stream, byteStreamCreateV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamRead benchmark
    stream = byteStreamCreate(NULL, 10);
    functionRunner("byteStreamRead","read 5 bytes", &stream, byteStreamRead_Bench);
    byteStreamDestroy(stream);
    stream = byteStreamFromFile("utf8-complex.txt");
    functionRunner("byteStreamRead","Read 1k bytes from utf8-complex.txt", &stream, byteStreamReadV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamSeek benchmark
    stream = byteStreamCreate(NULL, 1000);
    functionRunner("byteStreamSeek","SEEK_SET 500 bytes", &stream, byteStreamSeek_Bench);
    functionRunner("byteStreamSeek","SEEK_CUR 300 bytes", &stream, byteStreamSeekV2_Bench);
    functionRunner("byteStreamSeek","SEEK_END 100 bytes", &stream, byteStreamSeekV3_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamReadUntil benchmark
    stream = byteStreamFromFile("utf8-complex.txt");
    functionRunner("byteStreamReadUntil","read until 0xEF ~11862 bytes", &stream, byteStreamReadUntil_Bench);
    byteStreamDestroy(stream);
    free(ret);
    stream = byteStreamFromFile("ascii.txt");
    functionRunner("byteStreamReadUntil","read until \'p\' 1 byte", &stream, byteStreamReadUntilV2_Bench);
    byteStreamDestroy(stream);
    free(ret);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamSearchAndReplace benchmark
    stream = byteStreamFromFile("utf8-simple.txt");
    functionRunner("byteStreamSearchAndReplace","replace \"Cyrillic\" with nothing", &stream, byteStreamSearchAndReplace_Bench);
    byteStreamDestroy(stream);
    stream = byteStreamFromFile("utf8-complex.txt");
    functionRunner("byteStreamSearchAndReplace","change euros to dollars", &stream, byteStreamSearchAndReplaceV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamCursor benchmark
    stream = byteStreamFromFile("utf16.txt");
    functionRunner("byteStreamCursor","return cursor", &stream, byteStreamCursor_Bench);
    stream->cursor = 128;
    functionRunner("byteStreamCursor","return cursor again", &stream, byteStreamCursor_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamgetCh benchmark
    stream = byteStreamFromFile("utf16.txt");
    functionRunner("byteStreamGetCh","return first byte", &stream, byteStreamGetCh_Bench);
    stream->cursor = 128;
    functionRunner("byteStreamGetCh","return the 128th byte", &stream, byteStreamGetCh_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamWrite benchmark
    stream = byteStreamCreate(NULL, 800);
    functionRunner("byteStreamWrite","write \"onions\" to a stream", &stream, byteStreamWrite_Bench);
    stream->cursor = 500;
    functionRunner("byteStreamWrite","write a staircase to a stream", &stream, byteStreamWriteV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamWriteAtPosition benchmark
    stream = byteStreamFromFile("utf16.txt");
    functionRunner("byteStreamWriteAtPosition","write at the 100th byte", &stream, byteStreamWriteAtPosition_Bench);
    functionRunner("byteStreamWriteAtPosition","write at the 50th byte", &stream, byteStreamWriteAtPositionV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamReturnAscii benchmark
    stream = byteStreamFromFile("ascii.txt");
    byteStreamSearchAndReplace(stream, (unsigned char *)"\n", 1, (unsigned char *)"\0", 1);
    byteStreamSearchAndReplace(stream, (unsigned char *)" ", 1, (unsigned char *)"\0", 1);
    functionRunner("byteStreamReturnAscii","return the first ascii str", &stream, byteStreamReturnAscii_Bench);
    free(ret);
    functionRunner("byteStreamReturnAscii","return the second ascii str", &stream, byteStreamReturnAscii_Bench);
    free(ret);
    functionRunner("byteStreamReturnAscii","return the third ascii str", &stream, byteStreamReturnAscii_Bench);
    free(ret);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamReturnUtf16 benchmark
    stream = byteStreamFromFile("utf16.txt");
    byteStreamSearchAndReplace(stream, (unsigned char *)"\n", 1, (unsigned char *)"\0", 1);
    functionRunner("byteStreamReturnAscii","return the first utf16 str", &stream, byteStreamReturnUtf16_Bench);
    free(ret);
    functionRunner("byteStreamReturnAscii","return the second utf16 str", &stream, byteStreamReturnUtf16_Bench);
    free(ret);
    functionRunner("byteStreamReturnAscii","return the third utf16 str", &stream, byteStreamReturnUtf16_Bench);
    free(ret);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    return 0;
}
