#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void log_s(char* fmt,...)
{
    char buf[128];
    va_list vl;
    va_start(vl,fmt);
    vsprintf(buf, fmt, vl);
    va_end(vl);
    printf("%s\n", buf);
}

int main()
{
    log_s("malloc(%uz) failed", 213);
    int *p = malloc(sizeof(int));
    *p = 1;
    log_s("malloc: %p:%uz", p, sizeof(int));
}