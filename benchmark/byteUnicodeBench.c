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

#include <byteUnicode.h>

unsigned char *out;
size_t outl;


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

static void byteIsUtf8_bench(void){
    byteIsUtf8((unsigned char *)"première is first première is slightly different Кириллица is Cyrillic 𐐀 am Deseret");
    
}

static void byteIsUtf8V2_bench(void){
    byteIsUtf8((unsigned char *)"แผ่นดินฮั่นเสื่อมโทรมแสนสังเวช");
    
}

static void byteUtf16Strlen_bench(void){
    byteUtf16Strlen((unsigned char *)"\x28\x09\x47\x09\x2a\x09\x3e\x09\x32\x09\x20\x00\x2c\x09\x3e\x09\x1f\x09\x20\x00\x28\x09\x2e\x09\x38\x09\x4d\x09\x15\x09\x3e\x09\x30\x09\0\0");
    
}

static void byteUtf16StrlenV2_bench(void){
    byteUtf16Strlen((unsigned char *)"\xff\xfe\0\0");
    
}

static void byteStrlen_bench(void){
    byteStrlen(BYTE_UTF8, (unsigned char *)"Hello world, Καλημέρα κόσμε, コンニチハ");
    
}

static void byteStrlenV2_bench(void){
    byteStrlen(BYTE_UTF16LE, (unsigned char *)"\x28\x09\x47\x09\x2a\x09\x3e\x09\x32\x09\x20\x00\x2c\x09\x3e\x09\x1f\x09\x20\x00\x28\x09\x2e\x09\x38\x09\x4d\x09\x15\x09\x3e\x09\x30\x09\0\0");
    
}

static void byteStrlenV3_bench(void){
    byteStrlen(9, (unsigned char *)"0000000");
    
}

static void byteIsLegalEncoding_bench(void){
    byteIsLegalEncoding(BYTE_ISO_8859_1);
    
}

static void byteIsLegalEncodingV2_bench(void){
    byteIsLegalEncoding(BYTE_UTF8);
    
}

static void byteConvertTextFormat_bench(void){
    byteConvertTextFormat((unsigned char *)"ascii char :)", BYTE_ASCII, 14, &out, BYTE_UTF16BE, &outl);
    
}

static void byteConvertTextFormatV2_bench(void){
    byteConvertTextFormat((unsigned char *)"Кириллица", BYTE_UTF8, 19, &out, BYTE_UTF16LE, &outl);
    
}

static void byteConvertTextFormatV3_bench(void){
    byteConvertTextFormat((unsigned char *)"latin 1", BYTE_ISO_8859_1, 8, &out, BYTE_UTF8, &outl);
    
}

static void byteHasBOM_bench(void){
    byteHasBOM((unsigned char *)"\xff\xfe");
    
}

static void byteHasBOMV2_bench(void){
    byteHasBOM((unsigned char *)"\xfe\xff");
    
}

static void bytePrependBOM_bench(void){
    bytePrependBOM(BYTE_UTF16BE, &out, &outl);
}

static void bytePrependBOMV2_bench(void){
    bytePrependBOM(BYTE_UTF16LE, &out, &outl);
}

int main(){

    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
    printf("| %-26s | %-40s | %-15s | %-15s |\n", "Function Name", "Description", "Run Time (ms)", "Instructions");
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteIsUtf8 benchmark
    functionRunner("byteIsUtf8", "Tests a utf8 string", byteIsUtf8_bench);
    functionRunner("byteIsUtf8", "Tests a utf8 string", byteIsUtf8V2_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteUtf16Strlen benchmark
    functionRunner("byteUtf16Strlen", "Calculates an encoded str length", byteUtf16Strlen_bench);
    functionRunner("byteUtf16Strlen", "Calculates an encoded str length", byteUtf16StrlenV2_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteStrlen benchmark
    functionRunner("byteStrlen", "Calculate Utf8 string length", byteStrlen_bench);
    functionRunner("byteStrlen", "Calculate Utf16 string length", byteStrlenV2_bench);
    functionRunner("byteStrlen", "Calculate idk string length", byteStrlenV3_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteIsLegalEncoding
    functionRunner("byteIsLegalEncoding", "encoding is latin1", byteIsLegalEncoding_bench);
    functionRunner("byteIsLegalEncoding", "encoding is utf8", byteIsLegalEncodingV2_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteConvertTextFormat
    functionRunner("byteConvertTextFormat", "ascii to utf16be", byteConvertTextFormat_bench);
    free(out);
    functionRunner("byteConvertTextFormat", "utf8 to utf16le", byteConvertTextFormatV2_bench);
    free(out);
    functionRunner("byteConvertTextFormat", "latin1 to utf8", byteConvertTextFormatV3_bench);
    free(out);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //byteHasBOM
    functionRunner("byteHasBOM", "0xff 0xfe", byteHasBOM_bench);
    functionRunner("byteHasBOM", "0xfe 0xff", byteHasBOMV2_bench);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");

    //bytePrependBOM
    out = calloc(1, 47);
    outl = 47;
    strcpy((char *)out, "pretend this is a utf16 string of some kind :)");
    functionRunner("bytePrependBOM", "prepend be bom", bytePrependBOM_bench);
    free(out);

    out = calloc(1, 37);
    outl = 37;
    strcpy((char *)out, "I am a string to prepend a bom to :)");
    functionRunner("bytePrependBOM", "prepend le bom", bytePrependBOMV2_bench);
    free(out);
    printf("+----------------------------+------------------------------------------+-----------------------------------+\n");
}