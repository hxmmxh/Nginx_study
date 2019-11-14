#include "ngy_headers.h"

ngy_pool_t *ngy_create_pool(size_t size)
{
    ngy_pool_t *p;
    p = ngy_memalign(NGY_POOL_ALIGNMENT, size);
    if (p == NULL)
        return NULL;
    //下面开始初始化ngy_pool_t的成员
    p->d.last = (u_char *)p + sizeof(ngy_pool_t);
    p->d.end = (u_char *)p + size;
    p->d.next = NULL;
    p->d.failed = 0;
    //实际上可用的内存大小为size-sizeof(ngy_pool_t)
    size = size - sizeof(ngy_pool_t);
    p->max = (size < NGY_MAX_ALLOC_FROM_POOL) ? size : NGY_MAX_ALLOC_FROM_POOL;
    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    return p;
}

void ngy_destory_pool(ngy_pool_t *pool)
{
    ngy_pool_t *p, *n;
    ngy_pool_large_t *l;
    ngy_pool_cleanup_t *c;
    //先回收资源
    for (c = pool->cleanup; c != NULL; c = c->next)
    {
        if (c->handler)
        {
            log_s("run cleanup: %p", c);
            c->handler(c->data);
        }
    }
    //清除大块内存
    for (l = pool->large; l != NULL; l = l->next)
    {
        if (l->alloc)
        {
            log_s("free : %p", l->alloc);
            ngy_free(l->alloc);
        }
    }
    //清除小块内存
    for (p = pool, n = pool->d.next;; p = n, n = n->d.next)
    {
        log_s("free: %p,unused: %u", p, p->d.end - p->d.last);
        ngy_free(p);
        if (n == NULL)
            break;
    }
}

//归还大内存，复用小内存
//资源回收链表没变化
void ngy_reset_pool(ngy_pool_t *pool)
{
    ngy_pool_t *p;
    ngy_pool_large_t *l;
    for (l = pool->large; l != NULL; l = l->next)
    {
        if (l->alloc)
            ngy_free(l->alloc);
    }
    for (p = pool; p != NULL; p = p->d.next)
    {
        p->d.last = (u_char *)p + sizeof(ngy_pool_t);
        p->d.failed = 0;
    }
    pool->current = pool;
    pool->chain = NULL;
    pool->large = NULL;
}

//在函数返回类型前加static，函数就定义为静态函数。函数的定义和声明在默认情况下都是extern的，但静态函数只是在声明他的文件当中可见，不能被其他文件所用。
//不要在头文件中声明static的全局函数，不要在cpp内声明非static的全局函数，如果你要在多个cpp中复用该函数，就把它的声明提到头文件里去，否则cpp内部声明需加上static修饰
static inline void *ngy_palloc_small(ngy_pool_t *pool, size_t size, ngy_uint_t align);
static void *ngy_palloc_large(ngy_pool_t *pool, size_t size);
//用于扩展内存池，申请一个新的内存池节点，挂接在已有内存池的后面
static void *ngy_palloc_block(ngy_pool_t *pool, size_t size);

void *ngy_palloc(ngy_pool_t *pool, size_t size)
{
    if (size <= pool->max)
        return ngy_palloc_small(pool, size, 1);
    else
        return ngy_palloc_large(pool, size);
}

void *ngy_pnalloc(ngy_pool_t *pool, size_t size)
{
    if (size <= pool->max)
        return ngy_palloc_small(pool, size, 0);
    else
        return ngy_palloc_large(pool, size);
}

void *ngy_pcalloc(ngy_pool_t *pool, size_t size)
{
    void *p;
    p = ngy_palloc(pool, size);
    if (p)
        memset(p, 0, size);
    return p;
}

//align指示是否进行内存对齐
static inline void *ngy_palloc_small(ngy_pool_t *pool, size_t size, ngy_uint_t align)
{
    u_char *m;
    ngy_pool_t *p;
    p = pool->current;
    do
    {
        m = p->d.last;
        //如果需要内存对齐，则需要调整m的大小
        if (align)
            m = ngy_align_ptr(m, NGY_ALIGNMENT);
        //空间足够的话，申请成功后返回
        if ((size_t)(p->d.end - m) >= size)
        {
            p->d.last = m + size;
            return m;
        }
        p = p->d.next;
    } while (p);
    //如果遍历完整个内存池都没有找到满足申请大小的内存
    return ngy_palloc_block(pool, size);
}

static void *ngy_palloc_block(ngy_pool_t *pool, size_t size)
{
    u_char *m;
    size_t psize;
    ngy_pool_t *p, *new;
    //求出要分配的空间的大小
    psize = (size_t)(pool->d.end - (u_char *)pool);
    m = ngy_memalign(NGY_POOL_ALIGNMENT, psize);
    if (m == NULL)
        return NULL;
    new = (ngy_pool_t *)m;
    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;
    m += sizeof(ngy_pool_data_t);
    m = ngy_align_ptr(m, NGY_POOL_ALIGNMENT);
    new->d.last = m + size;
    //找到内存池链表的最后一个节点
    for (p = pool->current; p->d.next; p = p->d.next)
    {
        //如果在某一内存池上分配失败的次数过多，则更改current指针
        if (p->d.failed++ > 4)
            pool->current = p->d.next;
    }
    //将新申请的节点挂接在内存池链表的末端
    p->d.next = new;
    return m;
}

static void *ngy_palloc_large(ngy_pool_t *pool, size_t size)
{
    void *p;
    ngy_uint_t n;
    ngy_pool_large_t *large;
    //在进程的堆中分配空间
    p = ngy_alloc(size);
    if (p == NULL)
        return NULL;
    //寻找该内存池大块内存链表中的NULL节点
    //最多寻找三次，如果没找到则直接申请个新的节点，并插入到大块内存链表的开头
    n = 0;
    for (large = pool->large; large; large = large->next)
    {
        if (large->alloc == NULL)
        {
            large->alloc = p;
            return p;
        }
        if (n++ > 3)
            break;
    }
    //在内存池中分配ngy_pool_large_t结构体
    large = ngy_palloc_small(pool, sizeof(ngy_pool_large_t), 1);
    if (large == NULL)
    {
        //分配失败需要先释放刚才在堆上申请的内存
        ngy_free(p);
        return NULL;
    }
    //初始化该结构体,插入到大块内存链表的首部
    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

void *ngy_pmemalign(ngy_pool_t *pool, size_t size, size_t alignment)
{
    void *p;
    //不使用小块内存，从进程的堆中分配，挂在大块内存链表中
    ngy_pool_large_t *large;

    p = ngy_memalign(alignment, size);
    if (p == NULL)
        return NULL;
    //操作同ngy_palloc_large
    large = ngy_palloc_small(pool, sizeof(ngy_pool_large_t), 1);
    if (large == NULL)
    {
        ngy_free(p);
        return NULL;
    }
    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

ngy_int_t ngy_pfree(ngy_pool_t *pool, void *p)
{
    ngy_pool_large_t *l;
    for (l = pool->large; l; l = l->next)
    {
        if (p == l->alloc)
        {
            log_s("free: %p", l->alloc);
            ngy_free(l->alloc);
            l->alloc = NULL;

            return NGY_OK;
        }
    }
    return NGY_DECLINED;
}

ngy_pool_cleanup_t *ngy_pool_cleanup_add(ngy_pool_t *p, size_t size)
{
    ngy_pool_cleanup_t *c;
    //这个结构体的空间在该内存池中分配
    c = ngy_palloc(p, sizeof(ngy_pool_cleanup_t));
    if (c == NULL)
        return NULL;
    //当size不为0时，会分配size大小的内存，将data成员指向该内存，利用其传递参数
    if (size)
    {
        c->data = ngy_palloc(p, size);
        if (c->data == NULL)
            return NULL;
    }
    else
    {
        c->data = NULL;
    }

    c->handler = NULL;
    //插入到资源清理链表的首部
    c->next = p->cleanup;
    p->cleanup = c;
    log_s("add cleanup:%p", c);
    return c;
}

void ngy_pool_run_cleanup_file(ngy_pool_t *p, ngy_fd_t fd)
{
    ngy_pool_cleanup_t *c;
    ngy_pool_cleanup_file_t *cf;

    for (c = p->cleanup; c; c->next)
    {
        //如果清理方法是关闭文件，则关闭文件
        if (c->handler == ngy_pool_cleanup_file)
        {
            cf = c->data;
            if (cf->fd == fd)
            {
                c->handler(cf);
                //清理完把方法置空
                c->handler = NULL;
                return;
            }
        }
    }
}

void ngy_pool_cleanup_file(void *data)
{
    //data在add时就应被设置为ngy_pool_cleanup_file_t 结构
    ngy_pool_cleanup_file_t *c = data;
    log_s("file cleanup:fd:%d %s", c->fd, c->name);
    if (close(c->fd) == -1)
        log_s("file %s close failed", c->name);
}

void ngy_pool_delete_file(void *data)
{
    ngy_pool_cleanup_file_t *c = data;
    log_s("file cleanup:fd:%d %s", c->fd, c->name);
    unlink(c->name);
    close(c->name);
}
