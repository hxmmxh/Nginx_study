#ifndef HXM_NGY_PALLOC_H
#define HXM_NGY_PALLOC_H

#include "ngy_headers.h"





#define NGY_MAX_ALLOC_FROM_POOL  (ngy_pagesize - 1)
#define NGY_DEFAULT_POOL_SIZE    (16 * 1024)
#define NGY_POOL_ALIGNMENT       16

//内存池的最小体积？？
#define NGY_MIN_POOL_SIZE                                                     \
    ngy_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)




typedef struct ngy_pool_s ngy_pool_t;
typedef struct ngy_pool_large_s ngy_pool_large_t;
typedef struct ngy_pool_data_s ngy_pool_data_t;
typedef struct ngy_pool_cleanup_s ngy_pool_cleanup_t;
typedef struct ngy_pool_cleanup_file_s ngy_pool_cleanup_file_t;

//大体积内存数据块
struct ngy_pool_large_s
{
    ngy_pool_large_t *next; //所有大块内存通过next指针连接在一起
    void *alloc;            ////指向ngy_alloc分配出的大块内存
};

//小体积内存的数据块
struct ngy_pool_data_s
{
    u_char *last;      //指向未分配的空闲内存的首地址
    u_char *end;       //指向当前小块内存池的尾部
    ngy_pool_t *next;  //指向下一个小块内存池
    ngy_uint_t failed; //内存分配失败次数，大于4后会修改current指针
};

//函数指针，用于清除操作，data来源ngy_pool_cleanup_t中
typedef void (*ngy_pool_cleanup_pt)(void *data);

//内存回收链表
struct ngy_pool_cleanup_s
{
    ngy_pool_cleanup_pt handler;//用于清除操作的函数
    void *data; //指向要清除的数据
    ngy_pool_cleanup_t *next;//指向下一个内存回收块
};

//内存的分配模块
struct ngy_pool_s
{
    ngy_pool_data_t d;
    size_t max; //评估内存属于大块还是小块的标准
    ngy_pool_t *current;
    ngy_chain_t *chain;//指向一个缓冲区结构，
    ngy_pool_large_t *large;
    ngy_pool_cleanup_t *cleanup;
};


struct ngy_pool_cleanup_file_s
{
    ngy_fd_t fd;
    u_char *name;
};


//共有12个相关的函数
//创建内存池
ngy_pool_t *ngy_create_pool(size_t size);
//销毁内存池，同时清理数据，释放内存
void ngy_destroy_pool(ngy_pool_t *pool);
//重置内存池,归还大块内存，复用小块内存
void ngy_reset_pool(ngy_pool_t *pool);

//分配地址对齐的内存
void* ngy_palloc(ngy_pool_t *pool, size_t size);
//分配时不进行地址对齐
void* ngy_pnalloc(ngy_pool_t *pool, size_t size);
//分配地址对齐的内存并清0
void* ngy_pcalloc(ngy_pool_t *pool, size_t size);
//按照参数alignment进行地址对齐来分配内存，不会使用小块内存，从堆中分配，挂在大块内存的链表中
void* ngy_pmemalign(ngy_pool_t *pool, size_t size, size_t alignment);
//提前释放大块内存,通过返回值判断是否释放成功
ngy_int_t ngy_pfree(ngy_pool_t *pool, void *p);

//添加一个需要在内存池释放时同步释放的资源
ngy_pool_cleanup_t *ngy_pool_cleanup_add(ngy_pool_t *p, size_t size);
//在内存池释放前，提前关闭文件
void ngy_pool_run_cleanup_file(ngy_pool_t *p, ngy_fd_t fd);
//关闭文件释放资源
void ngy_pool_cleanup_file(void *data);
//删除文件释放资源
void ngy_pool_delete_file(void *data);

#endif