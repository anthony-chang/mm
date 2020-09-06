# mm
 mm is an implementation of a thread-safe memory manager for C. It acts as a wrapper for the standard POSIX `malloc()`, `calloc()`, `realloc()`, and `free()`.

 ## Instructions

 ### Download and compile & run tests
```
$ git clone git@github.com:anthony-chang/mm.git
$ cd mm
$ make
$ ./mm
```
### Usage
```C
#include "mm.h"

void foo() {
  int* pointer = (int*) mm_malloc(sizeof(int));
  *pointer = 5;

  // no free :D
}

int main(int argc, const char** argv) {
  foo();

  mm_free_all();
  return 0;
}

```

 ## TODO

 - mm uses a naive method of storing `malloc`'ed metadata in a linked list, so calling `free()` takes linear time.
   - This can be improved by storing metadata blocks in a hashmap so `free()` can be done in constant time.
 - Simple mark-and-sweep garbage collection.
