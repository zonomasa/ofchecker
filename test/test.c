#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

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
        ptr[i] = 'z';
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
    assert(malloc_usable_size(ptr)  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] realloc_12 ...\n");
    /* If size is 0, realloc() acts like free(). */
    /* So it should detect overflow */
    ptr = (char *)malloc(3);
    for (i = 0; i < 8; i++){
        ptr[i] = 'z';
    }
    ptr = (char *)realloc(ptr, 0);
    assert(ofc_getCount()  == 5);
    TESTLOG("passed\n");

    return;
}


void
test_calloc(void)
{
    char *ptr;
    int   i;

    TESTLOG("[TEST] calloc_01 ...\n");
    ptr = (char *)calloc(1,8);
    for (i = 0; i < 16; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_02 ...\n");
    ptr = (char *)calloc(2,4);
    for (i = 0; i < 16; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");


    TESTLOG("[TEST] calloc_03 ...\n");
    ptr = (char *)calloc(1,5);
    for (i = 0; i < 16; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 11);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_03 ...\n");
    ptr = (char *)calloc(5,1);
    for (i = 0; i < 16; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_04 ...\n");
    ptr = (char *)calloc(1,1024);
    for (i = 0; i < 1032; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_05 ...\n");
    ptr = (char *)calloc(1,256 * 1024);
    for (i = 0; i < (256 * 1024) + 8; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_06 ...\n");
    ptr = (char *)calloc(1,8);
    for (i = 0; i < 8; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_07 ...\n");
    ptr = (char *)calloc(1,1024);
    for (i = 0; i < 1024; i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_08 ...\n");
    ptr = (char *)calloc(1,256 * 1024);
    for (i = 0; i < (256 * 1024); i++)
        ptr[i] = 'z';
    free(ptr);
    assert(ofc_getCount()  == 0);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_09 ...\n");
    ptr = (char *)calloc(6,0);
    for (i = 0; i < 8; i++)
        ptr[i] = 'b';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_10 ...\n");
    ptr = (char *)calloc(0,6);
    for (i = 0; i < 8; i++)
        ptr[i] = 'b';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    TESTLOG("[TEST] calloc_10 ...\n");
    ptr = (char *)calloc(0,0);
    for (i = 0; i < 8; i++)
        ptr[i] = 'b';
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");

    return;
}


void
test_posix_memalign(void)
{
    char *ptr;
    int   i;
    

    TESTLOG("#### posix_memalign ####\n");

    TESTLOG("[TEST] posix_memalign_01 ...\n");
    assert(posix_memalign((void**)&ptr, 64, 128) == 0);
    for (i = 0; i < 129; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 1);
    TESTLOG("passed\n");

    TESTLOG("[TEST] posix_memalign_02 ...\n");
    assert(posix_memalign((void**)&ptr, 64, 1024) == 0);
    for (i = 0; i < 1032; i++){
        ptr[i] = 'z';
    }
    free(ptr);
    assert(ofc_getCount()  == 8);
    TESTLOG("passed\n");
}

int
main(void)
{
    test_malloc();
    test_realloc();
    test_calloc();
    test_free();
    test_posix_memalign();
    fflush(stdout);
    TESTLOG("######################\n");
    TESTLOG("#### End of Tests ####\n");
    TESTLOG("######################\n");
    return 0;
}
