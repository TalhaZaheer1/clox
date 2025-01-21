#include "../common.h"

typedef enum { VAL_BOOL, VAL_NUMBER, VAL_NIL } ValueType;

typedef struct {
  ValueType tag;
  union {
    bool boolean;
    double number;
  } as;
} Value;

#define CREATE_NUMBER(literal) ((Value){VAL_NUMBER, {.number = literal}})
#define CREATE_BOOLEAN(literal) ((Value){VAL_BOOL, {.boolean = literal}})
#define CREATE_NIL() ((Value){VAL_NIL, {.number = 0}})

#define IS_BOOL(value) (value.tag == VAL_BOOL)
#define IS_NUMBER(value) (value.tag == VAL_NUMBER)
#define IS_NIL(value) (value.tag == VAL_NIL)

#define AS_NUMBER(value) (value.as.number)
#define AS_BOOL(value) (value.as.boolean)

typedef struct {
  int count;
  int capacity;
  Value *values;
} ValueArray;

void initValueArray(ValueArray *arr);
int writeValueArray(ValueArray *arr, Value constant);
void freeValueArray(ValueArray *arr);
void printValue(Value value);
