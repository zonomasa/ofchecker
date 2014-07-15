#define _GNU_SOURCE
#include <features.h>
#include <string.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <malloc.h>
#include <sched.h>

#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
#define _MEMALIGN
#endif

#define REDZONE_SIZE (8 + sizeof(size_t))
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
    void       *ptr, *p;

    while(real_malloc == NULL){
        if(!initializing){
            initializing = 1;
            __init();
            initializing = 0;
        }
        sched_yield();
    }

    ptr = real_malloc(size + REDZONE_SIZE + sizeof(size_t));

    usable = malloc_usable_size(ptr);

/*
  0         1         2         3
  0123456789012345678901234567890123456789
  HHHHHHHHUUUUUUUPPPPPPPPPRRRRRRRRSSSSSSSS
  H:8 (TODO)
  U:
  P:
  R:8
  S:sizeof(size_t)
*/

    p = ptr + size; /* end of user region */

    memset(p, MAGIC_BYTE, (usable - size - 8 - sizeof(size_t)));
    p = ptr + usable - 8 - sizeof(size_t);
    *(size_t *)p = size;


#if 1
    int i;
    for (i = 0; i < (usable - size - 8 - sizeof(size_t)); i++)
        printf("%02x", *(int*)((char *)(ptr + size) + i) & 0x000000ff);
    putchar('\n');
#endif

    return ptr;
}


void
free(void *ptr)
{
    unsigned int cnt = 0u;
    size_t       usable;
    size_t       size;
    void        *p;
    int          i;

    usable = malloc_usable_size(ptr);

    p = ptr + usable - 8 - sizeof(size_t);
    size = *(size_t *)p;

#if 1
    for (i = 0; i < (usable - size - 8 - sizeof(size_t)); i++)
        printf("%02x", *(int*)((char *)(ptr + size) + i) & 0x000000ff);
    putchar('\n');
#endif 
    for (i = 0; i < (usable - size - 8 - sizeof(size_t)); i++) {

        if(*((char *)ptr + size + i) != MAGIC_BYTE)
            cnt++;
    }
#if 1
    printf("overflow count : %du\n", cnt);
#endif

    if(cnt){
        void *trace[128];
//        int n = backtrace(trace, sizeof(trace) / sizeof(trace[0]));
//        backtrace_symbols_fd(trace, n, 1);
    }
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
