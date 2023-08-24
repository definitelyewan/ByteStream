#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <byteEndian.h>

static void byteSetEndianess_LittleEndian_Setup(void **state){
    (void) state; /* unused */

    assert_true(byteEndianess());
}

static void byteSetEndianess_LittleEndian2_Setup(void **state){
    (void) state; /* unused */

    byteSetEndianess(true);

    assert_true(byteEndianess());
}

static void byteSetEndianess_BigEndian_Setup(void **state){
    (void) state; /* unused */

    byteSetEndianess(false);

    assert_false(byteEndianess());
}

int main(){
    
    const struct CMUnitTest tests[] = {
        //byteSetEndianess tests
        cmocka_unit_test(byteSetEndianess_LittleEndian_Setup),
        cmocka_unit_test(byteSetEndianess_LittleEndian2_Setup),
        cmocka_unit_test(byteSetEndianess_BigEndian_Setup),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}