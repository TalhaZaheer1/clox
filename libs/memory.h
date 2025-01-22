#ifndef MEMORY_H
#define MEMORY_H

#include "./common.h"

#define GROW_CAPACITY(oldCapacity) (oldCapacity < 8 ? 8 : (oldCapacity * 2))

#define GROW_ARRAY(type, pointer, oldCount, newCount)                          \
  (type *)reallocate(pointer, sizeof(type) * oldCount, sizeof(type) * newCount)

#define FREE_ARRAY(type, pointer, oldCount)                                    \
  reallocate(pointer, sizeof(type) * oldCount, 0)

#define ALLOCATE(type, size) (type *)reallocate(NULL, 0, sizeof(type) * size)

#define FREE(type, pointer, oldCount)                                          \
  reallocate(pointer, sizeof(type) * oldCount, 0)

void *reallocate(void *pointer, size_t oldSize, size_t size);

#endif // !MEMORY_H
