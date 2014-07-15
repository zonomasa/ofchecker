#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>


void
overflow(void) {
    char *ptr;
    int   i;

//    mcheck(MCHECK_TAIL);

    /* Can not detect */
    ptr = (char *)malloc(3);
    for (i = 0; i < 8; i++){
        ptr[i] = 'a';
    }
//    mcheck_check_all();
    free(ptr);
    putchar('\n');
    /* OK */
/* <jemalloc>: Corrupt redzone 0 bytes after 0x2b0f58f5a100 (size 8), byte=0x32 */
    ptr = (char *)malloc(3);
    for (i = 0; i < 8; i++){
        ptr[i] = 'b';
    }
//    mcheck_check_all();
    free(ptr);

    /* OK */
/*
<jemalloc>: Corrupt redzone 0 bytes after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 1 byte after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 2 bytes after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 3 bytes after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 4 bytes after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 5 bytes after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 6 bytes after 0x2b387c731180 (size 32), byte=0x61
<jemalloc>: Corrupt redzone 7 bytes after 0x2b387c731180 (size 32), byte=0x61
*/
    ptr = (char *)malloc(24);
    for (i = 0; i < 25; i++){
        ptr[i] = 'a';
    }
//    mcheck_check_all();
    free(ptr);

    /* OK */
/*
<jemalloc>: Corrupt redzone 1 byte before 0x2b0908c0f081 (size 32), byte=0x64
<jemalloc>: Corrupt redzone 2 bytes before 0x2b0908c0f081 (size 32), byte=0x64
<jemalloc>: Corrupt redzone 3 bytes before 0x2b0908c0f081 (size 32), byte=0x64
<jemalloc>: Corrupt redzone 4 bytes before 0x2b0908c0f081 (size 32), byte=0x64
<jemalloc>: Corrupt redzone 5 bytes before 0x2b0908c0f081 (size 32), byte=0x64
<jemalloc>: Corrupt redzone 6 bytes before 0x2b0908c0f081 (size 32), byte=0x64
<jemalloc>: Corrupt redzone 7 bytes before 0x2b0908c0f081 (size 32), byte=0x64
*/

    ptr = (char *)malloc(24);
    for (i = 0; i > -1; i--){
        ptr[i] = 'd';
    }
//    mcheck_check_all();
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
