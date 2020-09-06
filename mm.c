#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mm.h"

static header_t* get_free_block(size_t size) {
    header_t* cur = head;
    while(cur) {
        if(cur->st.is_free && cur->st.size >= size) {
            return cur;
        }
        cur = cur->st.next;
    }
    return NULL;
}

void* mm_alloc(size_t size) {
    size_t total_size;
    void* p;
    header_t *header;

    if(size <= 0)
        return NULL;

    pthread_mutex_lock(&lock_g);

    header = get_free_block(size);
    if(header) { // already existing block
        header->st.is_free = 0;
        pthread_mutex_unlock(&lock_g);
        return (void*) (header + 1);
    }

    // no block found, we allocate it
    total_size = sizeof(header_t) + size;
    p = malloc(total_size);
    if(!p) {
        pthread_mutex_unlock(&lock_g);
        return NULL;
    }
    header = p;
    header->st.size = size;
    header->st.is_free = 1;
    header->st.next = NULL;
    if(!head) {
        head = header;
    }
    if(tail) {
        tail->st.next = header;
    }
    tail = header;
    pthread_mutex_unlock(&lock_g);
    return (void*) (header + 1);

}


int main(int argc, const char** argv) {
    int *p = mm_alloc(sizeof(int));
    *p = 2934485;

    printf("%p, %d\n", p, *p);

    return 0;
}