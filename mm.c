#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mm.h"

void* mm_malloc(size_t size) {
    size_t total_size;
    void* p;
    header_t *header;

    if(size <= 0) {
        return NULL;
    }

    pthread_mutex_lock(&lock_g);

    total_size = sizeof(header_t) + size;
    p = malloc(total_size);
    if(!p) {
        pthread_mutex_unlock(&lock_g);
        return NULL;
    }
    // add to list
    header = p;
    header->st.size = size;
    header->st.next = NULL;
    if(!head) {
        head = header;
    }
    if(tail) {
        tail->st.next = header;
    }
    tail = header;
    // stats
    stats_g.num_allocced++;
    stats_g.b_allocced += size;

    pthread_mutex_unlock(&lock_g);
    return (void*) (header + 1);
}

void mm_free(void *p) {
    header_t *header, **indirect;
    CHECK_NULLPTR_RET(p);

    pthread_mutex_lock(&lock_g);
    header = ((header_t*)p) - 1;

    // remove from list
    indirect = &head;
    while(*indirect != header) {
        indirect = &(*indirect)->st.next;
    }
    *indirect = header->st.next;

    // stats
    stats_g.num_allocced--;
    stats_g.b_allocced -= header->st.size;

    free(header);
    pthread_mutex_unlock(&lock_g);
}

void mm_free_all(unsigned int dump_stats) {
    header_t *cur = head, *next;
    unsigned int num_freed = 0, num_allocced = stats_g.num_allocced;
    size_t b_freed = 0, b_allocced = stats_g.b_allocced;

    pthread_mutex_lock(&lock_g);

    // iterate list
    while(cur) {
        num_freed++;
        b_freed += cur->st.size;
        next = cur->st.next;
        free(cur);
        cur = next;
    }

    // housekeeping
    if(num_freed == num_allocced && b_freed == b_allocced) {
        stats_g.num_allocced = 0;
        stats_g.b_allocced = 0;
    }
    else {
        printf("Failed to free all memory.\n");
        printf("MEM BLOCKS ALLOCATED: %u | BYTES ALLOCATED: %zu\n", num_allocced, b_allocced);
        printf("MEM BLOCKS FREED:     %u | BYTES FREED:     %zu\n", num_freed, b_freed);

    }

    pthread_mutex_unlock(&lock_g);

    if(dump_stats) {
        printf("___________________________________________________\n");
        printf("MEM BLOCKS ALLOCATED: %u | BYTES ALLOCATED: %zu\n", num_allocced, b_allocced);
        printf("MEM BLOCKS FREED:     %u | BYTES FREED:     %zu\n", num_freed, b_freed);
        printf("___________________________________________________\n");
    }
}

int main(int argc, const char** argv) {
    int *p = mm_malloc(sizeof(int));
    int *p1 = mm_malloc(sizeof(int));
    int *p2 = mm_malloc(sizeof(int));
    int *p3 = mm_malloc(sizeof(int));
    int *p4 = mm_malloc(sizeof(int));
    int *p5 = mm_malloc(69);

    *p = 2934485;

    printf("%p, %d\n", p, *p);
    mm_free(p4);
    mm_free_all(1);

    return 0;
}