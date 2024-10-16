#include "memory_management.h"

Node block1;
Node *fnodes = 0;
int initialisingdone = 0;
bool runn = true;

// Requests more memory with size units
Node *rqstMem(int units)
{
    void *fmemory = sbrk(units * sizeof(Node));   //it'll look for that size in memory and give the address of memory location to freemem
    if (fmemory == (void *) -1)  // supposing system full then return 0
    {
        return 0;
    }
    
    Node *freednode = (Node *) fmemory;
    freednode->size = units;
    _free((void *) (freednode));  //running it through _free() to check whether its full or not
    freednode->f = 1; //freednode is free initialising to 1
    return fnodes;
    
    //check according to page size
    if (units < PAGE_SIZE)
    {
        units = PAGE_SIZE;
    }
    
    // testing to check whether unit is 0
    if (units <= 0)
    {
        return 0;
    }
}

void *_malloc(int size)
{
    
    // testing to check whether size is 0
    if (size <=0 )
    {
        return 0;
    }

    // Initialise the first block since the blocks have not been initialised yet and create a circular linked list
    if (initialisingdone == 0) //if there is no nodes created yet
    {
        block1.n = &block1;  //creating block1
        block1.size = 0;
        block1.n->f = 1;  //next block to the first block is free
        block1.init = 1;
        fnodes = block1.n;
        initialisingdone = 1;
    }
    
    Node *pointednode = fnodes;
    Node *node2 = pointednode->n;
    pointednode->init = 1;
    node2->init = 1;

    // Looking for a free node
    if (node2 == fnodes)
    {
        if ((node2 = rqstMem(size)) == 0)
        {
            return 0;
        }
    }

    // While loop is run infinitely
    while (true)
    {
        // Checking the size of the node2 in case it is bigger than wanted size
        if (node2->size >= size)
        {
            // When size of node2 is not equal to size required
            if (node2->size != size)
            {
                node2->size = node2->size - size;
                node2 = node2 + node2->size;
                node2->size = size;

                pointednode->f = 1;   //the node we are at is taken as free and initialised
                fnodes = pointednode;
                fnodes->init = 1;
                return (void *) (node2);     //node2+1 before apparently to go to next node
            }
            else   // When size of node2 is equal to size required
            {
                pointednode->n = node2->n;
                pointednode->f = 1;
                fnodes = pointednode;
                fnodes->init = 1;
                return (void *) (node2);     //node2+1 before
            }
        }

        // going on to next node
        pointednode = node2;
        node2 = node2->n;
    }
}


void _free(void *ptr)
{
    Node *node2 = fnodes;   //node2 is storing the address of first block of the linked list
    Node *freednode = ((Node *) ptr);   //freednode stores the address made in request_mem
  
    if (ptr == NULL)
    {
        node2 = 0;
        return;
    }

    // Iterates over  nodes to find a suitable place to store *ptr
    while (!((freednode < node2->n) && (node2 < freednode)))   //node2>freednode>node2->n
    {
        if (((freednode < node2->n) || (node2 < freednode)) && (node2 >= node2->n))
        {
            break;   //sequence breaking
        }
        node2 = node2->n;
        //checking through the entire linked list and running the while loop again
    }

    //to check if size of node2->n and freednnode + freednode->size address is the same
    if ((freednode + freednode->size) == node2->n)
    {
        freednode->size = freednode->size + node2->n->size;  //second element (freednode)-> is equal to freednode->size + node2->n->size to form the next node of the list
        freednode->n = node2->n->n;  //changing the address of the size gotten above
    }
    else
    {
        freednode->n = node2->n;
    }
    if ((node2 + node2->size) != freednode)
    {
        node2->n = freednode;
    }
    else
    {
        node2->size = node2->size + freednode->size;
        node2->n = freednode->n;
    }
    fnodes = node2;
}
