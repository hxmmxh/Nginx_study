#include "ngy_headers.h"

ngy_uint_t ngy_pagesize=4096;
ngy_uint_t ngy_pagesize_shift;
ngy_uint_t ngy_cacheline_size;

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
    p = ngy_alloc(size);
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
ngy_memalign(size_t alignment, size_t size)
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
