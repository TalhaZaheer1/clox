#include "./value.h"
#include "../memory.h"

void initValueArray(ValueArray *arr) {
  arr->count = 0;
  arr->capacity = 0;
  arr->values = NULL;
}

int writeValueArray(ValueArray *arr, Value constant) {
  if (arr->capacity < arr->count + 1) {
    int oldCapacity = arr->capacity;
    arr->capacity = GROW_CAPACITY(oldCapacity);
    arr->values = GROW_ARRAY(Value, arr->values, oldCapacity, arr->capacity);
  }
  arr->values[arr->count++] = constant;
  return arr->count - 1;
}

void freeValueArray(ValueArray *arr) {
  arr->values = FREE_ARRAY(Value, arr->values, arr->capacity);
  arr->capacity = 0;
  arr->count = 0;
}

void printValue(Value value) { printf("%g", value); }
