#include "../common.h"

typedef double Value;

typedef struct {
  int count;
  int capacity;
  Value *values;
} ValueArray;

void initValueArray(ValueArray *arr);
int writeValueArray(ValueArray *arr, Value constant);
void freeValueArray(ValueArray *arr);
void printValue(Value value);
