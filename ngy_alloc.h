#ifndef HXM_NGY_ALLOC_H
#define HXM_NGY_ALLOC_H

#include <malloc.h>
#include "ngy_int.h"

void *ngy_alloc(size_t size);  //分配内存
void *ngx_calloc(size_t size); //分配内存并清0
void ngy_free(void *ptr);      //释放内存

void *ngx_memalign(size_t alignment, size_t size); //以alignment为内存对齐单位分配内存，alignment必须是2的幂

extern ngx_uint_t ngx_pagesize; //内存页的大小
extern ngx_uint_t ngx_pagesize_shift;
extern ngx_uint_t ngx_cacheline_size;

#endif