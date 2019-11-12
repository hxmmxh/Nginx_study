#ifndef HXM_NGY_HEADERS_H
#define HXM_NGY_HEADERS_H

#include <string.h>

#include "ngy_int.h"
#include "ngy_log_s.h"
#include "ngy_alloc.h"
#include "ngy_palloc.h"






//对齐宏，把d对齐成a的倍数
#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))

#endif