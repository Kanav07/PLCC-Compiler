#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include"symbolTable.h"

int OFFSET = 0;

int hash(char s[],int size)
{
	int sum=0,i=0;
	for(i=0;s[i]!='\0';i++)
		sum = sum + toascii(s[i]);
	return sum%size;
}

SPointer createSymbolTable(int functionsize,int variablesize,int recordsize)
{
	SPointer symbolTablePointer;
	FPointer functionBucketsPointer;
	VPointer variableBucketsPointer;
	RPointer recordPointer;
	int i=0,j=0,k=0;
	symbolTablePointer = (SPointer)malloc(sizeof(symbolTable));
	functionBucketsPointer = (FPointer)malloc(functionsize*sizeof(functionBuckets));
	symbolTablePointer->f = functionBucketsPointer;
	symbolTablePointer->size = functionsize;
	for(i=0;i<functionsize;i++)
	{
		variableBucketsPointer = (VPointer)malloc(variablesize*sizeof(variableBuckets));
       		(functionBucketsPointer + i)->v = variableBucketsPointer;
		(functionBucketsPointer + i)->size = variablesize;
		(functionBucketsPointer + i)->s = 0;
		for(j=0;j<variablesize;j++)
		{	recordPointer = (RPointer)malloc(recordsize*sizeof(recordBuckets));
			(variableBucketsPointer+j)->size = recordsize;
			(variableBucketsPointer+j)->s = 0;
			(variableBucketsPointer+j)->r = recordPointer;
			(variableBucketsPointer+j)->offset=0;
			(variableBucketsPointer+j)->inparameters = 0;
			(variableBucketsPointer+j)->outparameters = 0;
			for(k=0;k<recordsize;k++)
			{
				(recordPointer+k)->s=0;
				(recordPointer+k)->offset = 0;
			}
		}
	}
	return symbolTablePointer;
}

FPointer insertFunctionBuckets(SPointer s,char funcname[])
{
	int key,size = s->size,j=0,bp_temp=0,bp=0,i=0;
	FPointer f;
	f = s->f; 
	for(i=0;i<size;i++)
	{
		if(!strcmp((f+i)->functionName,funcname))
		{
			printf("SEMANTIC ERROR : Function %s overloading not allowed\n",funcname);
			exit(1);
		}
	}	
	key = hash(funcname,size);
	//printf("fkey = %d\n",key);
	if((f+key)->s == 0)
	{
		(f+key)->s = 1;
		strcpy(((f+key)->functionName),funcname);
		return (f+key);
	}
	do
	{
		j++;	
		bp_temp = key + pow(j,2);
		bp = bp_temp%size;
	}
	while((f+bp)->s != 0 && j!=size);
	if((f+bp)->s == 0)
	{  
    		(f+bp)->s = 1;
		strcpy(((f+bp)->functionName),funcname);
		return (f+bp);
	}
	else
	{
		printf("functionname cannot be inserted\n");
		return ;
	}
}

VPointer insertVariableBuckets(FPointer fglobal,FPointer fp,nodeinfo TInfo,char T[],int off)
{
	int key,size = fp->size,j=0,bp_temp=0,bp=0,i=0;
	char hashablekey[20];
	VPointer vp;
	VPointer vglobal;
	vp = fp->v;
	vglobal = fglobal->v;
	for(i=0;i<size;i++)
	{
		if(!strcmp((vp+i)->tInfo.lexemeName,TInfo.lexemeName) && !strcmp(T,"UNKNOWN"))
		{
			return (vp+i);
		}
		else if(!strcmp((vp+i)->tInfo.lexemeName,TInfo.lexemeName) && strcmp(T,"UNKNOWN"))
		{
			printf("SEMATIC ERROR : Variable %s declared twice in the same scope at line no %d\n",TInfo.lexemeName,TInfo.lineNo);
			exit(1);
		}
		
	}
	for(i=0;i<size;i++)
	{
		if(!strcmp((vglobal+i)->tInfo.lexemeName,TInfo.lexemeName) && !strcmp(T,"UNKNOWN"))
		{
			//printf("variable already added\n");
			return (vp+i);
		}
		else if(!strcmp((vglobal+i)->tInfo.lexemeName,TInfo.lexemeName) && strcmp(T,"UNKNOWN"))
		{
			printf("SEMANTIC ERROR : Global Variable %s cannot be redfined at lineno %d\n",TInfo.lexemeName,TInfo.lineNo);
			exit(1);
		}
	}
	if(!strcmp(T,"UNKNOWN"))
	{
		printf("SEMANTIC ERROR : %s must be declared before its use at lineno %d\n",TInfo.lexemeName,TInfo.lineNo);
		exit(1);
	}
	strcpy(hashablekey,TInfo.lexemeName);
	key = hash(hashablekey,size);
	//printf("vkey = %d\n",key);
	if((vp+key)->s == 0)
	{
		(vp+key)->s = 1;
		(vp+key)->tInfo = TInfo;
		strcpy((vp+key)->t,T);
		(vp+key)->offset = off;
		return (vp+key);
	}
	do
	{
		j++;	
		bp_temp = key + pow(j,2);
		bp = bp_temp%size;
	}
	while((vp+bp)->s != 0 && j!=size);
	//printf("modifiedvkey = %d\n",bp);
	if((vp+bp)->s == 0)
	{  
    		(vp+bp)->s = 1;
		(vp+bp)->tInfo = TInfo;
		strcpy((vp+bp)->t,T);
		(vp+bp)->offset = off;
		return (vp+bp);
	}
	else
	{
		printf("variablename cannot be inserted\n");
		return ;
	}
}

void insertRecordBuckets(VPointer vp,nodeinfo TInfo,char T[],int off)
{
	RPointer rp;
	int key,size = vp->size,bp_temp=0,bp=0,i=0,j=0;
	char hashablekey[20];
	rp = vp->r;
	for(i=0;i<size;i++)
	{
		if(!strcmp((rp+i)->tInfo.lexemeName,TInfo.lexemeName))
		{
			printf("record already defined\n");
			return ;
		}
		
	}
	strcpy(hashablekey,TInfo.lexemeName);
	key = hash(hashablekey,size);
	//printf("vkey = %d\n",key);
	if((rp+key)->s == 0)
	{
		(rp+key)->s = 1;
		(rp+key)->tInfo = TInfo;
		strcpy((rp+key)->t,T);
		(rp+key)->offset = off;
		return ;
	}
	do
	{
		j++;	
		bp_temp = key + pow(j,2);
		bp = bp_temp%size;
	}
	while((rp+bp)->s != 0 && j!=size);
	//printf("modifiedvkey = %d\n",bp);
	if((rp+bp)->s == 0)
	{  
    		(rp+bp)->s = 1;
		(rp+bp)->tInfo = TInfo;
		strcpy((rp+bp)->t,T);
		(rp+bp)->offset = off;
		return ;
	}
	else
	{
		printf("variablename cannot be inserted\n");
		return ;
	}

}

void printSymbolTable(SPointer sp)
{
	int i,j,k;
	char scope[20];
	FPointer fp = sp->f;
	VPointer vp;
	RPointer rp;
	char recordid[40];
	char fieldid[20];
	for(i=0;i<(sp->size);i++)
	{
		if((fp+i)->s == 1)
		{	
			
			strcpy(scope,((fp+i)->functionName));
			
			for(j=0;j<((fp+i)->size);j++)
			{
				vp = (fp+i)->v;
				if(!strcmp(scope,"global") && !strcmp((vp+j)->t,"RECORD") && (vp+j)->s == 1)
				{	
					printf("%20s%20s%20s%10d%10d\n",(vp+j)->tInfo.lexemeName,scope,(vp+j)->t,(vp+j)->tInfo.lineNo,(vp+j)->offset);
					
					rp = (vp+j)->r;
					//printf("%s\n",recordid);
					for(k=0;k<(vp+j)->size;k++)
					{	
						strcpy(recordid,(vp+j)->tInfo.lexemeName);
						strcat(recordid,".");
						if((rp+k)->s == 1)
						{	
							strcpy(fieldid,(rp+k)->tInfo.lexemeName);
							strcat(recordid,fieldid);
							printf("%20s%20s%20s%10d%10d\n",recordid,scope,(rp+k)->t,(rp+k)->tInfo.lineNo,(rp+k)->offset);
						}
					}
				}
				else if((vp+j)->s == 1)
				{
					printf("%20s%20s%20s%10d%10d\n",(vp+j)->tInfo.lexemeName,scope,(vp+j)->t,(vp+j)->tInfo.lineNo,(vp+j)->offset);
					
				}
			}
		}
	}
}

FPointer fillingGlobalBucket(SPointer s)
{
	FPointer f;
	char global[] = "global";
	f = insertFunctionBuckets(s,global);
	return f;
}


void findingFunctions(SPointer s , FPointer fglobal , abstractSyntaxTree AST)
{
	int condition = 0;
	FPointer f;
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
			if(!strcmp(AST->temphead->info.tokenName,"TK_END") && !checkifremaining(AST))
			{
				//printf("returning\n");
				return ;
			}
			if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		findingFunctions(s,fglobal,AST);
	}
	else
	{	
		AST->temphead = AST->temphead->children[0];
		if(!strcmp((AST->temphead->info.NodeSymbol),"<function>") || !strcmp((AST->temphead->info.NodeSymbol),"<mainFunction>"))
		{
			f = insertFunctionBuckets(s,AST->temphead->children[0]->info.lexemeName);	
			//printf("%s\n",AST->temphead->children[0]->info.lexemeName);
			//printf("%s\n",f->functionName);		
			populateSymbolTable(f,fglobal,AST);
		}
		else if(!strcmp((AST->temphead->parent->info.NodeSymbol),"<function>") || !strcmp((AST->temphead->parent->info.NodeSymbol),"<mainFunction>"))
		{
			AST->temphead = AST->temphead->parent;
			f = insertFunctionBuckets(s,AST->temphead->children[0]->info.lexemeName);			
			populateSymbolTable(f,fglobal,AST);
		}
		findingFunctions(s,fglobal,AST);
	}
}

void addingrecordtoSymbolTable(abstractSyntaxTree AST,VPointer vp)
{
	int condition = 0;
	char type[20];
	nodeinfo TInfo;
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
				
			if(!strcmp("TK_ENDRECORD",AST->temphead->info.tokenName))
				return ;
			if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{	
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		addingrecordtoSymbolTable(AST,vp);
	}
	else
	{
		if(!strcmp("<fieldDefinition>",AST->temphead->info.NodeSymbol))
		{
			
			if(!strcmp("TK_REAL",AST->temphead->children[1]->info.tokenName))
			{
				
				strcpy(type,"REAL");
				TInfo = AST->temphead->children[2]->info;
				insertRecordBuckets(vp,TInfo,type,OFFSET);
			}
			else if(!strcmp("TK_INT",AST->temphead->children[1]->info.tokenName))
			{
				
				strcpy(type,"INT");
				TInfo = AST->temphead->children[2]->info;
				insertRecordBuckets(vp,TInfo,type,OFFSET);
			}
		}
		AST->temphead = AST->temphead->children[0];
		addingrecordtoSymbolTable(AST,vp);
	}

}

int findoffset(FPointer fglobal, char recordid[])
{
	VPointer vp;
	RPointer rp;
	int size = fglobal->size;
	int i=0,j=0;
	int offsize = 0;
	vp = fglobal->v;
	for(i=0;i<size;i++)
	{
		if((vp+i)->s == 1)
		{	
			if(!strcmp(recordid,(vp+i)->tInfo.lexemeName))
			{
				rp = (vp+i)->r;
				for(j=0;j<(vp+i)->size;j++)
				{
					if((rp+j)->s == 1 && !strcmp((rp+j)->t,"INT"))
						offsize +=2;
					if((rp+j)->s == 1 && !strcmp((rp+j)->t,"REAL"))
						offsize +=4;
				}
			}
		}
	}
	return offsize;
}	

void populateSymbolTable(FPointer f,FPointer fglobal,abstractSyntaxTree AST)
{
	int condition = 0;
	int position = 0;
	nodeinfo ninfo;
	char TYPE[20];
	char recordid[20];
	Node temp;
	VPointer vp;

	if(AST->temphead->children[0] == NULL && strcmp(AST->temphead->info.tokenName,"TK_ID") )
	{
		while(!condition)
		{
			if(!strcmp(AST->temphead->info.tokenName,"TK_END"))
				return ;
			else if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		populateSymbolTable(f,fglobal,AST);
		}
	else if(AST->temphead->children[0] == NULL && !strcmp(AST->temphead->info.tokenName,"TK_ID"))
	{
			//printf("onlyname %s\n",AST->temphead->info.lexemeName);
			position = AST->temphead->positioninallchildren;
			ninfo = AST->temphead->info;
			if((position > 0) && !strcmp(AST->temphead->parent->children[position-1]->info.tokenName,"TK_INT"))
			{
				if(((position+1)<(AST->temphead->parent->numch)) && !strcmp(AST->temphead->parent->children[position+1]->info.tokenName,"TK_GLOBAL"))
				{
					OFFSET += 2;
					strcpy(TYPE,"INT");
					insertVariableBuckets(fglobal,fglobal,ninfo,TYPE,OFFSET);
				}
				else
				{
					OFFSET+=2;
					strcpy(TYPE,"INT");
					insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
				}
			}
			else if((position > 0) && !strcmp(AST->temphead->parent->children[position-1]->info.tokenName,"TK_REAL"))
			{
				if(((position+1)<(AST->temphead->parent->numch)) && !strcmp(AST->temphead->parent->children[position+1]->info.tokenName,"TK_GLOBAL"))
				{
					OFFSET+=4;
					strcpy(TYPE,"REAL");
					insertVariableBuckets(fglobal,fglobal,ninfo,TYPE,OFFSET);
				}
				else
				{
					OFFSET +=4;					
					strcpy(TYPE,"REAL");
					insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
				}
			}
			else if((position > 0) && !strcmp(AST->temphead->parent->children[position-1]->info.NodeSymbol,"<constructedDatatype>"))
			{
				strcpy(TYPE,AST->temphead->parent->children[position-1]->children[1]->info.lexemeName);
				strcpy(recordid,AST->temphead->parent->children[position-1]->children[1]->info.lexemeName);
				OFFSET = OFFSET + findoffset(fglobal,recordid);
				insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
			}
			else
			{
				
				strcpy(TYPE,"UNKNOWN");
				insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
			}
			//printf("idname = %s TYPE = %s\n",AST->temphead->info.lexemeName,TYPE);
		while(!condition)
		{
			if(!strcmp(AST->temphead->info.tokenName,"TK_END"))
				return ;
			else if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		populateSymbolTable(f,fglobal,AST);

	}
	else
	{
		AST->temphead = AST->temphead->children[0];
		if(!strcmp("TK_ID",AST->temphead->info.tokenName))
		{	
			//printf("onlyname %s\n",AST->temphead->info.lexemeName);
			position = AST->temphead->positioninallchildren;
			ninfo = AST->temphead->info;
			if((position > 0) && !strcmp(AST->temphead->parent->children[position-1]->info.tokenName,"TK_INT"))
			{
				if(((position+1)<(AST->temphead->parent->numch)) && !strcmp(AST->temphead->parent->children[position+1]->info.tokenName,"TK_GLOBAL"))
				{
					OFFSET+=2;
					strcpy(TYPE,"INT");
					insertVariableBuckets(fglobal,fglobal,ninfo,TYPE,OFFSET);
				}
				else
				{
					OFFSET +=2;
					strcpy(TYPE,"INT");
					insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
				}
			}
			else if((position > 0) && !strcmp(AST->temphead->parent->children[position-1]->info.tokenName,"TK_REAL"))
			{
				if(((position+1)<(AST->temphead->parent->numch)) && !strcmp(AST->temphead->parent->children[position+1]->info.tokenName,"TK_GLOBAL"))
				{
					OFFSET +=4;
					strcpy(TYPE,"REAL");
					insertVariableBuckets(fglobal,fglobal,ninfo,TYPE,OFFSET);
				}
				else
				{
					OFFSET += 4;					
					strcpy(TYPE,"REAL");
					insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
				}
			}
			else if((position > 0) && !strcmp(AST->temphead->parent->children[position-1]->info.NodeSymbol,"<constructedDatatype>"))
			{
				strcpy(TYPE,AST->temphead->parent->children[position-1]->children[1]->info.lexemeName);
				strcpy(recordid,AST->temphead->parent->children[position-1]->children[1]->info.lexemeName);
				OFFSET = OFFSET + findoffset(fglobal,recordid);
				insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
			}
			else
			{
				strcpy(TYPE,"UNKNOWN");
				insertVariableBuckets(fglobal,f,ninfo,TYPE,OFFSET);
			}
			//printf("idname = %s TYPE = %s\n",AST->temphead->info.lexemeName,TYPE);
		}
		if(!strcmp("<typeDefinition>",AST->temphead->info.NodeSymbol))
		{
			temp = AST->temphead;
			strcpy(TYPE,"RECORD");
			ninfo = AST->temphead->children[1]->info;
			vp = insertVariableBuckets(fglobal,fglobal,ninfo,TYPE,OFFSET);
			addingrecordtoSymbolTable(AST,vp);
			AST->temphead = temp;
			while(!condition)
			{
				if(!strcmp(AST->temphead->info.tokenName,"TK_END"))
					return ;
				else if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
				{
					AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
					condition = 1;
				}
				else
					AST->temphead = AST->temphead->parent;
			}
		populateSymbolTable(f,fglobal,AST);	
		}	
		if(!strcmp("TK_ID",AST->temphead->info.tokenName))
		{
			while(!condition)
			{
				if(!strcmp(AST->temphead->info.tokenName,"TK_END"))
					return ;
				else if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
				{
					AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
					condition = 1;
				}
				else
					AST->temphead = AST->temphead->parent;
			}
		populateSymbolTable(f,fglobal,AST);
		}
		populateSymbolTable(f,fglobal,AST);
		
	}
}









