#include "ngy_headers.h"

int main()
{
    log_s("hello world");
    log_s("hello %d", 1);
    log_s("hello %d, %c", 1, 'c');
    log_s("hello %d, %s", 1, "world");
}