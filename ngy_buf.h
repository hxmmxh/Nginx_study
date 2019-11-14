#ifndef HXM_NGY_BUF_H
#define HXM_NGY_BUF_H

#include "ngy_headers.h"

typedef void *            ngy_buf_tag_t;

typedef struct ngy_buf_s  ngy_buf_t;
typedef struct ngy_chain_s  ngy_chain_t;

struct ngy_buf_s
{
    u_char *pos;//从这个位置开始处理内存中的数据
    u_char *last;//有效内容到此为止，pos和last之间的内容是希望处理的内容
    off_t file_pos;//要处理的文件的位置
    off_t file_last;//截至的文件位置

    u_char *start;//当缓冲区用于内存时，指向这段内存的起始位置
    u_char *end;//指向内存的末尾
};

struct ngy_chain_s {
    ngy_buf_t    *buf;
    ngy_chain_t  *next;
};







#endif