#include "ngy_headers.h"

//空链表prev和next成员都指向自己
//不为空时next指向第一个元素，prev指向最后一个元素
void ngy_queue_init(ngy_queue_t *p)
{
    p->prev = p;
    p->next = p;
}
//检测是否为空
ngy_uint_t ngy_queue_empty(ngy_queue_t *p)
{
    return p == p->prev ? 1 : 0;
}
//将元素x插入到容器h的头部
void ngy_queue_insert_head(ngy_queue_t *h, ngy_queue_t *x)
{
    x->next = h->next;
    x->next->prev = x;
    h->next = x;
    x->prev = h;
}

void ngy_queue_insert_tail(ngy_queue_t *h, ngy_queue_t *x)
{
    x->next = h;
    h->prev->next = x;
    x->prev = h->prev;
    h->prev = x;
}

ngy_queue_t *ngy_queue_head(ngy_queue_t *h)
{
    return h->next;
}

ngy_queue_t *ngy_queue_last(ngy_queue_t *h);
{
    return h->prev;
}

ngy_queue_t *ngy_queue_sentinel(ngy_queue_t *h)
{
    return h;
}

void ngy_queue_remove(ngy_queue_t *x)
{
    x->prev->next = x->next;
    x->next->prev = x->prev;
    x->prev = NULL;
    x->next = NULL;
}

void ngy_queue_split(ngy_queue_t *h, ngy_queue_t *q, ngy_queue_t *n)
{
    n->prev = h->prev;
    n->prev->next = n;
    n->next = q;
    h->prev = q->prev;
    h->prev->next = h;
    q->prev = n;
}

void ngy_queue_add(ngy_queue_t *h, ngy_queue_t *n)
{
    h->prev->next = n->next;
    n->next->prev = h->prev;
    h->prev = n->prev;
    h->prev->next = h;
}

//用快慢指针的方法找中间节点
ngy_queue_t *ngy_queue_middle(ngy_queue_t *h)
{
    ngy_queue_t *middle, *quick;
    middle = ngy_queue_head(h);
    if (middle = ngy_queue_last(h))
        return middle;
    quick = ngy_queue_head(h);
    while (1)
    {
        middle = ngy_queue_next(middle);
        quick = ngy_queue_next(quick);
        if (quick = ngy_queue_last(q))
            return middle;
        quick = ngy_queue_next(quick);
        if (quick = ngy_queue_last(q))
            return middle;
    }
}

//采用稳定的插入排序
void ngy_queue_sort(ngy_queue_t *h, ngy_int_t (*cmp)(const ngy_queue_t *, const ngy_queue_t *))
{
    ngy_queue_t *q, *prev, *next;
    q = ngy_queue_head(h);
    if (q == ngy_queue_last(h))
        return;
    for (q = ngy_queue_next(q); q != ngy_queue_sentinel(h); q = next)
    {
        prev = ngy_queue_prev(q);
        next = ngy_queue_next(q);
        //先把该节点移除
        ngy_queue_remove(q);
        //找到插入位置
        do{
            if(cmp(prev,q)<=0)
                break;
            prev = ngy_queue_prev(prev);
        } while (prev != ngy_queue_sentinel(h));
        ngy_queue_t_insert_after(prev, q);
    }
}

ngy_queue_t *ngy_queue_next(ngy_queue_t *q)
{
    return q->next;
}

ngy_queue_t *ngy_queue_prev(ngy_queue_t *q)
{
    return q->prev;
}

void ngy_queue_t_insert_after(ngy_queue_t *q, ngy_queue_t *x)
{
    ngy_queue_insert_head(q, x);
}