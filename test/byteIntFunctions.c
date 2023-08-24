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

    int i = btoi("\0\0\0\x03",4);
    assert_int_equal(3,i);
}

static void btoi_convertToFF_Setup(void **state){
    (void) state; /* unused */

    int i = btoi("\xff",1);
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