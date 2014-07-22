#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>

void
test_malloc(void)
{
    char *ptr;
    int   i;

    ptr = (char *)malloc(7);
    for (i = 0; i < 16; i++){
        ptr[i] = 'a';
    }
    free(ptr);

    ptr = (char *)malloc(3);
    for (i = 0; i < 8; i++){
        ptr[i] = 'b';
    }
    free(ptr);

    ptr = (char *)malloc(24);
    for (i = 0; i < 32; i++){
        ptr[i] = 'a';
    }
    free(ptr);

    ptr = (char *)malloc(24);
    for (i = 0; i > -1; i--){
        ptr[i] = 'd';
    }
    free(ptr);

    return;
}


void
test_realloc(void)
{
    char *ptr;
    int   i;

    ptr = (char *)malloc(7);
    ptr = (char *)realloc(ptr, 8);
    for (i = 0; i < 16; i++){
        ptr[i] = 'a';
    }
    free(ptr);

}

int
main(void)
{
    printf("#### malloc test ####\n");

    test_malloc();

    printf("#### realloc test ####\n");

//    test_realloc();
    return 0;
}
