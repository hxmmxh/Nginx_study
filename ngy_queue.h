#ifndef HXM_NGY_QUEUE_H
#define HXM_NGY_QUEUE_H

#include "ngy_headers.h"

typedef struct ngy_queue_s ngy_queue_t;

struct ngy_queue_s
{
    ngy_queue_t *prev;
    ngy_queue_t *next;
};

//作为容器时所支持的12种方法
//初始化
void ngy_queue_init(ngy_queue_t *p);
//判断是否为空
ngy_uint_t ngy_queue_empty(ngy_queue_t *p);
//在头部插入
void ngy_queue_insert_head(ngy_queue_t *h, ngy_queue_t *x);
//在尾部插入
void ngy_queue_insert_tail(ngy_queue_t *h, ngy_queue_t *x);
//返回第一个元素
ngy_queue_t *ngy_queue_head(ngy_queue_t *h);
//返回最后一个元素
ngy_queue_t *ngy_queue_last(ngy_queue_t *h);
//返回自身？？表示链表容器结构体的指针
ngy_queue_t *ngy_queue_sentinel(ngy_queue_t *h);
//移除元素x，x是某容器中的元素
void ngy_queue_remove(ngy_queue_t *x);
//拆分链表，将链表h以元素q为界拆分成两个链表h和n
//h由前半部分构成，不包含q，n由后半部分组成，q为其首元素
void ngy_queue_split(ngy_queue_t *h, ngy_queue_t *q, ngy_queue_t *n);
//合并链表，h,n都是容器，把n添加到h的末尾
void ngy_queue_add(ngy_queue_t *h, ngy_queue_t *n);
//返回链表的中心元素，如果有N个返回第N/2+1个
ngy_queue_t *ngy_queue_middle(ngy_queue_t *h);
//排序，cmpfunc是比较方法，类型为
// ngy_int_t (*cmpfunc)(const ngy_queue_t *, const ngy_queue_t *)
void ngy_queue_sort(ngy_queue_t *queue, ngy_int_t (*cmp)(const ngy_queue_t *, const ngy_queue_t *));

//作为链表元素时支持的四种方法
//返回下一个元素
ngy_queue_t *ngy_queue_next(ngy_queue_t *q);
//返回上一个元素
ngy_queue_t *ngy_queue_prev(ngy_queue_t *q);
//q是某一元素结构体的ngy_queue_t成员的指针
//type是链表元素结构体的类型名称，link是结构体种ngy_queue_t类型的成员名字
//返回q所在结构体的地址
#define ngy_queue_data(q, type, link) \
    (type *)((u_char *)q - offsetof(type, link))
//将元素x插入到元素q之后
void ngy_queue_insert_after(ngy_queue_t *q, ngy_queue_t *x);

#endif