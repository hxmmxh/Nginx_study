#include "ngy_headers.h"

ngx_uint_t ngx_pagesize;
ngx_uint_t ngx_pagesize_shift;
ngx_uint_t ngx_cacheline_size;

void *
ngy_alloc(size_t size)
{
    void *p;

    p = malloc(size);
    if (p == NULL)
    {
        log_s("ERROR!!: malloc(%u) failed", size);
    }
    log_s("malloc: %p:%u", p, size);
    return p;
}

void *
ngx_calloc(size_t size)
{
    void *p;
    p = ngx_alloc(size);
    if (p)
    {
        memset(p, 0, size);
    }
    return p;
}

void ngy_free(void *ptr)
{
    free(ptr);
}

void *
ngx_memalign(size_t alignment, size_t size)
{
    void *p;

    p = memalign(alignment, size);
    if (p == NULL)
    {
        log_s("ERROR!!: memalign(%uz, %uz) failed", alignment, size);
    }

    log_s("memalign: %p:%uz @%uz", p, size, alignment);

    return p;
}
