#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>




void
overflow(void) {
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


int
main(void)
{

    printf("#### Overflow test ####\n");
    overflow();

    return 0;
}
