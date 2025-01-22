#include "../common.h"

typedef enum { VAL_BOOL, VAL_NUMBER, VAL_NIL, VAL_OBJ } ValueType;

typedef enum { OBJ_STRING, OBJ_FUNCTION } ObjType;

typedef struct {
  ObjType type;
} Obj;

typedef struct {
  Obj obj;
  int length;
  char *chars;
} ObjString;

typedef struct {
  ValueType tag;
  union {
    bool boolean;
    double number;
    Obj *obj;
  } as;
} Value;

bool isObjType(Value value, ObjType type);

#define CREATE_NUMBER(literal) ((Value){VAL_NUMBER, {.number = literal}})
#define CREATE_BOOLEAN(literal) ((Value){VAL_BOOL, {.boolean = literal}})
#define CREATE_NIL() ((Value){VAL_NIL, {.number = 0}})
#define CREATE_OBJ(pointer) ((Value){VAL_OBJ, {.obj = pointer}})

#define IS_BOOL(value) (value.tag == VAL_BOOL)
#define IS_NUMBER(value) (value.tag == VAL_NUMBER)
#define IS_NIL(value) (value.tag == VAL_NIL)
#define IS_OBJ(value) (value.tag == VAL_OBJ)

#define AS_NUMBER(value) (value.as.number)
#define AS_BOOL(value) (value.as.boolean)
#define AS_OBJ(value) (value.as.obj)

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) (isObjType(value, OBJ_STRING))
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef struct {
  int count;
  int capacity;
  Value *values;
} ValueArray;

void initValueArray(ValueArray *arr);
int writeValueArray(ValueArray *arr, Value constant);
void freeValueArray(ValueArray *arr);
void printValue(Value value);
Value createStringVal(char *strPtr, size_t length);
