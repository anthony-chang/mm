# mm
 mm is an implementation of a thread-safe memory manager for C. It acts as a wrapper for the standard POSIX `malloc()`, `calloc()`, `realloc()`, and `free()`.

 ## Usage


 ## TODO

 - mm uses a naive method of storing `malloc`'ed metadata in a linked list, so calling `free()` takes linear time.
   - This can be improved by storing metadata blocks in a hashmap so `free()` can be done in constant time.
 - Simple mark-and-sweep garbage collection.
