#include<stdio.h>
#include"parserDef.h"

// Batch         - 55
// Avinav Singla - 2010A7PS125P
// Kanav         - 2010A7PS152P

// Required Functions

void createParseTable(FILE *G, int T[][54]);     // Creates the Predictive Parsing Table

parseTree parseInputSourceCode(char *testcaseFile, int T[][54]); // Parses the input source code and creates the parse tree

void printParseTree(parseTree ptree, FILE *fp); // Prints the parse tree to an output file.

void createAbstractSyntaxtree(parseTree ptree, abstractSyntaxTree A) ;// Generates the AST using the Parsing Table and Semantic Rules.

void printAST(abstractSyntaxTree A, FILE *fp , int *totalAllocatedMemory) ;// Prints the AST and computes the total space allocated to AST.


// Support Functions

int getIndex(char tname[20]);  // Gives the index of terminal in the Predictive Parsing Table

int getNTIndex(char ntname[20]); // Gives the index of non-terminal in the Predictive Parsing Table

char *getRule(int index); // Gives the rule definition corresponding to the rulenumber

parseTree initializeptree(void); // Creates and initializes a parsetree and returns a parsetree pointer

abstractSyntaxTree initializeAbstractSyntaxTree(void); // Creates and initializes a parsetree and returns a parsetree pointer

Node createNode(nodeinfo ninfo, Node parentaddress,int position); // Creates a parsetree node and initializes it with the info provided.
 
 int tokenize(char *rule,char rulearray[][40]); // Tokenizes the rule definiton so that they could easily be indexed as children.

void insertnodes(parseTree ptree,char rulearray[][40],int no_of_tokens); // Inserts the node at the appropriate place

void adjustposition(parseTree ptree); // Adjusts the pointer which points at the current node to be expanded.

int checkifremaining(parseTree ptree); // Checks if a given "TK_END" is actually the end of the input.

void shiftup(parseTree T) ; // Helps in preprocessing of the given Parse Tree to generate the corresponding AST.

void handlingEpsAndPunc(parseTree ptree) ;// Removes punctuation and epsilon from parse tree

void handlingAssignment(parseTree ptree) ; // Handles Assignment Statement

void handlingConditional(parseTree ptree); // Handles conditional operators

void handlingArithmetic(parseTree ptree); // Handles arithmetic operators

// Stack Operations

Stackhead createstack(); // Creates and initializes a stack

char *top(Stackhead s); // Returns the top of stack and null if top is empty.

Stackhead push(Stackhead s ,char tok[50]); // Pushes the supplied token on top of stack and returns the updated stack.

Stackhead pushstring(Stackhead s,char token[500]); // Tokenizes the provided string and uses push to push them one by one.

Stackhead pop(Stackhead s); // Pops the stack and returns the updated stack

void printstack(Stackhead s); // Prints the stack.



