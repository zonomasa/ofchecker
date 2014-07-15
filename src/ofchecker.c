#define _GNU_SOURCE
#include <features.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <malloc.h>
#include <sched.h>

#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
#define _MEMALIGN
#endif

#define REDZONE_SIZE 8
#define MAGIC_BYTE 0x5a

static void *(* real_malloc)(size_t size);
static void  (* real_free)(void *ptr);
static void *(* real_realloc)(void *ptr, size_t size);
static void *(* real_calloc)(void *ptr, size_t size);
#ifdef _MEMALIGN
static int   (* real_posix_memalign)(void *ptr, size_t size);
#endif


static void
__init(void)
{
    real_malloc  = dlsym(RTLD_NEXT, "malloc");
    real_free    = dlsym(RTLD_NEXT, "free");
    real_realloc = dlsym(RTLD_NEXT, "realloc");
    real_calloc  = dlsym(RTLD_NEXT, "calloc");
#ifdef _MEMALIGN
    real_posix_memalign = dlsym(RTLD_NEXT, "posix_memalign");
#endif

}


void *
malloc(size_t size)
{
    static int  initializing = 0;
    size_t      usable;
    void       *ret;

    while(real_malloc == NULL){
        if(!initializing){
            initializing = 1;
            __init();
            initializing = 0;
        }
        sched_yield();
    }

    size += REDZONE_SIZE;
    ret = real_malloc(size);

    usable = malloc_usable_size(ret);
    memset(ret + (usable - REDZONE_SIZE), MAGIC_BYTE, REDZONE_SIZE);


#if 1
    int i;
    for (i = 0; i < REDZONE_SIZE; i++){
        printf("%02x", *(int*)((char *)(ret + (usable - REDZONE_SIZE))+i) & 0x000000ff);
    }
    putchar('\n');
#endif

    return ret;
}
/*
012345678901
******PPRRRR
*/

void
free(void *ptr)
{
    size_t      usable;

//    usable = malloc_usable_size(ret);
}


void *
realloc(void *ptr, size_t size)
{
}


void *
calloc(size_t nmemb, size_t size)
{
}


int
posix_memalign(void **memptr, size_t alignment, size_t size)
{
}
