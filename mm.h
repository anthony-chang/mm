#ifndef MM_H
#define MM_H

#include <pthread.h>

#define CHECK_NULLPTR_RET(p)                                \
                do {if(!p) {                                \
                        printf("%s: NULLPTR!\n", __func__); \
                        return; }} while(0);
#define CHECK_NULLPTR_RET_VAL(p, val)                       \
                do {if(!p) {                                \
                        printf("%s: NULLPTR!\n", __func__); \
                        return val; }} while(0);;

typedef struct ALIGN16 { unsigned char s[16]; } ALIGN16;

typedef union header {
    struct {
        size_t size;
        union header *next;
    } st;
    ALIGN16 align; // align to 16 bytes
} header_t;

typedef struct mm_stats {
    unsigned int num_allocced;
    size_t b_allocced;
} mm_stats_t;

header_t *head, *tail;

pthread_mutex_t lock_g;
mm_stats_t stats_g;

void* mm_alloc(size_t size);
void mm_free(void* p);
void mm_free_all();







#endif // MM_H

