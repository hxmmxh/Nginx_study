#ifndef HXM_NGY_HEADERS_H
#define HXM_NGY_HEADERS_H

#include <string.h>

#include "ngy_alloc.h"
#include "ngy_buf.h"
#include "ngy_int.h"
#include "ngy_log_s.h"
#include "ngy_palloc.h"

typedef unsigned char u_char;
typedef int ngy_fd_t;

#define NGY_OK 0
#define NGY_DECLINED -5//拒绝

//默认的对齐大小Y
#define NGY_ALIGNMENT   sizeof(unsigned long) 
//对齐宏，把d对齐成a的倍数
#define ngy_align(d, a) (((d) + (a - 1)) & ~(a - 1))
//对齐指针
#define ngy_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#endif