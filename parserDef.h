// Batch         - 55
// Avinav Singla - 2010A7PS125P
// Kanav         - 2010A7PS152P



typedef enum { TRUE , FALSE}boolean;

struct stacknode;
typedef struct stacknode Stacknode;
typedef Stacknode *Stacklink;

struct stacknode
{
	char tok[50];
	Stacklink next;
};



struct stackhead;
typedef struct stackhead Stackhead;
struct stackhead
{
	int size;
	Stacklink top;
        boolean error;
};


typedef struct{
char lexemeName[50];
int lineNo;
char tokenName[30];
int valueIfNumber;
char parentNodeSymbol[30];
boolean IsLeafNode;
char NodeSymbol[30];
}nodeinfo;

struct pnode;
typedef struct pnode pnode;
typedef struct pnode *Node;

struct pnode{
nodeinfo info;
Node parent;
Node children[20];
int numch;
int positioninallchildren;
};

struct parsetree;
typedef struct parsetree parsetree;
typedef struct parsetree *parseTree ;

struct parsetree{
Node head;
Node temphead;
int size;
};


typedef int table;
typedef FILE *grammar;
typedef parseTree abstractSyntaxTree ;













