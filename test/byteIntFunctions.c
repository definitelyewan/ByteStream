#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <byteInt.h>

static void btoi_convertTo3_Setup(void **state){
    (void) state; /* unused */

    int i = btoi((unsigned char *)"\0\0\0\x03",4);
    assert_int_equal(3,i);
}

static void btoi_convertToFF_Setup(void **state){
    (void) state; /* unused */

    int i = btoi((unsigned char *)"\xff",1);
    assert_int_equal(255,i);
}

static void itob_convertTo0x03_Setup(void **state){
    (void) state; /* unused */

    char *b = itob(3);
    assert_memory_equal(b,"\0\0\0\x03",4);

    free(b);
}

static void itob_convertTo0xFF_Setup(void **state){
    (void) state; /* unused */

    char *b = itob(255);
    assert_memory_equal(b,"\0\0\0\xFF",4);

    free(b);
}

static void itob_convertToINTMAX_Setup(void **state){
    (void) state; /* unused */

    char *b = itob(INT_MAX);
    assert_memory_equal(b,"\x7f\xff\xff\xff",4);

    free(b);
}

static void sttob_7c7354f3(void **state){
    (void) state;
    if(sizeof(size_t) == 8){

        unsigned char *test = sttob(0x7c7354f3);

        assert_non_null(test);
        assert_memory_equal("\x00\x00\x00\x00\x7c\x73\x54\xf3",test, sizeof(size_t));
        free(test);

    }

}

static void sttob_ffffffffffffffff(void **state){
    (void) state;
    if(sizeof(size_t) == 8){

        unsigned char *test = sttob(0xffffffffffffffff);

        assert_non_null(test);
        assert_memory_equal("\xff\xff\xff\xff\xff\xff\xff\xff",test, sizeof(size_t));
        free(test);

    }

}

static void sttob_1(void **state){
    (void) state;
    if(sizeof(size_t) == 8){

        unsigned char *test = sttob(0x01);

        assert_non_null(test);
        assert_memory_equal("\x00\x00\x00\x00\x00\x00\x00\x01",test, sizeof(size_t));
        free(test);

    }
}

static void btost_7c7354f3(void **state){
    (void) state;

    size_t test = btost((unsigned char *)"\x7c\x73\x54\xf3", 4);
    assert_int_equal(test, 0x7c7354f3);
}

static void btost_ffffffffffffffff(void **state){
    (void) state;

    size_t test = btost((unsigned char *)"\xff\xff\xff\xff\xff\xff\xff\xff", 8);
    assert_int_equal(test, 0xffffffffffffffff);
}

static void btost_1(void **state){
    (void) state;

    size_t test = btost((unsigned char *)"\x01", 1);
    assert_int_equal(test, 0x01);
}

static void u32tob_ffffffff(void **state){
    (void) state;



    unsigned char *test = u32tob(0xffffffff);

    assert_non_null(test);
    assert_memory_equal("\xff\xff\xff\xff",test, sizeof(uint32_t));
    free(test);

    
}

static void setBit_allBits_setup(void **state){
    (void) state; /* unused */

    uint8_t bits = 0;

    for(int i = 0; i < 8; i++){
        bits = setBit(bits, i, 1);
    }

    assert_int_equal(bits,255);
}

static void setBit_unsetAllBits_setup(void **state){
    (void) state; /* unused */

    uint8_t bits = 255;

    for(int i = 0; i < 8; i++){
        bits = setBit(bits, i, 0);
    }

    assert_int_equal(bits,0);
}

static void setBit_setOutOfRange_setup(void **state){
    (void) state; /* unused */

    uint8_t bits = 0;
    bits = setBit(bits, 100, 1);
    assert_int_equal(bits,0);
}

static void readBit_readall_setup(void **state){
    (void) state; /* unused */

    uint8_t bits = 0;
    
    for(int i = 0; i < 8; i++){
        int v = readBit(bits, i);
        assert_int_equal(v, 0);
    }

    bits = 255;
    
    for(int i = 0; i < 8; i++){
        int v = readBit(bits, i);
        assert_int_equal(v, 1);
    }
}

static void readBit_outOfRange_setup(void **state){
    (void) state; /* unused */

    uint8_t bits = 255;
    
    int v = readBit(bits, 100);
    assert_int_equal(v,-1);
}

static void byteSyncintDecode_singleByteVal_Setup(void **state){
    (void) state; /* unused */
    assert_int_equal(10,byteSyncintDecode(10));
}

static void byteSyncintDecode_doubleByteVal_Setup(void **state){
    (void) state; /* unused */
    assert_int_equal(255,byteSyncintDecode(383));
}

static void byteSyncintDecode_tripleByteVal_Setup(void **state){
    (void) state; /* unused */
    assert_int_equal(5000,byteSyncintDecode(9992));
}

static void byteSyncintEncode_singleByteVal_Setup(void **state){
    (void) state; /* unused */
    assert_int_equal(10,byteSyncintEncode(10));
}

static void byteSyncintEncode_doubleByteVal_Setup(void **state){
    (void) state; /* unused */
    assert_int_equal(383,byteSyncintEncode(255));
}

static void byteSyncintEncode_tripleByteVal_Setup(void **state){
    (void) state; /* unused */
    assert_int_equal(9992,byteSyncintEncode(5000));
}

int main(){
    
    const struct CMUnitTest tests[] = {
        //btoi
        cmocka_unit_test(btoi_convertTo3_Setup),
        cmocka_unit_test(btoi_convertToFF_Setup),
        //itob
        cmocka_unit_test(itob_convertTo0x03_Setup),
        cmocka_unit_test(itob_convertTo0xFF_Setup),
        cmocka_unit_test(itob_convertToINTMAX_Setup),

        //sttob
        cmocka_unit_test(sttob_7c7354f3),
        cmocka_unit_test(sttob_ffffffffffffffff),
        cmocka_unit_test(sttob_1),

        //btost
        cmocka_unit_test(btost_7c7354f3),
        cmocka_unit_test(btost_ffffffffffffffff),
        cmocka_unit_test(btost_1),

        // u32tob
        cmocka_unit_test(u32tob_ffffffff),

        //setBit
        cmocka_unit_test(setBit_allBits_setup),
        cmocka_unit_test(setBit_unsetAllBits_setup),
        cmocka_unit_test(setBit_setOutOfRange_setup),

        //readBit
        cmocka_unit_test(readBit_readall_setup),
        cmocka_unit_test(readBit_outOfRange_setup),

        //byteSyncintDecode
        cmocka_unit_test(byteSyncintDecode_singleByteVal_Setup),
        cmocka_unit_test(byteSyncintDecode_doubleByteVal_Setup),
        cmocka_unit_test(byteSyncintDecode_tripleByteVal_Setup),
        //byteSyncintEncode
        cmocka_unit_test(byteSyncintEncode_singleByteVal_Setup),
        cmocka_unit_test(byteSyncintEncode_doubleByteVal_Setup),
        cmocka_unit_test(byteSyncintEncode_tripleByteVal_Setup),


    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
