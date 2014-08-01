

void
ofc_bt()
{
    void *trace[128];
    int n = backtrace(trace, sizeof(trace) / sizeof(trace[0]));

    printf("****** Start backtrace ******\n");
    backtrace_symbols_fd(trace, n, 1);
    printf("******* End backtrace *******\n");
    return;
}
