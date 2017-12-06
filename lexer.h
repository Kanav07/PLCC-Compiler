// Batch         - 55
// Avinav Singla - 2010A7PS125P
// Kanav         - 2010A7PS152P



#include"lexerDef.h"

// -------------------------- Required Functions-------------------

FILE *getStream(FILE *fp, buffer B, buffersize k);  // Populates the buffer with a string of size k from the input file

tokenInfo getNextToken( FILE *fp, link *head); // This functions returns one token when invoked.

//--------------------------Support Functions----------------------

TokenList initialize(); // Used for initializing the linked list for holding tokens.

link addToList(tokenInfo t, link head); // Adds a token to linked list.
	
int isAllowed(char c);	// For deciding special symbols.

char *kewordsFieldId(char temp[]); // Lookup Table for keywords.

FILE *fileEdit(char *filename); // Edits the given input file.



