#include "symbolTableDef.h"


extern int hash(char s[],int size);
extern SPointer createSymbolTable(int functionsize,int variablesize,int recordsize);
extern FPointer insertFunctionBuckets(SPointer s,char funcname[]);
extern VPointer insertVariableBuckets(FPointer fglobal,FPointer fp,nodeinfo TInfo,char T[],int off);
extern void insertRecordBuckets(VPointer vp,nodeinfo TInfo,char T[],int off);
extern void printSymbolTable(SPointer sp);
extern FPointer fillingGlobalBucket(SPointer s);
extern void findingFunctions(SPointer s , FPointer fglobal , abstractSyntaxTree AST);
extern void populateSymbolTable(FPointer f,FPointer fglobal,abstractSyntaxTree AST);
extern void addingrecordtoSymbolTable(abstractSyntaxTree AST,VPointer vp);
extern int findoffset(FPointer fglobal, char recordid[]);


