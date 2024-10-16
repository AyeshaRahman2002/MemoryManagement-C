#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "kernel/types.h"
#include "user/user.h"
#include <stddef.h>
#include <stdbool.h>

// xv6 page size to more efficiently increase program size
#define PAGE_SIZE 1000

typedef union node Node;

void *_malloc(int size);
void _free(void *ptr);
Node *rqstMem(int units);

union node
{
    struct
    {
        union node *n;
        int size;
        int f;
        int init; // for debugging purposes
    };
};


extern Node block1;
extern Node *fnodes;
extern int initialisingdone;

#endif
