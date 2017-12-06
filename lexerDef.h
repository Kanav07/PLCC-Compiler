// Batch         - 55
// Avinav Singla - 2010A7PS125P
// Kanav         - 2010A7PS152P

typedef int buffersize;
typedef char* buffer;
typedef struct token {
	char lexemeName[30];
	char tokenName[30];	
	int lineno;
}tokenInfo;
typedef struct node tokenNode;
typedef tokenNode *link;
struct node                                                            
{
	tokenInfo t;
	link next;
};
struct tokenTable                     
{
	int size;
	link head;
};
typedef struct tokenTable TokenList;
static char *lexemes[25]={"with","parameters","end",
                  "while","int","real","type",
                  "global","parameter","list",
                  "input","output","int","real",
                  "endwhile","if","then","endif",
                  "read","write","return","call",
                  "record","endrecord","else"};

static char *tokens[25]={"TK_WITH","TK_PARAMETERS",
                    "TK_END","TK_WHILE","TK_INT",
                    "TK_REAL","TK_TYPE",
                    "TK_GLOBAL","TK_PARAMETER",
                    "TK_LIST","TK_INPUT","TK_OUTPUT",
                    "TK_INT","TK_REAL","TK_ENDWHILE",
                    "TK_IF","TK_THEN","TK_ENDIF",
                    "TK_READ","TK_WRITE","TK_RETURN",
                    "TK_CALL","TK_RECORD","TK_ENDRECORD",
                    "TK_ELSE"};



                    
