#include "parser.h"

typedef enum {EMPTY = 0 , FILLED = 1}status;
//typedef enum{INT = 0 , REAL = 1, RECORD = 2 , UNKNOWN = 3}type;

typedef struct rBuckets recordBuckets;
typedef recordBuckets *RPointer;
struct rBuckets{
char t[20];
nodeinfo tInfo;
status s;
int offset;
};

typedef struct vBuckets variableBuckets;
typedef variableBuckets *VPointer;
struct vBuckets{
RPointer r;
char t[20];
nodeinfo tInfo;
status s;
int size;
int offset;
int inparameters;
int outparameters;
};

typedef struct fBuckets functionBuckets;
typedef functionBuckets *FPointer;
struct fBuckets{
VPointer v;
int size;
status s;
char functionName[20];
};

typedef struct sTable symbolTable;
typedef symbolTable *SPointer;
struct sTable{
FPointer f;
int size;
};



