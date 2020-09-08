#include <stdio.h>
#include <string.h>

#include "mm.h"

void func_test1() {
    void *p = mm_malloc(sizeof(int));
    int* arr = mm_calloc(10, sizeof(int));
    char* str = mm_malloc(sizeof(char) * 6);

    *(int*)p = 3;
    for(int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    strcpy(str, "Hello");

    printf("p = %d\n", *(int*)p);
    for(int i = 0; i < 10; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    printf("str = %s\n", str);

    char* new_str = (char*) mm_realloc((void*)str, sizeof(char) * 13);
    strcpy(new_str, "Hello World!");
    printf("new_str = %s\n", new_str);
}


int main(void) {
    func_test1();

    mm_free_all(1);
    return 0;
}