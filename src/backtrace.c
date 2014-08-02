#include <stdio.h>

void
ofc_bt()
{
    void *trace[128];
    int n = backtrace(trace, sizeof(trace) / sizeof(trace[0]));

    fprintf(stderr,"****** Start backtrace ******\n");
    backtrace_symbols_fd(trace, n, 1);
    fprintf(stderr,"******* End backtrace *******\n");
    return;
}
