#include "./value.h"
#include "../VM/vm.h"
#include "../memory.h"
#include <stddef.h>
#include <string.h>

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

void printValue(Value value) {
  switch (value.tag) {
  case VAL_BOOL:
    printf(AS_BOOL(value) ? "true" : "false");
    break;
  case VAL_NUMBER:
    printf("%g", AS_NUMBER(value));
    break;
  case VAL_NIL:
    printf("nil");
    break;
  case VAL_OBJ:
    printObjVal(AS_OBJ(value));
    break;
  default:
    return;
  }
}

void printObjVal(Obj *obj) {
  switch (obj->type) {
  case OBJ_STRING:
    printObjStr((ObjString *)obj);
    break;
  }
}

void printObjStr(ObjString *string) { printf("%s", string->chars); }

bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString *copyStr(const char *str, size_t length) {
  char *heapStr = ALLOCATE(char, length + 1);
  memcpy(heapStr, str, length);
  heapStr[length] = '\0';
  return allocateStrObj(heapStr, length);
}

ObjString *allocateStrObj(char *heapStr, size_t length) {
  ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length = length;
  string->chars = heapStr;
  return string;
}

void *allocateObj(size_t size, ObjType type) {
  Obj *newObj = reallocate(NULL, 0, size);
  newObj->type = type;
  newObj->next = vm.objects;
  vm.objects = newObj;
  return newObj;
}
