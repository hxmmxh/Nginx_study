#include "ngy_headers.h"

typedef struct
{
    u_char *str;
    ngy_queue_t qEle;
    int num;
} TestNode;

ngy_int_t compTestNode(const ngy_queue_t *a, const ngy_queue_t *b)
{
    TestNode *aNode = ngy_queue_data(a, TestNode, qEle);
    TestNode *bNode = ngy_queue_data(b, TestNode, qEle);
    return aNode->num > bNode->num;
}

void printqueue(ngy_queue_t *q)
{
    for(ngy_queue_t *tem=ngy_queue_head(q);q!=ngy_queue_sentinel(q);q=ngy_queue_next(q))
    {
        TestNode *node = ngy_queue_data(q, TestNode, qEle);
        printf("%d ", node->num);
    }
    printf("\n");
}

int main()
{
    ngy_queue_t queueContainer;
    ngy_queue_init(&queueContainer);

    TestNode node[5];
    for (int i = 0; i < 5; ++i)
        node[i].num = i;

    ngy_queue_insert_tail(&queueContainer, &node[0].qEle);
    ngy_queue_insert_head(&queueContainer, &node[1].qEle);
    ngy_queue_insert_tail(&queueContainer, &node[2].qEle);
    ngy_queue_insert_after(&queueContainer, &node[3].qEle);
    ngy_queue_insert_tail(&queueContainer, &node[4].qEle);

    printqueue(&queueContainer);

    ngy_queue_sort(&queueContainer, compTestNode);
    printqueue(&queueContainer);
}
