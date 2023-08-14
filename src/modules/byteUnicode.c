#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <byteUnicode.h>
#include <wchar.h>

//default is little endian because x86 is little endian 
static bool byteLittleEndian = 1;

/**
 * @brief this function is responsible for changing the endianess of
 * encoding conversion functions.
 * 
 * little endian is 1 and big endian is 0.
 * this function has no return value.
 * @param endianess 
 */
void byteSetEndianess(bool endianess){
    byteLittleEndian = endianess;
}

bool byteIsUtf8(const unsigned char *utf){
    
    size_t i = 0;
    unsigned char codePoint = 0;

    while((codePoint = utf[i]) != 0x00){
        if(codePoint & 0x80){
	        if((utf[i + 1] & 0xc0) != 0x80){
                return false;
            }
	            
            if((codePoint & 0xe0) == 0xe0){
                if((utf[i + 2] & 0xc0) != 0x80){
                    return false;
                }
                
                //4 byte code
                if((codePoint & 0xf0) == 0xf0){
                    if ((codePoint & 0xf8) != 0xf0 || (utf[i + 3] & 0xc0) != 0x80){
                        return false;
                    }
                        
                    i += 4;
                
	            //3 byte code
                }else{
                    i += 3;
                }
		        
	        //2 byte code
            }else{
                i += 2;
            }

        //1 byte code
        }else{
            i++;
        }
    }

    return true;
}

size_t byteUtf16Strlen(const unsigned char *utf){

    size_t len = 0;

    if(utf == NULL){
        return len;
    }
    
    while(true){
        //0x00 0x00 means the end of utf
        if(utf[len] == 0x00 && utf[len + 1] == 0x00){
            break;
        }

        len = len + 2;
    }

    return len;
}

size_t byteStrlen(unsigned char encoding, const unsigned char *bytes){

    if(bytes == NULL){
        return 0;
    }

    switch(encoding){
        case BYTE_ISO_8859_1:
        case BYTE_ASCII:
        case BYTE_UTF8:
            return strlen((char *)bytes);
        case BYTE_UTF16BE:
        case BYTE_UTF16LE:
            return byteUtf16Strlen(bytes);
        default:
            return 0;
    }

    return 0;
}

bool byteIsLegalEncoding(unsigned char encoding){

    switch(encoding){
        case BYTE_ISO_8859_1:
        case BYTE_ASCII:
        case BYTE_UTF16BE:
        case BYTE_UTF16LE:
        case BYTE_UTF8:
            return true;
        default:
            return false;
    }

}

bool byteConvertTextFormat(unsigned char *in, unsigned char inEncoding, size_t inLen, unsigned char **out, unsigned char outEncoding, size_t *outLen){

    if((!byteIsLegalEncoding(inEncoding)) || (!byteIsLegalEncoding(outEncoding))){
        *outLen = 0;
        return false;
    }

    if(out == NULL || in == NULL){
        *outLen = 0;
        return false;
    }

    //no work to do
    if(outEncoding == inEncoding){
        *outLen = 0;
        return true;
    }

    unsigned char *tmp = NULL;
    size_t sampleInLen = inLen;
    size_t sampleOutLen = 0;

    //conversion to utf8
    switch(inEncoding){
        case BYTE_ASCII:
        
            sampleOutLen = inLen * 4; //make a safe guess, the highest code is 4 so *4
            
            tmp = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);

            //try and convert to utf8
            if(!byteAsciiToUtf8(tmp, &sampleOutLen, in, &sampleInLen)){
                free(tmp);
                *outLen = 0;
                *out = NULL;
                return false;
            }

            break;
        case BYTE_ISO_8859_1:

            sampleOutLen = inLen * 4; //make a safe guess, the highest code is 4 so *4

            tmp = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);

            //try and convert to utf8
            if(!byteLat1ToUtf8(tmp, &sampleOutLen, in, &sampleInLen)){
                free(tmp);
                *outLen = 0;
                *out = NULL;
                return false;
            }

            break;
        case BYTE_UTF8:

            //utf 8 is used to validate no conversion is used
            sampleOutLen = inLen;

            tmp = calloc(sizeof(unsigned char), sampleInLen + 1);
            memcpy(tmp, in, sampleInLen);

            break;
        case BYTE_UTF16BE:

            sampleOutLen = inLen; //guess and will be shrunk later

            tmp = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);
            
            //try and convert to utf8
            if(!byteUtf16beToUtf8(tmp, &sampleOutLen, in, &sampleInLen)){
                free(tmp);
                *outLen = 0;
                *out = NULL;
                return false;
            }

            break;
        case BYTE_UTF16LE:

            sampleOutLen = inLen; //guess and will be shrunk later

            tmp = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);

            //try and convert to utf8
            if(!byteUtf16leToUtf8(tmp, &sampleOutLen, in, &sampleInLen)){
                free(tmp);
                *outLen = 0;
                *out = NULL;
                return false;
            }

            break;
        default:
            return false;
    }

    //shrink
    tmp = realloc(tmp, sampleOutLen + BYTE_PADDING);

    //was the provided text valid?
    if(!byteIsUtf8(tmp)){
        free(tmp);
        *out = NULL;
        return false;
    }

    switch(outEncoding){
        case BYTE_ASCII:

            //assume ascii is <= to utf8
            *out = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);
            
            if(!byteUtf8ToAscii(*out, &sampleOutLen, tmp, &sampleInLen)){
                *outLen = sampleOutLen;
                free(tmp);
                free(*out);
                *out = NULL;
                return false;
            }

            break;
        case BYTE_ISO_8859_1:
            //assume lat is <= to utf8
            *out = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);
            
            if(!byteUtf8ToLat1(*out, &sampleOutLen, tmp, &sampleInLen)){
                *outLen = sampleOutLen;
                free(tmp);
                free(*out);
                *out = NULL;
                return false;
            }

            break;
        case BYTE_UTF16BE:

            //safe guesses
            if(inEncoding == BYTE_UTF16LE){
                sampleOutLen = inLen;
            }else{
                sampleOutLen = sampleOutLen * 2;
            }

            *out = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);

            if(!byteUtf8ToUtf16be(*out, &sampleOutLen, tmp, &sampleInLen)){
                *outLen = sampleOutLen;
                free(tmp);
                free(*out);
                *out = NULL;
                return false;
            }

            break;
        case BYTE_UTF16LE:

            //safe guesses
            if(inEncoding == BYTE_UTF16BE){
                sampleOutLen = inLen;
            }else{
                sampleOutLen = sampleOutLen * 2;
            }

            *out = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);

            if(!byteUtf8ToUtf16le(*out, &sampleOutLen, tmp, &sampleInLen)){
                *outLen = sampleOutLen;
                free(tmp);
                free(*out);
                *out = NULL;
                return false;
            }

            break;
        //if this stage is reached the sample string is always valid utf8
        //if the desired encoding was utf8 this will trigger
        default: //UTF8
            *out = calloc(sizeof(unsigned char), sampleOutLen + BYTE_PADDING);
            memcpy(*out, tmp, sampleOutLen);
            break;
    }

    //shrink
    tmp = realloc(tmp, sampleOutLen + BYTE_PADDING);

    *outLen = sampleOutLen;
    free(tmp);

    return true;
}

bool byteHasBOM(const unsigned char *utf){

    if(utf == NULL){
        return false;
    }
    
    return ((utf[0] == 0xff && utf[1] == 0xfe) || 
            (utf[0] == 0xfe && utf[1] == 0xff)) ? true : false;
}

bool bytePrependBOM(unsigned char encoding, unsigned char **utf, size_t *utfLen){

    if(utf == NULL || *utfLen < 2){
        //do not change utflen
        return false;
    }

    if(*utf == NULL){
        return false;
    }

    unsigned char bom[BYTE_BOM_SIZE];
    
    if(encoding == BYTE_UTF16BE){
        bom[0] = 0xfe;
        bom[1] = 0xff;
    }else if(encoding == BYTE_UTF16LE){
        bom[0] = 0xff;
        bom[1] = 0xfe;
    }else{
        return false;
    }

    *utfLen += BYTE_BOM_SIZE;
    *utf = realloc(*utf, *utfLen + BYTE_PADDING);
    
    memcpy((*utf + BYTE_BOM_SIZE), *utf, (*utfLen) - BYTE_BOM_SIZE);
    memcpy(*utf, bom, BYTE_BOM_SIZE);

    return true;
}

bool byteAsciiToUtf8(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen){
    
    unsigned char* outstart = out;
    const unsigned char* base = in;
    const unsigned char* processed = in;
    unsigned char* outend = out + *outlen;
    const unsigned char* inend = in + *inlen;
    unsigned int c;
    int bits;

    while((in < inend) && (out - outstart + 5 < *outlen)){
        c = *in++;

        if(out >= outend){
            break;
        }
	    
        if(c < 0x80){  
            *out++ =  c;                
            bits = -6;
        }else{ 
            *outlen = out - outstart;
            *inlen = processed - base;
            return false;
	    }
 
        while(bits >= 0){
            if(out >= outend){
                break;
            }
	        
            *out++ = ((c >> bits) & 0x3F) | 0x80;
            bits -= 6;
        }

	    processed = (const unsigned char*) in;
    }

    *outlen = out - outstart;
    *inlen = processed - base;
    
    return true;
}

bool byteUtf8ToAscii(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen){
    
    //do nothing
    if(in == NULL){
        *outlen = 0;
        *inlen = 0;
        return true;
    }
    
    const unsigned char* processed = in;
    const unsigned char* outend = out + *outlen;
    const unsigned char* outstart = out;
    const unsigned char* instart = in;
    const unsigned char* inend = in + *inlen;
    unsigned int c = 0;
    unsigned int d = 0;
    int trailing = 0;

    while(in < inend){
        d = *in++;
        
        if(d < 0x80){
            c = d; 
            trailing = 0; 
        
        }else if(d < 0xC0){
            //trailing byte in leading position
            *outlen = out - outstart;
            *inlen = processed - instart;
            return false;

        }else if(d < 0xE0){
            c = d & 0x1F; 
            trailing = 1; 
        
        }else if(d < 0xF0){ 
            c = d & 0x0F; 
            trailing = 2;

        }else if(d < 0xF8){ 
            c = d & 0x07; 
            trailing = 3; 
        }else{
            //impossible
            *outlen = out - outstart;
            *inlen = processed - instart;
            return false;
        }

        if(inend - in < trailing){
            break;
        } 

        while(trailing){
            if((in >= inend) || (((d= *in++) & 0xC0) != 0x80)){
                break;
            }
            
            c <<= 6;
            c |= d & 0x3F;
            trailing--;
        }

        if(c < 0x80){
            if(out >= outend){
                break;
            }
            
            *out++ = c;
        }else{
            //impossible
            *outlen = out - outstart;
            *inlen = processed - instart;
            return false;
        }
        processed = in;
    }

    *outlen = out - outstart;
    *inlen = processed - instart;
    return true;
}

bool byteLat1ToUtf8(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen){
    unsigned char* outstart = out;
    const unsigned char* base = in;
    const unsigned char* processed = in;
    unsigned char* outend = out + *outlen;
    const unsigned char* inend = in + *inlen;
    unsigned int c = 0;
    int bits = 0;

    while((in < inend) && (out - outstart + 5 < *outlen)){
        c = *in++;

        if(out >= outend){
            break;
        }
            
        if(c < 0x80){
            *out++ = c;
            bits = -6;

        }else{  
            *out++ = ((c >>  6) & 0x1F) | 0xC0;  
            bits = 0; 
        }
    
        while(bits >= 0){
            if(out >= outend){
                break;
            }
            
            *out++= ((c >> bits) & 0x3F) | 0x80;
            bits -= 6;
        }
        processed = (const unsigned char*) in;
    }
    
    *outlen = out - outstart;
    *inlen = processed - base;
    return true;
}

bool byteUtf8ToLat1(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen){

    if(in == NULL){
        *outlen = 0;
        *inlen = 0;
        return false;
    }

    const unsigned char* processed = in;
    const unsigned char* outend = out + *outlen;
    const unsigned char* outstart = out;
    const unsigned char* instart = in;
    const unsigned char* inend = in + *inlen;
    unsigned int c = 0; 
    unsigned int d = 0;
    int trailing = 0;


    while (in < inend){
        d = *in++;
        
        if(d < 0x80){ 
            c = d;
            trailing = 0; 
        
        //trail is leading
        }else if(d < 0xC0){
            *outlen = out - outstart;
            *inlen = processed - instart;
            return false;
        
        }else if (d < 0xE0){ 
            c = d & 0x1F;
            trailing = 1;
        
        }else if(d < 0xF0){
            c = d & 0x0F;
            trailing= 2;
        
        }else if(d < 0xF8){ 
            c = d & 0x07; 
            trailing = 3;
        
        }else{
            //impossible
            *outlen = out - outstart;
            *inlen = processed - instart;
            return false;
        }

        if(inend - in < trailing){
            break;
        } 

        while(trailing){
            if(in >= inend){
                break;
            }

            if(((d = *in++) & 0xC0) != 0x80){
                *outlen = out - outstart;
                *inlen = processed - instart;
                return false;
            }

            c <<= 6;
            c |= d & 0x3F;
            trailing--;
        }

        if(c <= 0xFF){
            if(out >= outend){
                break;
            }
            
            *out++ = c;
        }else{
            //impossible
            *outlen = out - outstart;
            *inlen = processed - instart;
            return false;
        }
        processed = in;
    }

    *outlen = out - outstart;
    *inlen = processed - instart;
    return true;
}

bool byteUtf16leToUtf8(unsigned char* out, size_t *outlen, const unsigned char* inb, size_t *inlenb){

    unsigned char* outstart = out;
    const unsigned char* processed = inb;
    unsigned char* outend = out + *outlen;
    unsigned short* in = (unsigned short*) inb;
    unsigned short* inend;
    unsigned int c = 0;
    unsigned int d = 0;
    unsigned int inlen = 0;
    unsigned char *tmp = NULL;
    int bits = 0;

    if((*inlenb % 2) == 1){
        (*inlenb)--;
    }

    inlen = *inlenb / 2;
    inend = in + inlen;

    while((in < inend) && (out - outstart + 5 < *outlen)){
        
        if(byteLittleEndian){
            c = *in++;
        }else{
            tmp = (unsigned char *) in;
            c = *tmp++;
            c = c | (((unsigned int)*tmp) << 8);
            in++;
        }

        //surrogates
        if((c & 0xFC00) == 0xD800){
            if(in >= inend){
                break;
            }

            if(byteLittleEndian){
                d = *in++;

            }else{
                tmp = (unsigned char *) in;
                d = *tmp++;
                d = d | (((unsigned int)*tmp) << 8);
                in++;
            }
            
            if((d & 0xFC00) == 0xDC00){
                c &= 0x03FF;
                c <<= 10;
                c |= d & 0x03FF;
                c += 0x10000;
            
            }else{
                *outlen = out - outstart;
                *inlenb = processed - inb;
                return false;
            }
        }

        /* assertion: c is a single UTF-4 value */
        if(out >= outend){
            break;
        }
            
        if(c < 0x80){  
            *out++ =  c;
            bits = -6; 

        }else if(c < 0x800){  
            *out++ = ((c >>  6) & 0x1F) | 0xC0;  
            bits=  0;
       
        }else if(c < 0x10000){  
            *out++ = ((c >> 12) & 0x0F) | 0xE0;  
            bits =  6;
        }else{
            *out++ = ((c >> 18) & 0x07) | 0xF0;  
            bits = 12; 
        }
    
        while(bits >= 0){
            if(out >= outend){
                break;
            }
            
            *out++= ((c >> bits) & 0x3F) | 0x80;
            bits -= 6;
        }
        processed = (const unsigned char*) in;
    }

    *outlen = out - outstart;
    *inlenb = processed - inb;
    return true;
}

bool byteUtf8ToUtf16le(unsigned char* outb, size_t *outlen, const unsigned char* in, size_t *inlen){
    unsigned short* out = (unsigned short*) outb;
    const unsigned char* processed = in;
    unsigned short* outstart = out;
    unsigned short* outend;
    const unsigned char* inend = in + *inlen;
    unsigned int c = 0; 
    unsigned int d = 0;
    int trailing = 0;
    unsigned char *tmp = NULL;
    unsigned short tmp1 = 0;
    unsigned short tmp2 = 0;

    if(in == NULL){
        if(*outlen >= 2){
            outb[0] = 0xFF;
            outb[1] = 0xFE;
            *outlen = 2;
            *inlen = 0;
            return true;
        }

        *outlen = 0;
        *inlen = 0;
        return true;
    }

    outend = out + *outlen / 2;

    while(in < inend){

        d = *in++;
        
        if(d < 0x80){ 
            c = d; 
            trailing = 0; 
        
        }else if(d < 0xC0){
            //trailing byte in leading position
            *outlen = (out - outstart) * 2;
            *inlen = processed - in;
            return false;

        }else if(d < 0xE0){ 
            c = d & 0x1F; 
            trailing= 1; 
        
        }else if(d < 0xF0){ 
            c = d & 0x0F; 
            trailing= 2; 
        
        }else if(d < 0xF8){ 
            c = d & 0x07; 
            trailing = 3; 
        
        }else{
            //not possible
            *outlen = (out - outstart) * 2;
            *inlen = processed - in;
            return false;
        }

        if(inend - in < trailing){
            break;
        } 

        while(trailing){
            if((in >= inend) || (((d= *in++) & 0xC0) != 0x80)){
                break;
            }
            
            c <<= 6;
            c |= d & 0x3F;
            trailing--;
        }

        if(c < 0x10000){
            if(out >= outend){
                break;
            }
                
            if(byteLittleEndian){
                *out++ = c;
            }else{
                tmp = (unsigned char *) out;
                *tmp = c;
                *(tmp + 1) = c >> 8 ;
                out++;
            }
        
        }else if(c < 0x110000){
            if (out+1 >= outend){
                break;
            }
            
            c -= 0x10000;

            if(byteLittleEndian){
                *out++ = 0xD800 | (c >> 10);
                *out++ = 0xDC00 | (c & 0x03FF);
            
            }else{
                tmp1 = 0xD800 | (c >> 10);
                tmp = (unsigned char *) out;
                *tmp = (unsigned char) tmp1;
                *(tmp + 1) = tmp1 >> 8;
                out++;

                tmp2 = 0xDC00 | (c & 0x03FF);
                tmp = (unsigned char *) out;
                *tmp  = (unsigned char) tmp2;
                *(tmp + 1) = tmp2 >> 8;
                out++;
            }
        
        }else{
            break;
        }
        processed = in;
    }

    *outlen = (out - outstart) * 2;
    *inlen = processed - in;
    return true;
}

bool byteUtf16beToUtf8(unsigned char* out, size_t *outlen, const unsigned char* inb, size_t *inlenb){
    unsigned char* outstart = out;
    const unsigned char* processed = inb;
    unsigned char* outend = out + *outlen;
    unsigned short* in = (unsigned short*) inb;
    unsigned short* inend;
    unsigned int c = 0; 
    unsigned int d = 0;
    unsigned int inlen = 0;
    unsigned char *tmp = NULL;
    int bits = 0;

    if((*inlenb % 2) == 1){
        (*inlenb)--;
    }

    inlen = *inlenb / 2;
    inend = in + inlen;

    while(in < inend){
        if(byteLittleEndian){
            tmp = (unsigned char *) in;
            c = *tmp++;
            c = c << 8;
            c = c | (unsigned int) *tmp;
            in++;

        }else{
            c = *in++;
        }

        //surrogates
        if((c & 0xFC00) == 0xD800){    
            //shouldnt happen
            if(in >= inend){
                *outlen = out - outstart;
                *inlenb = processed - inb;
                return false;
            }

            if(byteLittleEndian){
                tmp = (unsigned char *) in;
                d = *tmp++;
                d = d << 8;
                d = d | (unsigned int) *tmp;
                in++;
            
            }else{
                d = *in++;
            }
            
            if((d & 0xFC00) == 0xDC00){
                c &= 0x03FF;
                c <<= 10;
                c |= d & 0x03FF;
                c += 0x10000;
            
            }else {
                *outlen = out - outstart;
                *inlenb = processed - inb;
                return false;
            }
        }

        if(out >= outend){
            break;
        } 

        if(c < 0x80){
            *out++ = c;
            bits= -6;
        
        }else if(c < 0x800){
            *out++ = ((c >>  6) & 0x1F) | 0xC0;  
            bits =  0;
        
        }else if(c < 0x10000){  
            *out++ = ((c >> 12) & 0x0F) | 0xE0;
            bits =  6;
        
        }else{  
            *out++ = ((c >> 18) & 0x07) | 0xF0;
            bits = 12;
        }

        while(bits >= 0){
            if(out >= outend){
                break;
            } 
            
            *out++ = ((c >> bits) & 0x3F) | 0x80;
            bits -= 6;
        }
        processed = (const unsigned char*) in;
    }

    *outlen = out - outstart;
    *inlenb = processed - inb;
    
    return true;
}

bool byteUtf8ToUtf16be(unsigned char* outb, size_t *outlen, const unsigned char* in, size_t *inlen){
    unsigned short* out = (unsigned short*) outb;
    const unsigned char* processed = in;
    unsigned short* outstart = out;
    unsigned short* outend = 0;
    const unsigned char* inend = in + *inlen;
    unsigned int c = 0;
    unsigned int d = 0;
    int trailing = 0;
    unsigned char *tmp = NULL;
    unsigned short tmp1 = 0;
    unsigned short tmp2 = 0;

    if(in == NULL){

        //add bom
        if(*outlen >= 2){
            outb[0] = 0xFE;
            outb[1] = 0xFF;
            *outlen = 2;
            *inlen = 0;
            return true;
        }
        
        *outlen = 0;
        *inlen = 0;
        return true;
    }

    outend = out + (*outlen / 2);
    
    while(in < inend){
        d = *in++;
        
        if(d < 0x80){ 
            c = d;
            trailing = 0;

        }else if(d < 0xC0){
            *outlen = out - outstart;
            *inlen = processed - in;
            return false;

        }else if(d < 0xE0){ 
            c = d & 0x1F; 
            trailing = 1; 
        
        }else if(d < 0xF0){
            c = d & 0x0F; 
            trailing = 2; 
        
        }else if(d < 0xF8){ c = d & 0x07; 
            trailing = 3; 
        
        }else{
            //not possible
            *outlen = out - outstart;
            *inlen = processed - in;
            return false;
        }

        if(inend - in < trailing){
            break;
        } 

        while(trailing){
            if((in >= inend) || (((d= *in++) & 0xC0) != 0x80)){
                break;
            }

            c <<= 6;
            c |= d & 0x3F;
            trailing--;
        }

        if(c < 0x10000){
            if(out >= outend){
                break;
            }
            
            if(byteLittleEndian){
                tmp = (unsigned char *) out;
                *tmp = c >> 8;
                *(tmp + 1) = c;
                out++;
            
            }else{
                *out++ = c;
            }
        
        }else if(c < 0x110000){
            if(out + 1 >= outend){
                break;
            }

            c -= 0x10000;

            if (byteLittleEndian){
                tmp1 = 0xD800 | (c >> 10);
                tmp = (unsigned char *) out;
                *tmp = tmp1 >> 8;
                *(tmp + 1) = (unsigned char) tmp1;
                out++;

                tmp2 = 0xDC00 | (c & 0x03FF);
                tmp = (unsigned char *) out;
                *tmp = tmp2 >> 8;
                *(tmp + 1) = (unsigned char) tmp2;
                out++;
            
            }else{
                *out++ = 0xD800 | (c >> 10);
                *out++ = 0xDC00 | (c & 0x03FF);
            }
        
        }else{
            break;
        }
        
        processed = in;
    }

    *outlen = (out - outstart) * 2;
    *inlen = processed - in;
    return true;
}

