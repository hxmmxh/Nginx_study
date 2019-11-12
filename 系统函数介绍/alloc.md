# 动态内存分配函数接口介绍



```c
#include <stdlib.h>
#include <malloc.h>
//两个头文件选其中一个
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
```


* malloc()在运行期动态分配分配内存,free()释放由其分配的内存。malloc()在分配用户传入的大小的时候，还分配的一个相关的用于管理的额外内存，不过，用户是看不到的。所以，实际的大小 = 管理空间 + 用户空间



* malloc或realloc返回的内存块地址都是8的倍数（如果是64位系统，则为16的倍数）。如果需要更大的粒度，则需要使用memalign或valloc。这些函数在头文件“stdlib.h”中声明。
* 在GNU库中，可以使用函数free释放memalign和valloc返回的内存块
```c
void * memalign (size_t boundary, size_t size) 
//函数memalign将分配一个由size指定大小，地址是boundary的倍数的内存块。参数boundary必须是2的幂！函数memalign可以分配较大的内存块，并且可以为返回的地址指定粒度。
void * valloc (size_t size) 
//使用函数valloc与使用函数memalign类似，函数valloc的内部实现里，使用页的大小作为对齐长度，使用memalign来分配内存。它的实现如下所示： 
void *valloc (size_t size)
{
    return memalign (getpagesize (), size);
}
```