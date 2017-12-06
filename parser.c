#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"parser.h"
#include"lexer.h"

// Batch         - 55
// Avinav Singla - 2010A7PS125P
// Kanav         - 2010A7PS152P


// ----------------------------------------------DELIVERABLES-----------------------------------------


void createParseTable(FILE *G, int T[][54])
{
	int i,j;
	char non_terminal[50];
	char to_check_previous[50];
	char endofline[] = "END";
	char token[50];
	char followtoken[50];
	char epsilon[] = "eps";
	char expressions[200];
	int rulenumber= 0;
	int arrayrowindex = -1;
	int arraycolumnindex = 0;
	int count = 0,count1=0,condition = 0;
	FILE *firstpointer;
	FILE *followpointer;

	firstpointer = fopen("first.txt","r");
	followpointer = fopen("follow.txt","r");


while(fscanf(G,"%s",non_terminal) == 1)
{
	
	if(!strcmp(to_check_previous,non_terminal))
	{
		
		rulenumber++;
		while(fscanf(firstpointer,"%s",token) == 1)
		{
			if(strcmp(token,endofline))
			{
				if(!strcmp(token,epsilon))
				{	
					
					while(fscanf(followpointer,"%s",token) == 1)
					{
						if(strcmp(token,endofline))
						{
							arraycolumnindex = getIndex(token);
							T[arrayrowindex][arraycolumnindex] = rulenumber;
							
;						}
						else
						{condition=1;
						count++;break;
							}
					}
				}
				
				else				
				{
					arraycolumnindex = getIndex(token);
					T[arrayrowindex][arraycolumnindex] = rulenumber;
					
				}
			}
			
			else
			break;
		}
	}
	else
	{
		strcpy(to_check_previous,non_terminal);
		rulenumber++;
		arrayrowindex++;
		while(fscanf(firstpointer,"%s",token) == 1)
		{
			if(strcmp(token,endofline))
			{
				if(!strcmp(token,epsilon))
				{
					while(fscanf(followpointer,"%s",token) == 1)
					{
						if(strcmp(token,endofline))
						{
							arraycolumnindex = getIndex(token);
							T[arrayrowindex][arraycolumnindex] = rulenumber;
					
						}
						else
						{condition = 1;count++;break;}
					}
				}
				
				else				
				{
					arraycolumnindex = getIndex(token);
					T[arrayrowindex][arraycolumnindex] = rulenumber;
				
				}
			}
			
			else
			break;
		}
		
	}
	while(fscanf(G,"%s",expressions) == 1)
	if(!strcmp(expressions,endofline))
	{	
	count1++;
	break;
	}
	if(!condition)
	{
		while(fscanf(followpointer,"%s",followtoken) == 1)
		if(!strcmp(followtoken,endofline))
		{
			count++;	
			break;
		}
		
	}
	else	
	condition = 0;
}
	fclose(followpointer);
	fclose(firstpointer);
}

//-----------------------------------------------------------------------------------------------------------------


parseTree parseInputSourceCode(char *testcaseFile, int T[][54])
{
	Stackhead s;
	parseTree ptree;
	char initialstring[] = "<program> $ ";
	char string[100] = "<program>";
	char rule[300];
	char rulearray[30][40];	
	char nonterminal[30];
	Node initialnode;
	nodeinfo ninfo;
	tokenInfo t;
	FILE *fp;
	
	int no_of_tokens = 0,ruleno = 0;
	TokenList tokens;
	tokens=initialize();
	link header=tokens.head;


	fp = fopen(testcaseFile,"r");
	strcpy(ninfo.lexemeName,string);
	ninfo.lineNo = 0;
	strcpy(ninfo.tokenName,string);
	strcpy(ninfo.parentNodeSymbol,"\0");
	ninfo.IsLeafNode = FALSE;

	strcpy(ninfo.NodeSymbol,string);

	initialnode = (Node)createNode(ninfo,NULL,0);

	ptree = (parseTree)initializeptree();

	s = createstack(s);
	s = pushstring(s,initialstring);

	ptree->head = initialnode;
	ptree->temphead = initialnode;
	ptree->size++;
	t = getNextToken(fp,&header);
	
	
	while(!strcmp(t.tokenName,"TK_COMMENT"))
		t = getNextToken(fp,&header);
	
	while(strcmp(top(s)," $") && strcmp(t.tokenName,"$"))
	{
		while(!strcmp(t.tokenName,"TK_COMMENT"))
			t = getNextToken(fp,&header);
		if(strcmp(top(s),t.tokenName))
		{
			
			if(getNTIndex(top(s)) == -1)
			{	
				printf("ERROR_5 : Unknown nonterminal %s\n",top(s));
				break;
			}
			else
			{	
				ruleno = T[getNTIndex(top(s))][getIndex(t.tokenName)];
				if(ruleno == 0)
				{
					printf("ERROR_5 : No rule defined when lookahead is %s and top of stack is %s\n",t.tokenName,top(s));					
					break;
				}
	               		strcpy(rule,getRule(ruleno-1));
				s = pop(s);
				if(!strcmp(rule,"eps "))
				{
					s = pushstring(s,rule);
					no_of_tokens = tokenize(rule,rulearray);		
				        insertnodes(ptree,rulearray,no_of_tokens);
					ptree->temphead->info.IsLeafNode = TRUE;
					s = pop(s);	
					adjustposition(ptree);
				}
				else
				{
					s = pushstring(s,rule);
					no_of_tokens = tokenize(rule,rulearray);		
					insertnodes(ptree,rulearray,no_of_tokens);
				}
			}
				
		}
		else
		{
			strcpy(ptree->temphead->info.lexemeName,t.lexemeName);
			ptree->temphead->info.lineNo = t.lineno;
			strcpy(ptree->temphead->info.tokenName,t.tokenName);
			if(!strcmp(t.tokenName,"TK_NUM") || !strcmp(t.tokenName,"TK_RNUM"))
				ptree->temphead->info.valueIfNumber = (int)t.lexemeName;
			ptree->temphead->info.IsLeafNode = TRUE;
			strcpy(ptree->temphead->info.NodeSymbol,t.tokenName);
			s = pop(s);	
			if(strcmp(top(s),"$"))
				adjustposition(ptree);
			else
				break;
			t = getNextToken(fp,&header);
			
		}
		
	}
	t = getNextToken(fp,&header);

	if(!strcmp(top(s),"$") && !strcmp(t.tokenName,"END"))
		printf("SUCCESS: Successful Compilation\n");
	else
	{
		printf("FAILURE: Unsuccessful Compilation\n");
		exit(1);
	}
	return ptree;
}

//------------------------------------------------------------------------------------------------

void createAbstractSyntaxtree(parseTree ptree, abstractSyntaxTree A)
{	
	A = initializeAbstractSyntaxTree();
	A->head = ptree->head;
	A->temphead = ptree->head;
	ptree->temphead = ptree->head;
	handlingEpsAndPunc(ptree);
	ptree->temphead=ptree->head;
	shiftup(ptree);
	ptree->temphead=ptree->head;
	handlingAssignment(ptree);
	ptree->temphead=ptree->head;
	//handlingConditional(ptree);
	//ptree->temphead = ptree->head;
	//handlingArithmetic(ptree);
	ptree->temphead = ptree->head;
}

//---------------------------------------------------------------------------------------------------

void printAST(abstractSyntaxTree A, FILE *fp, int *totalAllocatedMemory)
{

int condition = 0;
	char isaLeafNode[] = "isaLeafNode";
	char isNotaLeafNode[] = "isNotaLeafNode";
	char dash[] = "----";
	
	char newline[] = "\n";
	
	
	

	if(A->temphead->children[0] == NULL)
	{
		if(!strcmp(A->temphead->info.tokenName,"TK_NUM") || !strcmp(A->temphead->info.tokenName,"TK_RNUM"))	
		{
			fprintf(fp,"%20s %20s %20d %20s %20s %20s %20s\n",A->temphead->info.lexemeName,A->temphead->info.tokenName,A->temphead->info.lineNo,isaLeafNode,A->temphead->info.lexemeName,A->temphead->info.parentNodeSymbol,dash);
			*(totalAllocatedMemory) = *(totalAllocatedMemory) + 1;
			fprintf(fp,"%s",newline);
		}
		else
		{
			fprintf(fp,"%20s %20s %20d %20s %20s %20s %20s\n",A->temphead->info.lexemeName,A->temphead->info.tokenName,A->temphead->info.lineNo,isaLeafNode,dash,A->temphead->info.parentNodeSymbol,dash);
			*(totalAllocatedMemory) = *(totalAllocatedMemory) + 1;
			fprintf(fp,"%s",newline);
		}
		while(!condition)
		{
			if(!strcmp(A->temphead->info.tokenName,"TK_END") && !checkifremaining(A))
				return ;
			if((A->temphead->positioninallchildren)+1 < A->temphead->parent->numch)
			{
				A->temphead = A->temphead->parent->children[(A->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				A->temphead = A->temphead->parent;
		}
	printAST(A,fp,totalAllocatedMemory);
	}
	else
	{	
		if(!strcmp("<program>",A->temphead->info.tokenName))
		{	
			fprintf(fp,"%20s %20s %20s %20s %20s %20s %20s\n",A->temphead->info.lexemeName,dash,dash,isNotaLeafNode,dash,dash,A->temphead->info.NodeSymbol);
			*(totalAllocatedMemory) = *(totalAllocatedMemory) + 1;
			fprintf(fp,"%s",newline);
		}	
		else
		{	
			fprintf(fp,"%20s %20s %20s %20s %20s %20s %20s\n",A->temphead->info.lexemeName,dash,dash,isNotaLeafNode,dash,A->temphead->info.parentNodeSymbol,A->temphead->info.NodeSymbol);
			*(totalAllocatedMemory) = *(totalAllocatedMemory) + 1;
			fprintf(fp,"%s",newline);
		}
		A->temphead = A->temphead->children[0];
		printAST(A,fp,totalAllocatedMemory);
	}
}


// --------------------------------------------------------------------------------------------------------


void printParseTree(parseTree ptree, FILE *fp)
{

	int condition = 0;
	char isaLeafNode[] = "isaLeafNode";
	char isNotaLeafNode[] = "isNotaLeafNode";
	char dash[] = "---";
	char newline[] = "\n";
	
	
	

	if(ptree->temphead->children[0] == NULL)
	{
		if(!strcmp(ptree->temphead->info.tokenName,"TK_NUM") || !strcmp(ptree->temphead->info.tokenName,"TK_RNUM"))	
		{
			fprintf(fp,"%20s %20s %20d %20s %20s %20s %20s\n",ptree->temphead->info.lexemeName,ptree->temphead->info.tokenName,ptree->temphead->info.lineNo,isaLeafNode,ptree->temphead->info.lexemeName,ptree->temphead->info.parentNodeSymbol,dash);
			fprintf(fp,"%s",newline);
		}
		else
		{
			fprintf(fp,"%20s %20s %20d %20s %20s %20s %20s\n",ptree->temphead->info.lexemeName,ptree->temphead->info.tokenName,ptree->temphead->info.lineNo,isaLeafNode,dash,ptree->temphead->info.parentNodeSymbol,dash);
			fprintf(fp,"%s",newline);
		}
		while(!condition)
		{
			if(!strcmp(ptree->temphead->info.tokenName,"TK_END") && !checkifremaining(ptree))
				return ;
			if((ptree->temphead->positioninallchildren)+1 < ptree->temphead->parent->numch)
			{
				ptree->temphead = ptree->temphead->parent->children[(ptree->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				ptree->temphead = ptree->temphead->parent;
		}
	printParseTree(ptree,fp);
	}
	else
	{	
		if(!strcmp("<program>",ptree->temphead->info.tokenName))
		{	
			fprintf(fp,"%20s %20s %20s %20s %20s %20s %20s\n",ptree->temphead->info.lexemeName,dash,dash,isNotaLeafNode,dash,dash,ptree->temphead->info.NodeSymbol);
			fprintf(fp,"%s",newline);
		}	
		else
		{	
			fprintf(fp,"%20s %20s %20s %20s %20s %20s %20s\n",ptree->temphead->info.lexemeName,dash,dash,isNotaLeafNode,dash,ptree->temphead->info.parentNodeSymbol,ptree->temphead->info.NodeSymbol);
			fprintf(fp,"%s",newline);
		}
		ptree->temphead = ptree->temphead->children[0];
		printParseTree(ptree,fp);
	}

}

// ---------------------------------------------------------------------------------------------------------





// -------------------------------------SUPPORT FUNCTIONS--------------------------------------------------



void shiftup(parseTree ptree)
{	
	Node curr;
	char parentNodeSymbol[30];
	int i=0;
	if(ptree->temphead->children[0] == NULL)
	{	//printf("%s %d %s\n",ptree->temphead->info.tokenName,checkifremaining(ptree),ptree->temphead->info.tokenName);
		if(!strcmp(ptree->temphead->info.tokenName,"TK_END") && !checkifremaining(ptree))
		{
			//printf("1. %s\n",ptree->temphead->info.tokenName);			
			return ;
		}
		if(ptree->temphead->parent->numch > 1)
		{
			//printf("2. %s\n",ptree->temphead->info.tokenName);
			adjustposition(ptree);
			//printf("3. %s\n",ptree->temphead->info.tokenName);
			shiftup(ptree);
		}
		else
		{
			while(ptree->temphead->parent->numch <= 1)
			{
				strcpy(parentNodeSymbol,ptree->temphead->parent->info.parentNodeSymbol);		
				ptree->temphead->parent->info = ptree->temphead->info;
				strcpy(ptree->temphead->parent->info.parentNodeSymbol,parentNodeSymbol);
				ptree->temphead->parent->numch = 0;
				ptree->temphead->parent->children[0] = NULL;
				curr = ptree->temphead ; 
				ptree->temphead = ptree->temphead->parent;
				free(curr);
			}
		}
		//printf("5. %s\n",ptree->temphead->info.tokenName);
		if(strcmp(ptree->temphead->info.tokenName,"TK_END") || checkifremaining(ptree))
		adjustposition(ptree);
		//printf("6. %s\n",ptree->temphead->info.tokenName);
		shiftup(ptree);
	}
	else
	{	
		if(strcmp(ptree->temphead->info.tokenName,"<program>"))
		{
			if(((ptree->temphead->parent->numch) == 1) && ((ptree->temphead->numch) > 1))
			{	
				while((ptree->temphead->parent->numch) == 1)
				{
					if(!strcmp(ptree->temphead->parent->info.tokenName,"<program>"))
					{
						strcpy(parentNodeSymbol,ptree->temphead->parent->info.parentNodeSymbol);		
						ptree->temphead->parent->info = ptree->temphead->info;
						strcpy(ptree->temphead->parent->info.parentNodeSymbol,parentNodeSymbol);
						ptree->temphead->parent->numch = ptree->temphead->numch;
					
						for(i=0;i<(ptree->temphead->numch);i++)
						{	
							ptree->temphead->parent->children[i] = ptree->temphead->children[i];
							ptree->temphead->children[i]->parent = ptree->temphead->parent;
						}
						curr = ptree->temphead;
						ptree->temphead = ptree->temphead->parent;
						free(curr);
						break;						
					}
					else
					{
						strcpy(parentNodeSymbol,ptree->temphead->parent->info.parentNodeSymbol);		
						ptree->temphead->parent->info = ptree->temphead->info;
						strcpy(ptree->temphead->parent->info.parentNodeSymbol,parentNodeSymbol);
						ptree->temphead->parent->numch = ptree->temphead->numch;
						
						for(i=0;i<(ptree->temphead->numch);i++)
						{	
							ptree->temphead->parent->children[i] = ptree->temphead->children[i];
							ptree->temphead->children[i]->parent = ptree->temphead->parent;
						}
						curr = ptree->temphead;
						ptree->temphead = ptree->temphead->parent;
						free(curr);
					}
				}
			}
		}
		//printf("4. %s\n",ptree->temphead->info.tokenName);
		ptree->temphead = ptree->temphead->children[0];	
		shiftup(ptree);
	}
return ;
}

//------------------------------------------------------------

void handlingEpsAndPunc(parseTree ptree)
{
Node curr;
int currentposition;
int i=0;
if(ptree->temphead->children[0] == NULL)
{
	if(!strcmp(ptree->temphead->info.tokenName,"eps") || !strcmp(ptree->temphead->info.tokenName,"TK_SEM") || !strcmp(ptree->temphead->info.tokenName,"TK_COMMA") || !strcmp(ptree->temphead->info.tokenName,"TK_CL") || !strcmp(ptree->temphead->info.tokenName,"TK_OP") || !strcmp(ptree->temphead->info.tokenName,"TK_SQL") || !strcmp(ptree->temphead->info.tokenName,"TK_SQR") || !strcmp(ptree->temphead->info.tokenName,"TK_COLON") || !strcmp(ptree->temphead->info.tokenName,"TK_DOT")  || !strcmp(ptree->temphead->info.tokenName,"TK_PARAMETERS") || !strcmp(ptree->temphead->info.tokenName,"TK_PARAMETER"))
	{
		while(ptree->temphead->parent->numch <= 1)
		{
			curr = ptree->temphead;
			ptree->temphead = ptree->temphead->parent;
			free(curr);
		}
		currentposition = ptree->temphead->positioninallchildren;
		curr = ptree->temphead;
		ptree->temphead = ptree->temphead->parent;
		free(curr);
		ptree->temphead->numch = ptree->temphead->numch - 1;
		for(i=currentposition ; i<(ptree->temphead->numch) ; i++)	
			ptree->temphead->children[i] = ptree->temphead->children[i+1];
		for(i=currentposition ; i<(ptree->temphead->numch);i++)
			ptree->temphead->children[i]->positioninallchildren = ptree->temphead->children[i]->positioninallchildren - 1;
		handlingEpsAndPunc(ptree);
	}
	else
	{
		if(!strcmp(ptree->temphead->info.tokenName,"TK_END") && !checkifremaining(ptree))return;		
		adjustposition(ptree);
		handlingEpsAndPunc(ptree);
	}
}
else
{
	ptree->temphead = ptree->temphead->children[0];
	handlingEpsAndPunc(ptree);
}
return ;
}

//---------------------------------------------------------------

void handlingAssignment(parseTree ptree)
{
	char parentNodeSymbol[30];
	int i=0,currentposition=0;
	Node curr;
	if(ptree->temphead->children[0] == NULL)
	{
		if(!strcmp(ptree->temphead->info.tokenName,"TK_ASSIGNOP"))
		{
			strcpy(parentNodeSymbol,ptree->temphead->parent->info.parentNodeSymbol);		
			ptree->temphead->parent->info = ptree->temphead->info;
			strcpy(ptree->temphead->parent->info.parentNodeSymbol,parentNodeSymbol);
			ptree->temphead->parent->numch = ptree->temphead->parent->numch - 1;
			currentposition = ptree->temphead->positioninallchildren;
			curr = ptree->temphead;
			ptree->temphead = ptree->temphead->parent;
			free(curr);
			for(i=currentposition;i<(ptree->temphead->numch);i++)
				ptree->temphead->children[i] = ptree->temphead->children[i+1];
			for(i=currentposition ; i<(ptree->temphead->numch);i++)
				ptree->temphead->children[i]->positioninallchildren = ptree->temphead->children[i]->positioninallchildren - 1;
			for(i=0 ; i<(ptree->temphead->numch);i++)
				strcpy(ptree->temphead->children[i]->info.parentNodeSymbol,ptree->temphead->info.lexemeName);
		}
		if(!strcmp(ptree->temphead->info.tokenName,"TK_END") && !checkifremaining(ptree))
			return;	
		adjustposition(ptree);
		handlingAssignment(ptree);
	}	
	else
	{
		ptree->temphead = ptree->temphead->children[0];				
		handlingAssignment(ptree);
	}
return ;
}

//-----------------------------------------------------------


void handlingConditional(parseTree ptree)
{

	char parentNodeSymbol[30];
	int i=0,currentposition=0;
	Node curr;
	if(ptree->temphead->children[0] == NULL)
	{
		if(!strcmp(ptree->temphead->info.tokenName,"TK_LT") || !strcmp(ptree->temphead->info.tokenName,"TK_LE") || !strcmp(ptree->temphead->info.tokenName,"TK_EQ") || !strcmp(ptree->temphead->info.tokenName,"TK_GT") || !strcmp(ptree->temphead->info.tokenName,"TK_GE") || !strcmp(ptree->temphead->info.tokenName,"TK_NE") || !strcmp(ptree->temphead->info.tokenName,"TK_AND") || !strcmp(ptree->temphead->info.tokenName,"TK_OR"))
		{
			strcpy(parentNodeSymbol,ptree->temphead->parent->info.parentNodeSymbol);		
			ptree->temphead->parent->info = ptree->temphead->info;
			strcpy(ptree->temphead->parent->info.parentNodeSymbol,parentNodeSymbol);
			ptree->temphead->parent->numch = ptree->temphead->parent->numch - 1;
			currentposition = ptree->temphead->positioninallchildren;
			curr = ptree->temphead;
			ptree->temphead = ptree->temphead->parent;
			free(curr);
			for(i=currentposition;i<(ptree->temphead->numch);i++)
				ptree->temphead->children[i] = ptree->temphead->children[i+1];
			for(i=currentposition ; i<(ptree->temphead->numch);i++)
				ptree->temphead->children[i]->positioninallchildren = ptree->temphead->children[i]->positioninallchildren - 1;
			for(i=0 ; i<(ptree->temphead->numch);i++)
				strcpy(ptree->temphead->children[i]->info.parentNodeSymbol,ptree->temphead->info.lexemeName);
		}
		if(!strcmp(ptree->temphead->info.tokenName,"TK_END") && checkifremaining(ptree))
			return;	
		adjustposition(ptree);
		handlingConditional(ptree);
	}	
	else
	{
		ptree->temphead = ptree->temphead->children[0];				
		handlingConditional(ptree);
	}
return ;
}

//------------------------------------------------------------

void handlingArithmetic(parseTree ptree)
{
	int currentposition,i,j=0;
	char parentNodeSymbol[30];
	Node curr;
	//printf("%s ha\n",ptree->temphead->info.tokenName);
	if(ptree->temphead->children[0] == NULL)
	{
		if(!strcmp(ptree->temphead->info.tokenName,"TK_PLUS") || !strcmp(ptree->temphead->info.tokenName,"TK_MINUS") || !strcmp(ptree->temphead->info.tokenName,"TK_MUL") || !strcmp(ptree->temphead->info.tokenName,"TK_DIV"))
		{
			//printf("entered TK_MUL\n");		
			strcpy(parentNodeSymbol,ptree->temphead->parent->info.parentNodeSymbol);		
			ptree->temphead->parent->info = ptree->temphead->info;
			strcpy(ptree->temphead->parent->info.parentNodeSymbol,parentNodeSymbol);
			ptree->temphead->parent->numch = ptree->temphead->parent->numch - 1;
			currentposition = ptree->temphead->positioninallchildren;
			curr = ptree->temphead;
			ptree->temphead = ptree->temphead->parent;
			free(curr);
			for(i=currentposition;i<(ptree->temphead->numch);i++)
				ptree->temphead->children[i] = ptree->temphead->children[i+1];
			for(i=currentposition ; i<(ptree->temphead->numch);i++)
				ptree->temphead->children[i]->positioninallchildren = ptree->temphead->children[i]->positioninallchildren - 1;
			for(i=0 ; i<(ptree->temphead->numch);i++)
				strcpy(ptree->temphead->children[i]->info.parentNodeSymbol,ptree->temphead->info.lexemeName);
		}
		if(!strcmp(ptree->temphead->info.tokenName,"TK_END"))
			return;	
		adjustposition(ptree);
		handlingArithmetic(ptree);
	}	
	else
	{	
		ptree->temphead = ptree->temphead->children[0];				
		handlingArithmetic(ptree);
		
	}
return ;
}

//----------------------------------------------------------------------

int checkifremaining(parseTree ptree)
{
	Node curr;
	curr = ptree->temphead;
	while(curr != ptree->head)
	{
		if(((curr->positioninallchildren)+1) <= ((curr->parent->numch)-1))
		return 1;
		else
		curr = curr->parent;
	}
return 0;
}
	
//------------------------------------------------------------------------

void insertnodes(parseTree ptree,char rulearray[][40],int no_of_tokens)
{
	Node curr;
	Node newnode;
	int i=0;
	curr = ptree->temphead;
	nodeinfo ninfo;
	curr->numch = no_of_tokens;
	for(i=0;i<no_of_tokens;i++)
	{
		strcpy(ninfo.lexemeName,rulearray[i]);
		ninfo.lineNo = 0;
		strcpy(ninfo.tokenName,rulearray[i]);
		ninfo.valueIfNumber = 0;
		strcpy(ninfo.parentNodeSymbol,curr->info.tokenName);
		ninfo.IsLeafNode = FALSE;
		strcpy(ninfo.NodeSymbol,rulearray[i]);
		newnode = createNode(ninfo,curr,i);
		curr->children[i] = newnode;
	}
	ptree->temphead = ptree->temphead->children[0];
}

//--------------------------------------------------------------------------

void adjustposition(parseTree ptree)
{
	Node curr;
	curr = ptree->temphead;
	if(((curr->positioninallchildren) + 1) <= ((curr->parent->numch)-1))
		ptree->temphead = curr->parent->children[(curr->positioninallchildren) + 1];
	else
	{
		ptree->temphead = ptree->temphead->parent;
		adjustposition(ptree);
	}
}

//----------------------------------------------------------------------------
		
int tokenize(char *rule,char rulearray[][40])
{
	char string[30];
	int i=0,j=0,k=0;
	for(i=0;i<strlen(rule);i++)
	{
		if(rule[i] != ' ' && i != (strlen(rule)))
			string[k++] = rule[i];
		else
		{
		
			string[k] = '\0';
			strcpy(rulearray[j],string);
			k=0; 
			j++;
		}
	}
	return j;
}

//-------------------------------------------------------------------------------


parseTree initializeptree()
{
	parseTree ptree;
	ptree = (parseTree)malloc(sizeof(parsetree));
	ptree->head = NULL;
	ptree->temphead = NULL;
	ptree->size = 0;
	return ptree;
}

//-------------------------------------------------------------------------------

abstractSyntaxTree initializeAbstractSyntaxTree()
{
	abstractSyntaxTree A;
	A = (abstractSyntaxTree)malloc(sizeof(parsetree));
	A->head = NULL;
	A->temphead = NULL;
	A->size = 0;
	return A;
}

//---------------------------------------------------------------------------------

Node createNode(nodeinfo ninfo, Node parentaddress,int position)
{
	Node n;
	int i;
	n = (Node)malloc(sizeof(pnode));
	n->info = ninfo;
	n->parent = parentaddress;
	for(i=0;i<20;i++)
	n->children[i] = NULL;
	n->numch = 0;
	n->positioninallchildren = position;
	return n;
}

//--------------------------------------------------------------------------------------

int getIndex(char tname[20])
{
   int i;
   char *token[54];
   token[0]="TK_ASSIGNOP";
   token[1]="TK_COMMENT";
   token[2]="TK_FIELDID";
   token[3]="TK_ID";
   token[4]="TK_NUM";
   token[5]="TK_RNUM";
   token[6]="TK_FUNID";
   token[7]="TK_RECORDID";
   token[8]="TK_WITH";
   token[9]="TK_PARAMETERS";
   token[10]="TK_END";
   token[11]="TK_WHILE";
   token[12]="TK_INT";
   token[13]="TK_REAL";
   token[14]="TK_TYPE";
   token[15]="TK_MAIN";
   token[16]="TK_GLOBAL";
   token[17]="TK_PARAMETER";
   token[18]="TK_LIST";
   token[19]="TK_SQL";
   token[20]="TK_SQR";
   token[21]="TK_INPUT";
   token[22]="TK_OUTPUT";
   token[23]="TK_SEM";
   token[24]="TK_COLON";
   token[25]="TK_DOT";
   token[26]="TK_ENDWHILE";
   token[27]="TK_OP";
   token[28]="TK_CL";
   token[29]="TK_IF";
   token[30]="TK_THEN";
   token[31]="TK_ENDIF";
   token[32]="TK_READ";
   token[33]="TK_WRITE";
   token[34]="TK_RETURN";
   token[35]="TK_PLUS";
   token[36]="TK_MINUS";
   token[37]="TK_MUL";
   token[38]="TK_DIV";
   token[39]="TK_CALL";
   token[40]="TK_RECORD";
   token[41]="TK_ENDRECORD";
   token[42]="TK_ELSE";
   token[43]="TK_AND";
   token[44]="TK_OR";
   token[45]="TK_NOT";
   token[46]="TK_LT";
   token[47]="TK_LE";
   token[48]="TK_EQ";
   token[49]="TK_GT";
   token[50]="TK_GE";
   token[51]="TK_NE";
   token[52]="TK_COMMA";
   token[53]="$";
   for(i=0;i<54;i++)
   {
       if(!strcmp(tname,token[i]))
         return i;
   }
       return -1;
}

//---------------------------------------------------------------------------------

int getNTIndex(char ntname[30])
{
	int i;
	char *nonterminals[50];
	nonterminals[0] = "<program>";
	nonterminals[1] = "<mainFunction>";
	nonterminals[2] = "<otherFunctions>";
	nonterminals[3] = "<function>";	
	nonterminals[4] = "<input_par>";
	nonterminals[5] = "<output_par>";
	nonterminals[6] = "<parameter_list>";
	nonterminals[7] = "<dataType>";
	nonterminals[8] = "<primitiveDatatype>";
	nonterminals[9] = "<constructedDatatype>";
	nonterminals[10] = "<remaining_list>";
	nonterminals[11] = "<stmts>";
	nonterminals[12] = "<typeDefinitions>";
	nonterminals[13] = "<typeDefinition>";
	nonterminals[14] = "<fieldDefinitions>";
	nonterminals[15] = "<fieldDefinition>";
	nonterminals[16] = "<moreFields>";
	nonterminals[17] = "<declarations>";
	nonterminals[18] = "<declaration>";
	nonterminals[19] = "<global_or_not>";
	nonterminals[20] = "<otherStmts>";
	nonterminals[21] = "<stmt>";
	nonterminals[22] = "<assignmentStmt>";
	nonterminals[23] = "<singleOrRecId>";
	nonterminals[24] = "<singleA>";
	nonterminals[25] = "<funCallStmt>";
	nonterminals[26] = "<outputParameters>";
	nonterminals[27] = "<inputParameters>";	
	nonterminals[28] = "<iterativeStmt>";
	nonterminals[29] = "<conditionalStmt>";
	nonterminals[30] = "<elsePart>";
	nonterminals[31] = "<ioStmt>";
	nonterminals[32] = "<allVar>";
	nonterminals[33] = "<arithmeticExpression>";
	nonterminals[34] = "<expPrime>";
	nonterminals[35] = "<term>";
	nonterminals[36] = "<termPrime>";
	nonterminals[37] = "<factor>";
	nonterminals[38] = "<highPrecedenceOperators>";
	nonterminals[39] = "<lowPrecedenceOperators>";
	nonterminals[40] = "<all>";
	nonterminals[41] = "<temp>";
	nonterminals[42] = "<booleanExpression>";
	nonterminals[43] = "<var>";
	nonterminals[44] = "<logicalOp>";
	nonterminals[45] = "<relationalOp>";
	nonterminals[46] = "<returnStmt>";
	nonterminals[47] = "<optionalReturn>";
	nonterminals[48] = "<idList>";
	nonterminals[49] = "<more_ids>";
	for(i=0;i<50;i++)
	{
		if(!strcmp(ntname,nonterminals[i]))
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------------------

char *getRule(int index)
{
	char *rule[90];
	int i;
	rule[0] = "<otherFunctions> <mainFunction> ";
	rule[1] = "TK_MAIN <stmts> TK_END ";
	rule[2] = "<function> <otherFunctions> ";
	rule[3] = "eps ";
	rule[4] = "TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END ";
	rule[5] = "TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR ";
	rule[6] = "TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR ";
	rule[7] = "eps ";
	rule[8] = "<dataType> TK_ID <remaining_list> ";
	rule[9] = "<primitiveDatatype> ";
	rule[10] = "<constructedDatatype> ";	
	rule[11] = "TK_INT ";
	rule[12] = "TK_REAL ";
	rule[13] = "TK_RECORD TK_RECORDID ";
	rule[14] = "TK_COMMA <parameter_list> ";
	rule[15] = "eps ";
	rule[16] = "<typeDefinitions> <declarations> <otherStmts> <returnStmt> ";
	rule[17] = "<typeDefinition> <typeDefinitions> ";
	rule[18] = "eps ";
	rule[19] = "TK_RECORD TK_RECORDID <fieldDefinitions> TK_ENDRECORD TK_SEM ";
	rule[20] = "<fieldDefinition> <fieldDefinition> <moreFields> ";
	rule[21] = "TK_TYPE <primitiveDatatype> TK_COLON TK_FIELDID TK_SEM ";
	rule[22] = "<fieldDefinition> <moreFields> ";
	rule[23] = "eps ";
	rule[24] = "<declaration> <declarations> ";
	rule[25] = "eps ";
	rule[26] = "TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM ";
	rule[27] = "TK_COLON TK_GLOBAL ";
	rule[28] = "eps ";
	rule[29] = "<stmt> <otherStmts> ";
	rule[30] = "eps ";
	rule[31] = "<assignmentStmt> ";
	rule[32] = "<iterativeStmt> ";
	rule[33] = "<conditionalStmt> ";
	rule[34] = "<ioStmt> ";
	rule[35] = "<funCallStmt> ";
	rule[36] = "<singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM ";
	rule[37] = "TK_ID <singleA> ";
	rule[38] = "TK_DOT TK_FIELDID ";
	rule[39] = "eps ";
	rule[40] = "<outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM ";
	rule[41] = "TK_SQL <idList> TK_SQR TK_ASSIGNOP ";
	rule[42] = "eps ";
	rule[43] = "TK_SQL <idList> TK_SQR ";
	rule[44] = "TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE ";
	rule[45] = "TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart> ";
	rule[46] = "TK_ELSE <stmt> <otherStmts> TK_ENDIF ";
	rule[47] = "TK_ENDIF ";
	rule[48] = "TK_READ TK_OP <singleOrRecId> TK_CL TK_SEM ";
	rule[49] = "TK_WRITE TK_OP <allVar> TK_CL TK_SEM ";
	rule[50] = "<singleOrRecId> ";
	rule[51] = "TK_RECORDID TK_DOT TK_FIELDID ";
	rule[52] = "<term> <expPrime> ";
	rule[53] = "<lowPrecedenceOperators> <term> <expPrime> ";
	rule[54] = "eps ";
	rule[55] = "<factor> <termPrime> ";
	rule[56] = "<highPrecedenceOperators> <factor> <termPrime> ";
	rule[57] = "eps ";
	rule[58] = "TK_OP <arithmeticExpression> TK_CL ";
	rule[59] = "<all> ";
	rule[60] = "TK_MUL ";
	rule[61] = "TK_DIV ";
	rule[62] = "TK_PLUS ";
	rule[63] = "TK_MINUS ";
	rule[64] = "TK_ID ";
	rule[65] = "TK_NUM ";
	rule[66] = "TK_RNUM ";
	rule[67] = "TK_RECORDID <temp> ";
	rule[68] = "eps ";
	rule[69] = "TK_DOT TK_FIELD ";
	rule[70] = "TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL ";
	rule[71] = "<var> <relationalOp> <var> ";
	rule[72] = "TK_NOT <booleanExpression> ";
	rule[73] = "TK_ID ";
	rule[74] = "TK_NUM ";
	rule[75] = "TK_RNUM ";
	rule[76] = "TK_AND ";
	rule[77] = "TK_OR ";
	rule[78] = "TK_LT ";
	rule[79] = "TK_LE ";
	rule[80] = "TK_EQ ";
	rule[81] = "TK_GT ";
	rule[82] = "TK_GE ";
	rule[83] = "TK_NE ";
	rule[84] = "TK_RETURN <optionalReturn> TK_SEM ";
	rule[85] = "TK_SQL <idList> TK_SQR ";
	rule[86] = "eps ";
	rule[87] = "TK_ID <more_ids> ";
	rule[88] = "TK_COMMA <idList> ";
	rule[89] = "eps ";
	return rule[index];
}

//--------------------------------------------------------------------------------------------


//------------------------------------STACK FUNCTIONS------------------------------------------


Stackhead createstack()
{
	Stackhead s;
	s.top = NULL;
	s.size =0;
	s.error  = FALSE;
	return s;
}

//----------------------------------------------------------------------------------------------

char *top(Stackhead s)

{
	if(s.top!=NULL)
		return s.top->tok;
	else
		return "null";
}

//------------------------------------------------------------------------------------------------

Stackhead push(Stackhead s ,char tok[30])
{
	Stacklink ptr = (Stacklink)malloc(sizeof(Stacknode));
	if(ptr == NULL)
	{
		s.error = TRUE;
		return s;
	}
	else
	{
		strcpy(ptr->tok,tok);
		ptr->next = s.top;
		s.top = ptr;
		s.size++;
		return s;
	}
}

//-------------------------------------------------------------------------------------------------

Stackhead pushstring(Stackhead s,char token[500])
{
	char string[30];
	char pushstring[20][30];
	int i=0,j=0,k=0;
	for(i=0;i<strlen(token);i++)
	{
		if(token[i] != ' ' && i != strlen(token))
			string[k++] = token[i];
		else
		{	
		
			string[k] = '\0';
			strcpy(pushstring[j],string);
			k=0; 
			j++;
		}

	}

	for(i=j-1;i>=0;i--)
	s = push(s,pushstring[i]);
	return s;
}

//----------------------------------------------------------------------------------------------------

Stackhead pop(Stackhead s)
{
	if(s.top == NULL)
		printf("Can't pop. Empty list.\n");
	else
	{
		Stacklink ptr = s.top;
		s.top = s.top->next;
		free(ptr);
		s.size--;
	}
	return s;
}

//-----------------------------------------------------------------------------------------------------

void printstack(Stackhead s)
{
	Stacklink curr= s.top;
	int count = 0;
	while (curr!= NULL)
	{
		count ++;
		printf("%d %s\n",count, curr->tok);
		curr=curr->next;
	}
	if(count ==0)
		printf("Empty Stack!!\n");
	else
		printf("End of Stack\n");
}

//---------------------------------------------XXXXXXXXXX------------------------------------------------
