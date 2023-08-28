#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <byteStream.h>

#ifdef __linux__
#define _GNU_SOURCE
#include <unistd.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <fcntl.h>

//example from man page
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

#endif

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#endif

unsigned char buffer[500];
unsigned char *ret;
size_t size;

void benchPrintf(const char *functionName, const char *desc, double time, unsigned long long instructions){
        printf("| %-26s | %-40s | %-15f | %-15llu |\n", functionName, 
                                                        desc, 
                                                        time,
                                                        instructions);
}

void functionRunner(const char *name, const char *desc, ByteStream **stream, void (*functionPtr)(ByteStream **)){
    
    unsigned long long instrcutions = 0.0;
    double time = 0.0;

#ifdef __linux__
    struct perf_event_attr pe;
    int fd;
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
    functionPtr(stream);
    iend = __rdtsc();
    QueryPerformanceCounter(&end);
    time = (double) ((end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;
    instrcutions = iend - istart;

#endif
    benchPrintf(name, desc, time, instrcutions);
}


static void byteStreamFromFile_Bench(ByteStream **stream){
#ifdef __linux__    
    *stream = byteStreamFromFile("assets/utf16.txt");
#endif
#ifdef _WIN32
    *stream = byteStreamFromFile("..\\assets\\utf16.txt");
#endif
}

static void byteStreamFromFileV2_Bench(ByteStream **stream){
#ifdef __linux__    
    *stream = byteStreamFromFile("assets/latin1.txt");
#endif
#ifdef _WIN32
    *stream = byteStreamFromFile("..\\assets\\latin1.txt");
#endif
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

static void byteStreamReturnInt_Bench(ByteStream **stream){
    byteStreamReturnInt(*stream);
}

static void byteStreamReturnSyncInt_Bench(ByteStream **stream){
    byteStreamReturnInt(*stream);
}

static void byteStreamRewind_Bench(ByteStream **stream){
    byteStreamRewind(*stream);
}

static void byteStreamTell_Bench(ByteStream **stream){
    byteStreamRewind(*stream);
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

    printf("%s\n",__FILE__);
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
    functionRunner("byteStreamRead","Read 5 bytes", &stream, byteStreamRead_Bench);
    byteStreamDestroy(stream);
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf8-complex.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf8-complex.txt");
#endif
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
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf8-complex.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf8-complex.txt");
#endif
    functionRunner("byteStreamReadUntil","Read until 0xEF ~11862 bytes", &stream, byteStreamReadUntil_Bench);
    byteStreamDestroy(stream);
    free(ret);
#ifdef __linux__
    stream = byteStreamFromFile("assets/ascii.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\ascii.txt");
#endif
    functionRunner("byteStreamReadUntil","Read until \'p\' 1 byte", &stream, byteStreamReadUntilV2_Bench);
    byteStreamDestroy(stream);
    free(ret);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamSearchAndReplace benchmark
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf8-simple.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf8-simple.txt");
#endif
    functionRunner("byteStreamSearchAndReplace","Replace \"Cyrillic\" with nothing", &stream, byteStreamSearchAndReplace_Bench);
    byteStreamDestroy(stream);
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf8-complex.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf8-complex.txt");
#endif
    functionRunner("byteStreamSearchAndReplace","Change euros to dollars", &stream, byteStreamSearchAndReplaceV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamCursor benchmark
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf16.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf16.txt");
#endif
    functionRunner("byteStreamCursor","Return cursor", &stream, byteStreamCursor_Bench);
    
    stream->cursor = 128;
    functionRunner("byteStreamCursor","Return cursor again", &stream, byteStreamCursor_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamgetCh benchmark
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf16.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf16.txt");
#endif
    functionRunner("byteStreamGetCh","Return first byte", &stream, byteStreamGetCh_Bench);
    stream->cursor = 128;
    functionRunner("byteStreamGetCh","Return the 128th byte", &stream, byteStreamGetCh_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamWrite benchmark
    stream = byteStreamCreate(NULL, 800);
    functionRunner("byteStreamWrite","Write \"onions\" to a stream", &stream, byteStreamWrite_Bench);
    stream->cursor = 500;
    functionRunner("byteStreamWrite","Write a staircase to a stream", &stream, byteStreamWriteV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamWriteAtPosition benchmark
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf16.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf16.txt");
#endif
    functionRunner("byteStreamWriteAtPosition","Write at the 100th byte", &stream, byteStreamWriteAtPosition_Bench);
    functionRunner("byteStreamWriteAtPosition","Write at the 50th byte", &stream, byteStreamWriteAtPositionV2_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamReturnAscii benchmark
#ifdef __linux__
    stream = byteStreamFromFile("assets/ascii.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\ascii.txt");
#endif
    byteStreamSearchAndReplace(stream, (unsigned char *)"\n", 1, (unsigned char *)"\0", 1);
    byteStreamSearchAndReplace(stream, (unsigned char *)" ", 1, (unsigned char *)"\0", 1);
    functionRunner("byteStreamReturnAscii","Return the first ascii str", &stream, byteStreamReturnAscii_Bench);
    free(ret);
    functionRunner("byteStreamReturnAscii","Return the second ascii str", &stream, byteStreamReturnAscii_Bench);
    free(ret);
    functionRunner("byteStreamReturnAscii","Return the third ascii str", &stream, byteStreamReturnAscii_Bench);
    free(ret);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamReturnUtf16 benchmark
#ifdef __linux__
    stream = byteStreamFromFile("assets/utf16.txt");
#endif
#ifdef _WIN32
    stream = byteStreamFromFile("..\\assets\\utf16.txt");
#endif
    byteStreamSearchAndReplace(stream, (unsigned char *)"\n", 1, (unsigned char *)"\0", 1);
    functionRunner("byteStreamReturnUtf16","Return the first utf16 str", &stream, byteStreamReturnUtf16_Bench);
    free(ret);
    functionRunner("byteStreamReturnUtf16","Return the second utf16 str", &stream, byteStreamReturnUtf16_Bench);
    free(ret);
    functionRunner("byteStreamReturnUtf16","Return the third utf16 str", &stream, byteStreamReturnUtf16_Bench);
    free(ret);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStreamReturnInt benchmark
    stream = byteStreamCreate(NULL, 8);
    byteStreamWrite(stream, "\0\0\0\xfe", 4);
    byteStreamWrite(stream, "\0\0\0\x03", 4);
    byteStreamSeek(stream, 0, SEEK_SET);
    functionRunner("byteStreamReturnInt","return 254", &stream, byteStreamReturnInt_Bench);
    functionRunner("byteStreamReturnInt","return 3", &stream, byteStreamReturnInt_Bench);
    byteStreamRewind(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamReturnSyncInt
    functionRunner("byteStreamReturnSyncInt","return 254 as syncint", &stream, byteStreamReturnSyncInt_Bench);
    functionRunner("byteStreamReturnSyncInt","return 3 as syncint", &stream, byteStreamReturnSyncInt_Bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamRewind
    functionRunner("byteStreamRewind","rewind the stream", &stream, byteStreamRewind_Bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    
    //byteStreamTell
    functionRunner("byteStreamTell","how many bytes into the stream am i?", &stream, byteStreamTell_Bench);
    byteStreamDestroy(stream);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    return 0;
}
