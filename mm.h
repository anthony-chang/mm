#ifndef MM_H
#define MM_H

#include <pthread.h>

/*****************************************************************
 * Definitions
 *****************************************************************/
#define CHECK_NULLPTR_RET(p)                                \
                do {if(!p) {                                \
                        printf("%s: NULLPTR!\n", __func__); \
                        return; }} while(0);
#define CHECK_NULLPTR_RET_VAL(p, val)                       \
                do {if(!p) {                                \
                        printf("%s: NULLPTR!\n", __func__); \
                        return val; }} while(0);;

/*****************************************************************
 * Types
 *****************************************************************/
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

/*****************************************************************
 * Globals
 *****************************************************************/
header_t *head, *tail;
pthread_mutex_t lock_g;
mm_stats_t stats_g;

/*****************************************************************
 * Functions
 *****************************************************************/
void* mm_malloc(size_t size);
void* mm_calloc(size_t num, size_t nsize);
void* mm_realloc(void *p, size_t size);
void mm_free(void *p);
void mm_free_all(unsigned int dump_stats);


#endif // MM_H

