#include <stdio.h>
#include <execinfo.h>


void
ofc_bt()
{
    void *trace[128];
    int n = backtrace(trace, sizeof(trace) / sizeof(trace[0]));

    fprintf(stderr,"<ofchecker> : ***  Start backtrace ***\n");
    backtrace_symbols_fd(trace, n, 2);
    fprintf(stderr,"<ofchecker> : ***   End backtrace  ***\n");
    return;
}
