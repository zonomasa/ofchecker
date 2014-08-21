#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TESTLOG(s) do{                           \
        fprintf(stdout,s);                       \
        fflush(stdout);                          \
    }while(0)

extern unsigned int ofc_getCount();

void
test_malloc(void)
{
    char *ptr;
    int   i;

    TESTLOG("#### malloc ####\n");

    TESTLOG("[TEST] malloc_01 ...\n");
    ptr = (char *)malloc(8);
    for (i = 0; i < 16; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_02 ...\n");
    ptr = (char *)malloc(1024);
    for (i = 0; i < 1032; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_03 ...\n");
    ptr = (char *)malloc(256 * 1024);
    for (i = 0; i < (256 * 1024) + 8; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_04 ...\n");
    ptr = (char *)malloc(11);
    for (i = 0; i < 16; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 5);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_05 ...\n");
    ptr = (char *)malloc(11);
    for (i = 0; i < 20; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 9);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_06 ...\n");
    ptr = (char *)malloc(0);
    if (ptr != NULL){
        for (i = 0; i < 8; i++){
            ptr[i] = 'z';
        }
    }
    free(ptr);
    if (ptr != NULL){
        assert(ofc_getCount()  == 8);
    }
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_07 ...\n");
    /* over fixed-redzone and break size info */
    ptr = (char *)malloc(24);
    for (i = 0; i < 32; i++){
        ptr[i] = 'a';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    /* No overflow */
    TESTLOG("[TEST] malloc_08 ...\n");
    ptr = (char *)malloc(8);
    for (i = 0; i < 8; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_09 ...\n");
    ptr = (char *)malloc(1024);
    for (i = 0; i < 1024; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] malloc_10 ...\n");
    ptr = (char *)malloc(256 * 1024);
    for (i = 0; i < (256 * 1024); i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    return;
}


void
test_free(void)
{

    void *ptr;
    printf("hoge\n");
    ptr = NULL;
    free(ptr);

    return ;
}

void
test_realloc(void)
{
    char *ptr;
    int   i;

    TESTLOG("#### realloc ####\n");

    TESTLOG("[TEST] realloc_01 ...\n");
    ptr = (char *)malloc(3);
    ptr = (char *)realloc(ptr, 8);
    for (i = 0; i < 16; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_02 ...\n");
    ptr = (char *)malloc(256);
    ptr = (char *)realloc(ptr, 1024);
    for (i = 0; i < 1032; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_03 ...\n");
    ptr = (char *)malloc(128 * 1024);
    ptr = (char *)realloc(ptr, 256 * 1024);
    for (i = 0; i < (256 * 1024) + 8; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_04 ...\n");
    ptr = (char *)malloc(8);
    ptr = (char *)realloc(ptr, 11);
    for (i = 0; i < 16; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 5);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_05 ...\n");
    ptr = (char *)malloc(32);
    ptr = (char *)realloc(ptr, 11);
    for (i = 0; i < 20; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 9);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_06 ...\n");
    ptr = (char *)malloc(8);
    ptr = (char *)realloc(ptr, 0);
    if (ptr != NULL){
        for (i = 0; i < 8; i++){
            ptr[i] = 'z';
        }
    }
    free(ptr);
    if (ptr != NULL){
        assert(ofc_getCount()  == 8);
    }
    TESTLOG("passed\n");


    TESTLOG("[TEST] realloc_07 ...\n");
    /* over fixed-redzone and break size info */
    ptr = (char *)malloc(108);
    ptr = (char *)realloc(ptr, 24);
    for (i = 0; i < 32; i++){
        ptr[i] = 'a';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");


    /* No overflow */
    TESTLOG("[TEST] realloc_08 ...\n");
    ptr = (char *)malloc(1);
    ptr = (char *)realloc(ptr, 8);
    for (i = 0; i < 8; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_09 ...\n");
    ptr = (char *)malloc(16);
    ptr = (char *)realloc(ptr, 1024);
    for (i = 0; i < 1024; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_10 ...\n");
    ptr = (char *)malloc(1024);
    ptr = (char *)realloc(ptr, 256 * 1024);
    for (i = 0; i < (256 * 1024); i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_11 ...\n");
    ptr = (char *)realloc(NULL, 0);
    for (i = 0; i > 3; i--){
        ptr[i] = 'd';
    }
    free(ptr);
    assert(ofc_getCount()  == 3);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_12 ...\n");
    /* corner case :size = 0 */
    ptr = (char *)malloc(3);
    for (i = 0; i > 8; i--){
        ptr[i] = 'z';
    }
    ptr = (char *)realloc(ptr, 0);
    assert(ofc_getCount()  == 5);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_07 ...\n");
    ptr = (char *)malloc(3);
    ptr = (char *)realloc(ptr, 0);
    if (ptr != NULL){
        for (i = 0; i < 8; i++){
            ptr[i] = 'z';
        }
    }
    free(ptr);
    if (ptr != NULL){
        assert(ofc_getCount()  == 8);
    }
    TESTLOG("passed\n");

    assert(0);























    ptr = (char *)malloc(7);
    ptr = (char *)realloc(ptr, 8);
    for (i = 0; i < 16; i++){
        ptr[i] = 'a';
    }
    free(ptr);

    ptr = (char *)malloc(3);
    ptr = (char *)realloc(ptr, 4);
    for (i = 0; i < 8; i++){
        ptr[i] = 'b';
    }
    free(ptr);

    ptr = (char *)malloc(48);
    ptr = (char *)realloc(ptr, 24);
    for (i = 0; i < 32; i++){
        ptr[i] = 'a';
    }
    free(ptr);

    ptr = (char *)malloc(3);
    ptr = (char *)realloc(ptr, 4);
    for (i = 0; i > 8; i--){
        ptr[i] = 'd';
    }
    free(ptr);

    free(ptr);

//    assert (ptr != NULL);

}


void
test_calloc(void)
{
    char *ptr;
    int   i;

    ptr = (char *)calloc(0,144);
    for (i = 0; i < 8; i++)
        ptr[i] = 'b';
    free(ptr);

    ptr = (char *)calloc(6,0);
    for (i = 0; i < 8; i++)
        ptr[i] = 'b';
    free(ptr);

    ptr = (char *)calloc(6,144);
    for (i = 0; i < 6*144; i++)
        ptr[i] = 'b';

    free(ptr);

    return;
}

int
main(void)
{
    test_malloc();
    test_realloc();
    test_calloc();
    test_free();
    fflush(stdout);
    printf("######################");
    printf("#### End of Tests ####");
    printf("######################");
    return 0;
}
