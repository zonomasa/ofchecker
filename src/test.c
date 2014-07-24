#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

    ptr = (char *)malloc(0);
    free(ptr);

    


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
    for (i = 0; i > -1; i--){
        ptr[i] = 'd';
    }
    free(ptr);

}


void
test_calloc(void)
{
    char *ptr;
    int   i;

    ptr = (char *)calloc(6,144);
    for (i = 0; i < 6*144; i++)
        ptr[i] = 'b';

    free(ptr);

    return;
}

int
main(void)
{
    printf("#### malloc test ####\n");

    test_malloc();

    printf("#### realloc test ####\n");

    test_realloc();

    printf("#### calloc test ####\n");

    test_calloc();

    printf("#### free test ####\n");

    test_free();


    fflush(stdout);
    printf("######################");
    printf("#### End of Tests ####");
    printf("######################");
    return 0;
}
