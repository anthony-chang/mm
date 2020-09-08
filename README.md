# mm
 mm is an implementation of a thread-safe memory manager for C. It acts as a wrapper for the standard POSIX `malloc()`, `calloc()`, `realloc()`, and `free()`.

 ## Instructions

 ### Download and compile
```
$ git clone git@github.com:anthony-chang/mm.git
$ cd mm
$ make
```
### Run tests
```
$ ./mm
```
Or run with [valgrind](https://www.valgrind.org/)
```
$ valgrind --leak-check=yes ./mm
```
### Clean
```
$ make clean
```



### Usage
```C
#include "mm.h"

void foo() {
  int *arr = mm_calloc(5, sizeof(int));
  for(int i = 0;  i < 5; i++) {
    arr[i] = i;
  }

  int *bigger_arr = mm_realloc(arr, sizeof(int) * 10);
  for(int i = 5;  i < 10; i++) {
    bigger_arr[i] = i;
  }

  for(int i = 0; i < 10; i++) {
    printf("%d, ", bigger_arr[i]); // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  }

  // no free :D
}

int main(int argc, const char** argv) {
  foo();

  mm_free_all(1);
  return 0;
}

```

 ## TODO

 - mm uses a naive method of storing `malloc`'ed metadata in a linked list, so calling `free()` takes linear time.
   - This can be improved by storing metadata blocks in a hashmap so `free()` can be done in constant time.
 - Simple mark-and-sweep garbage collection.
