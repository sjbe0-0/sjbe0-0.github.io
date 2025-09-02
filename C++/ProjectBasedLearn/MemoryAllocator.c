/*
brk: 指向堆末尾的标识符
sbrk(): 调整堆的大小，返回新的堆末尾地址
sbrk(0):给出程序中断的当前地址。
sbrk(n):使用正值调用sbrk会使brk增加n个字节，从而分配内存
sbrk(-n):使用负值调用sbrk会使brk减少n个字节，从而回收内存
失败时，sbrk()返回(void*)-1;

*/

/*
内存分布图：
high
           stack
           heap
           bss
           text
low
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <stdio.h>
typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;
    // 为了对齐，增加一个字节
    ALIGN stub;
};

typedef union header header_t;

header_t *head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock;

header_t *get_free_block(size_t size) {
    header_t *curr = head;
    while (curr) {
        //查看当前块是否足够分配size大小的内存
        if(curr->s.is_free && curr->s.size >= size) {
            return curr;
        }

        return NULL;
    }
}


void free(void* block)
{
    header_t *header, *tmp;
    //程序中断
    void* programbreak;

    if (!block) {
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t*)block - 1;
    
    //获取当前程序中断地址
    programbreak = sbrk(0);


    //判断当前块是否在程序中断地址之后，如果是，则合并当前块和下一个块
    if ((char*)block + header->s.size == programbreak) {
        if (head == tail) {
            head = tail = NULL;
        } else {
            tmp = head;
            while (tmp) {
                if (tmp->s.next == tail) {
                    tmp->s.next = NULL;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }

        sbrk(0 - header->s.size -sizeof(header_t));

        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

void* malloc(size_t size) 
{
    size_t total_size;
    void* block;
    header_t *header;

    if (size == 0) {
        return NULL;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);
    if (header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    total_size = sizeof(header_t) + size;
    block = sbrk(total_size);
    if (block == (void*) -1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = (header_t*)block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;

     if (!head) {
        head = header;
     } 

     if (tail) {
        tail->s.next = header;
     }

     tail = header;
     pthread_mutex_unlock(&global_malloc_lock);
     return (void*)(header + 1);
}

void* calloc(size_t num, size_t nsize)
{
    size_t size;
    void* block;

    if (!num || !nsize) {
        return NULL;
    }

    size = num * size;
    if (nsize != size / num) {
        return NULL;
    }

    block = malloc(size);
    if (!block) {
        return NULL;
    }

    memset(block, 0 , size);
    return block;
}

void* realloc(void* block, size_t size) 
{
    header_t * header;
    void* ret;
    if (!block || !size) {
        return malloc(size);
    }

    header = (header_t*)block - 1;
    if (header->s.size >= size) {
        return block;
    }
    ret = malloc(size);
    if (ret) {
        memcpy(ret, block, header->s.size);
        free(block);
    }

    return ret;
}

void print_mem_list() {
    header_t *curr = head;
    printf("head = %p, tail = %p \n", (void*)head, (void*)tail);
    while(curr) {
        printf("addr = %p, size = %zu, is_free = %u, next = %p \n",
        (void*)curr, curr->s.size, curr->s.is_free, (void*)curr->s.next);
        curr = curr->s.next;
    }
}