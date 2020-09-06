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

void* mm_calloc(size_t num, size_t nsize) {
    size_t size;
    void *p;

    if(!num || !nsize) {
        return NULL;
    }

    size = num * nsize;
    if(nsize != size / num) { // check overflow
        return NULL;
    }
    p = mm_malloc(size);
    CHECK_NULLPTR_RET_VAL(p, NULL);
    memset(p, 0, size);
    return p;
}

void* mm_realloc(void *p, size_t size) {
    header_t *header;
    void* ret;

    if(!p || !size) { // C-standard defined behaviour
        return mm_malloc(size);
    }

    header = ((header_t*)p) - 1;
    ret = mm_malloc(size);
    if(ret) {
        pthread_mutex_lock(&lock_g);
        memcpy(ret, p, header->st.size);
        pthread_mutex_unlock(&lock_g);

        mm_free(p);
    }
    return ret;
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
    void *p = (int*)mm_malloc(sizeof(int));

    p = (unsigned long long*) mm_realloc(p, sizeof(unsigned long long));

    *(unsigned long long*)p = 254235390850985092;

    printf("%llu\n", *(unsigned long long*)p);

    mm_free_all(1);


    return 0;
}