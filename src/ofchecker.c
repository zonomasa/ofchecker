#define _GNU_SOURCE
#include <features.h>
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <malloc.h>
#include <sched.h>

#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
#define _MEMALIGN
#endif

/**
  Example of 7byte malloc with ofchecker.
    0         1         2         3
    0123456789012345678901234567890123456789
    HHHHHHHHUUUUUUUPPPPPPPPPRRRRRRRRSSSSSSSS

  H: Glibc malloc header
  U: User payload
  P: Padding for round-up size, which should be filled by MAGIC_BYTE
  R: Fixed Redzone for detecting overflow, which should be filled by MAGIC_BYTE too
  S: Size which is requested by user, for calcurating 'P' size in free()

   - RZ means Redzone, filled by MAGIC_BYTE
*/

#define SIZEOF_RZ(u,s) (u - s - SIZEOF_SIZE)  /* 'P' + 'R' */
#define SIZEOF_F_RZ    (8U)                   /* 'R' */
#define SIZEOF_SIZE    sizeof(size_t)         /* 'S' */
#define P_RZ(p,u,s) (p + s)                             /* pointer of 'P' */
#define P_F_RZ(p,u) (p + u - SIZEOF_SIZE - SIZEOF_F_RZ) /* pointer of 'R' */
#define P_SIZE(p,u) (p + u - SIZEOF_SIZE)               /* pointer of 'R' */

#define MAGIC_BYTE 0x5a

#ifdef OFC_DBG
#define OFC_DUMP(p,u,s) do{                                             \
    int i;                                                              \
    for (i = 0; i < SIZEOF_RZ(u, s); i++)                               \
        printf("%02x", *(int*)((char *)(p + s) + i) & 0x000000ff);      \
    putchar('\n');                                                      \
    fflush(stdout);                                                     \
    }while(0)
#else
#define OFC_DUMP(p,u,s)
#endif
#define OFC_DUMP_COUNT(c)  fprintf(stderr, "<ofchecker> : Corrupted memory after payload : %zu byte\n", c);
#define OFC_DUMP_COUNT_MAYBE(c)  fprintf(stderr, "<ofchecker> : Corrupted memory after payload : %zu byte and more\n", c);
#define OFC_DUMP_INFO(p,s) fprintf(stderr, "<ofchecker> : Address : %p\n<ofchecker> : Size : %zu\n", p,s);
#define OFC_UNUSE(u) (u = u)

static void *(* real_malloc)(size_t size);
static void  (* real_free)(void *ptr);
static void *(* real_realloc)(void *ptr, size_t size);
static void *(* real_calloc)(size_t nmemb, size_t size);
#ifdef _MEMALIGN
static int   (* real_posix_memalign)(void **memptr, size_t alignment, size_t size);
#endif
extern void ofc_bt(void);

static int  initializing = 0;
static int  ofc_count = 0;    /* for testing */


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

    ptr = real_malloc(size + SIZEOF_F_RZ + SIZEOF_SIZE);
    usable = malloc_usable_size(ptr);

    p = ptr + size; /* end of user region */
    memset(p, MAGIC_BYTE, SIZEOF_RZ(usable, size));

    p += SIZEOF_RZ(usable,size); /* end of redzone */
    *(size_t *)p = size;

    OFC_DUMP(ptr, usable, size);

    return ptr;
}


void
free(void *ptr)
{
    size_t        usable,size,cnt = 0u;
    unsigned int  i;
    char         *p;

    /* If ptr is NULL, no operation is performed.*/
    if (ptr == NULL){
        return real_free(ptr);
    }
    usable = malloc_usable_size(ptr);

    /* At the first, check fixed redzone. If overwritten, following size
       info is maybe invalid */
    for (p = (char *)P_F_RZ(ptr, usable); p < (char *)P_F_RZ(ptr, usable) + SIZEOF_F_RZ; p++){
        if(*p != MAGIC_BYTE)
            cnt++;
    }
    if (cnt == SIZEOF_F_RZ){
        ofc_count = cnt; /* for testing */
        /* Maybe size info was broken */
        OFC_DUMP_COUNT_MAYBE(cnt);
        ofc_bt();
        real_free(ptr);
        return;
    }
    size = *(size_t *)P_SIZE(ptr, usable);
    OFC_DUMP(ptr, usable, size);
    p = P_RZ(ptr, usable, size);
    for (i = 0; i < SIZEOF_RZ(usable ,size) - SIZEOF_F_RZ; i++) {
        if(*(p + i) != MAGIC_BYTE)
            cnt++;
    }
    if (cnt){
        ofc_count = cnt; /* for testing */
        OFC_DUMP_COUNT(cnt);
        OFC_DUMP_INFO(ptr,size);
        ofc_bt();
    }
    real_free(ptr);
    return;
}


void *
realloc(void *ptr, size_t size)
{
    size_t   usable;
    void    *p;

    while (real_realloc == NULL){
        if (!initializing){
            initializing = 1;
            __init();
            initializing = 0;
        }
        sched_yield();
    }

    if (size == 0){
        /* size=0,ptr is not NULL => free(ptr); */
        free(ptr);
        return NULL;
    }

    ptr = real_realloc(ptr, size + SIZEOF_F_RZ + SIZEOF_SIZE);
    usable = malloc_usable_size(ptr);
    /* corner cases */

    /* ptr is NULL => malloc(size); */
    p = ptr + size; /* end of user region */
    memset(p, MAGIC_BYTE, SIZEOF_RZ(usable, size));
    p += SIZEOF_RZ(usable,size); /* end of redzone */
    *(size_t *)p = size;
    OFC_DUMP(ptr, usable, size);
    return ptr;
}


void *
calloc(size_t nmemb, size_t size)
{
    size_t   newNmemb,newSize,usable;
    void    *ptr, *p;

    while(real_calloc == NULL){
        if(!initializing){
            initializing = 1;
            __init();
            initializing = 0;
        }
        sched_yield();
    }

    if ((size * nmemb) == 0){
        /* corner cases */
        /* When size=0 or nmemb=0, */
        /* malloc() maybe return minimum block, */
        newSize = SIZEOF_F_RZ + SIZEOF_SIZE;
        newNmemb = 1;
    }else{
        newSize = size;
        newNmemb = nmemb + ((SIZEOF_F_RZ + SIZEOF_SIZE - 1) / size + 1);
    }
    ptr = real_calloc(newNmemb ,newSize);
    usable = malloc_usable_size(ptr);
    p = ((char*)ptr + (size * nmemb)); /* end of user region */
    memset(p, MAGIC_BYTE, SIZEOF_RZ(usable, size * nmemb));
    p += SIZEOF_RZ(usable,size * nmemb); /* end of redzone */
    *(size_t *)p = size * nmemb;
    OFC_DUMP(ptr, usable, size * nmemb);
    return ptr;
}


int
posix_memalign(void **memptr, size_t alignment, size_t size)
{
    size_t      usable;
    void       *p;
    int         ret;

    while(real_posix_memalign == NULL){
        if(!initializing){
            initializing = 1;
            __init();
            initializing = 0;
        }
        sched_yield();
    }


    ret = real_posix_memalign(memptr, alignment,
                              size + SIZEOF_F_RZ + SIZEOF_SIZE);
    if (ret != 0)
        return ret;

    usable = malloc_usable_size(*memptr);
    p = *memptr + size; /* end of user region */
    memset(p, MAGIC_BYTE, SIZEOF_RZ(usable, size));
    p += SIZEOF_RZ(usable,size); /* end of redzone */
    *(size_t *)p = size;
    OFC_DUMP(*memptr, usable, size);
    return 0;
}


/**
 * Return overflowed byte in last free().
 * - Thread unsafe
 * - For testging.
 */
unsigned int
ofc_getCount()
{
    unsigned int ret;
    ret = ofc_count;
    ofc_count = 0;
    return ret;
}
