#include <stdio.h>

#include "mm.h"

int main(void) {
    void *p = (int*)mm_malloc(sizeof(int));

    p = (unsigned long long*) mm_realloc(p, sizeof(unsigned long long));

    mm_free_all(1);


    return 0;
}