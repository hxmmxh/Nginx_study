#ifndef HXM_NGY_ALLOC_H
#define HXM_NGY_ALLOC_H

#include <malloc.h>
#include "ngy_int.h"

void *ngy_alloc(size_t size);  //分配内存
void *ngx_calloc(size_t size); //分配内存并清0
void ngy_free(void *ptr);      //释放内存

void *ngy_memalign(size_t alignment, size_t size); //以alignment为内存对齐单位分配内存，alignment必须是2的幂

extern ngy_uint_t ngy_pagesize; //内存页的大小
extern ngy_uint_t ngy_pagesize_shift;
extern ngy_uint_t ngy_cacheline_size;

#endif