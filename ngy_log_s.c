#include "ngy_log_s.h"

void log_s(char* fmt,...)
{
    char buf[128];
    va_list vl;
    va_start(vl,fmt);
    vsprintf(buf, fmt, vl);
    va_end(vl);
    printf("%s\n", buf);
}