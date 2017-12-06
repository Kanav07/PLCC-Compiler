//#include"symbolTableDef.h"

#include"symbolTableDef.h"

#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
int LabelNo=0;



void varDec(SPointer T,FILE *f)
{
	//Finds all the variable declaration in the main function and allocates labelled memory loactions to them.
	FPointer fp;
	fp=T->f;
	int i,j,k;
	VPointer vp;
	for(i=0;i<T->size;i++)
	{
		
		if(!strcmp((fp+i)->functionName,"_main"))
		{//accessing only the variables in the main function
			//now do the data declarations for each datatype
			vp=(fp+i)->v;
			for(j=0;j<fp->size;j++)
			{
				
				if(!strcmp((vp+j)->t,"INT"))
				{
					fprintf(f,"%-10s%10s\n",(vp+j)->tInfo.lexemeName,"DW 0H");
				}
				else if(!strcmp((vp+j)->t,"REAL"))
				{
					fprintf(f,"%-10s%10s\n",(vp+j)->tInfo.lexemeName,"real4 0.0");
				}
				else if((vp+j)->s==1)
				{
					fprintf(f,"%-10s%10s%10s\n",(vp+j)->tInfo.lexemeName,(vp+j)->t,"{}");
				}		
			}
		}
		else if(!strcmp((fp+i)->functionName,"global"))
		{
			vp=(fp+i)->v;
			for(j=0;j<fp->size;j++)
			{
				if( !strcmp((vp+j)->t,"RECORD")  ) //last good config--- strcmp((vp+j)->t,"INT") && strcmp((vp+j)->t,"REAL")
				{//assuming unknown type are not stored in the symbol table
					fprintf(f, "%-10s%10s\n",(vp+j)->tInfo.lexemeName,"struct");
					RPointer rp=(vp+j)->r;

					//if(rp==NULL) printf("CODEGEN VARDEC ERROR---RP IS NULL DUDE\n");
					//printf("%s vp->size=%d\n",(vp+j)->t, (vp+j)->size);
					for (k = 0; k < vp->size; k++)
					{
						//printf("%s\n",(rp+k)->t );
						if(!strcmp((rp+k)->t,"INT")){
						fprintf(f,"%-10s%10s\n",(rp+k)->tInfo.lexemeName,"DW 0H");
						}
						else if(!strcmp((rp+k)->t,"REAL")){
						fprintf(f,"%-10s%10s\n",(rp+k)->tInfo.lexemeName,"real4 0.0");
						}	
					}
					fprintf(f, "%-10s%10s\n",(vp+j)->tInfo.lexemeName,"ends");
				}
			}	
		}
	}
	//printf("return check\n");
	fseek(f,0,SEEK_END);
	return ;
}



void readGen(abstractSyntaxTree AST,FILE *f)
{
	

	int condition=0;
	Node temp;
	char LEXEME[100];
	if(!strcmp((AST->temphead->children[1]->info.NodeSymbol),"TK_ID")){
		if (f==NULL)printf("F is null\n");
		strcpy(LEXEME,(AST->temphead->children[1]->info.lexemeName));
		fprintf(f,"\tMOV AH,1\n\tINT 21H\n\tMOV %s,AX\n",LEXEME);
	}
	else{
		AST->temphead=AST->temphead->children[1];
		strcpy(LEXEME,(AST->temphead->children[0]->info.lexemeName));
		strcat(LEXEME,".");
		strcat(LEXEME,(AST->temphead->children[1]->info.lexemeName));
		fprintf(f,"\tMOV AH,1\n\tINT 21H\n\tMOV %s,AX\n",LEXEME);
	}
	return ;
}
void writeGen(abstractSyntaxTree AST,FILE *f)
{
	int condition=0;
	Node temp;
	char LEXEME[100];
	if(!strcmp((AST->temphead->children[1]->info.NodeSymbol),"TK_ID")){
		if (f==NULL)printf("F is null\n");
		strcpy(LEXEME,(AST->temphead->children[1]->info.lexemeName));
		fprintf(f,"\tMOV DX,%s\n\tMOV AH,2\n\tINT 21H\n\n",LEXEME);
	}
	else{
		AST->temphead=AST->temphead->children[1];
		strcpy(LEXEME,(AST->temphead->children[0]->info.lexemeName));
		strcat(LEXEME,".");
		strcat(LEXEME,(AST->temphead->children[1]->info.lexemeName));
		fprintf(f,"\tMOV DX,%s\n\tMOV AH,2\n\tINT 21H\n",LEXEME);
	}
	return ;
}
void arithExpGen(abstractSyntaxTree AST,FILE *f)
{
	//fprintf(f,".CODE\n.STARTUP\n");
	int condition=0;
	Node temp;
	char VARIABLE[50];
	//printf("%s\n",AST->temphead->info.NodeSymbol);
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
			//printf("terminals = %s\n",AST->temphead->info.tokenName);
			if((!strcmp(AST->temphead->info.tokenName,"TK_ID") || !strcmp(AST->temphead->info.tokenName,"TK_NUM") || !strcmp(AST->temphead->info.tokenName,"TK_RNUM")) )
			{
				if(!strcmp(AST->temphead->info.tokenName,"TK_ID"))
				{
					//strcpy(VARIABLE,"[");
					strcpy(VARIABLE,(AST->temphead->info.lexemeName));
					//strcat(VARIABLE,"]");
				}
				else
					strcpy(VARIABLE,(AST->temphead->info.lexemeName));	
				fprintf(f,"\tADD AX,%s\n",VARIABLE );
				//printf("%s\n",VARIABLE );
				if(!((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch))
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
		arithExpGen(AST,f);
	}
	else
	{	
		//printf("non terminals = %s\n",AST->temphead->info.NodeSymbol);
		if(!strcmp((AST->temphead->info.NodeSymbol),"<expPrime>"))
		{	
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
			//your code goes here.....
			temp = AST->temphead;
			if(!strcmp((AST->temphead->children[0]->info.NodeSymbol),"TK_MINUS"))
			{
				if(!strcmp(AST->temphead->children[1]->info.tokenName,"TK_ID"))
				{
					//strcpy(VARIABLE,"[");
					strcpy(VARIABLE,(AST->temphead->children[1]->info.lexemeName));
					//strcat(VARIABLE,"]");
				}
				else if((!strcmp(AST->temphead->children[1]->info.tokenName,"TK_NUM")) || (!strcmp(AST->temphead->children[1]->info.tokenName,"TK_RNUM")) )
					strcpy(VARIABLE,(AST->temphead->children[1]->info.lexemeName));	
				fprintf(f, "\tSUB AX,%s\n",VARIABLE );
				AST->temphead = temp;
				AST->temphead = AST->temphead->children[2];
			}
			else
			{
				AST->temphead = temp;
				AST->temphead = AST->temphead->children[0];
			}
			
		}
		else
		{
			AST->temphead = AST->temphead->children[0];
		}
		arithExpGen(AST,f);
	}
}
void assignGen(abstractSyntaxTree AST,FILE *f)
{
	fprintf(f, "\t%s\n","MOV AX,0H" );
	char RESULT[100]; //accomodates id and recordid
	Node temp;
	temp=AST->temphead;
	if(!strcmp((AST->temphead->children[0]->info.NodeSymbol),"TK_ID")){
		if (f==NULL)printf("F is null\n");
		strcpy(RESULT,(AST->temphead->children[0]->info.lexemeName));
	}
	else{
		AST->temphead=AST->temphead->children[0];
		strcpy(RESULT,(AST->temphead->children[0]->info.lexemeName));
		strcat(RESULT,".");
		strcat(RESULT,(AST->temphead->children[1]->info.lexemeName));
	}
	AST->temphead=temp;
	AST->temphead=AST->temphead->children[1];
	arithExpGen(AST,f);
	fprintf(f, "\tMOV %s,AX\n", RESULT);
}
void relOpGen(char OP[],FILE *f)
{
	LabelNo++;
	if(!strcmp(OP,"TK_LT"))
	{
		fprintf(f, "\tJGE Label%d\n",LabelNo );
	}
	else if(!strcmp(OP,"TK_LE"))
	{
		fprintf(f,"\tJG Label%d\n",LabelNo );
	}
	else if(!strcmp(OP,"TK_EQ"))
	{
		fprintf(f, "\tJNE Label%d\n",LabelNo );
	}
	else if(!strcmp(OP,"TK_NE"))
	{
		fprintf(f, "\tJE Label%d\n",LabelNo );
	}
	else if(!strcmp(OP,"TK_GT"))
	{
		fprintf(f, "\tJLE Label%d\n",LabelNo );
	}	
	else if(!strcmp(OP,"TK_GE"))
	{
		fprintf(f, "\tJL Label%d\n",LabelNo );
	}
}
void boolExprGen(abstractSyntaxTree AST,FILE *f)
{
	//fprintf(f,".CODE\n.STARTUP\n");
	int condition=0;
	Node temp;
	char LHS[50];
	char RHS[50];
	char OP[10];
	//printf("%s\n",AST->temphead->info.NodeSymbol);
	if(AST->temphead->children[0] == NULL)
	{
		//while(!condition)
		//{
			//printf("terminals = %s\n",AST->temphead->info.tokenName);
			
			if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
				boolExprGen(AST,f);
			}
			else
				return;
		//}
		
	}
	else
	{	
		//printf("non terminals = %s\n",AST->temphead->info.NodeSymbol);
		if((!strcmp((AST->temphead->children[0]->info.NodeSymbol),"TK_ID")) || (!strcmp((AST->temphead->children[0]->info.NodeSymbol),"TK_NUM")) || (!strcmp((AST->temphead->children[0]->info.NodeSymbol),"TK_RNUM")))
		{	
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
			//your code goes here.....
			temp = AST->temphead;
			if(!strcmp(AST->temphead->children[0]->info.tokenName,"TK_ID"))
				{
					//strcpy(LHS,"[");
					strcpy(LHS,(AST->temphead->children[0]->info.lexemeName));
					//strcat(LHS,"]");

				}
				else if((!strcmp(AST->temphead->children[0]->info.tokenName,"TK_NUM")) || (!strcmp(AST->temphead->children[0]->info.tokenName,"TK_RNUM")) )
					strcpy(LHS,(AST->temphead->children[0]->info.lexemeName));
				fprintf(f,"\tMOV AX,%s\n",LHS );
				if(!strcmp(AST->temphead->children[2]->info.tokenName,"TK_ID"))
				{
					//strcpy(RHS,"[");
					strcpy(RHS,(AST->temphead->children[2]->info.lexemeName));
					//strcat(RHS,"]");
				}
				else if((!strcmp(AST->temphead->children[2]->info.tokenName,"TK_NUM")) || (!strcmp(AST->temphead->children[2]->info.tokenName,"TK_RNUM")) )
					strcpy(RHS,(AST->temphead->children[2]->info.lexemeName));
				fprintf(f,"\tMOV BX,%s\n",RHS );
				fprintf(f, "\tCMP AX,BX\n" );
				strcpy(OP,AST->temphead->children[1]->info.tokenName);
				//printf("%s\n",OP );
				relOpGen(OP,f);
				
			AST->temphead = temp;
			AST->temphead = AST->temphead->children[0];
		}
		else
		{
			AST->temphead = AST->temphead->children[0];
		}
		boolExprGen(AST,f);
	}
}
void iterateGen(abstractSyntaxTree AST,FILE *f)
{
	int condition=0;
	Node temp;
	//printf("%s\n",AST->temphead->info.NodeSymbol);
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
			//printf("terminals = %s\n",AST->temphead->info.tokenName);
			if(!strcmp(AST->temphead->info.tokenName,"TK_ENDWHILE"))
				return ;
			if(!strcmp(AST->temphead->info.tokenName,"TK_WHILE"))
			{
				fprintf(f, "\tLabel%d: ",++LabelNo );
			}
			/*if(!strcmp(AST->temphead->info.tokenName,"TK_ELSE"))
				fprintf(f, "Label%d: ",LabelNo );*/
			if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		iterateGen(AST,f);
	}
	else
	{	
		//printf("non terminals = %s\n",AST->temphead->info.NodeSymbol);
		if(!strcmp((AST->temphead->info.NodeSymbol),"<booleanExpression>"))
		{	
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
			//your code goes here.....
			temp = AST->temphead;
			boolExprGen(AST,f);
			//printf("hello\n");
			AST->temphead = temp;
			AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
		}
		else
		{
			AST->temphead = AST->temphead->children[0];
		}
		iterateGen(AST,f);
	}
}
void conditionGen(abstractSyntaxTree AST,FILE *f)
{

	int condition=0;
	Node temp;
	//printf("%s\n",AST->temphead->info.NodeSymbol);
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
			//printf("terminals = %s\n",AST->temphead->info.tokenName);
			if(!strcmp(AST->temphead->info.tokenName,"TK_ENDIF")){
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
		conditionGen(AST,f);
	}
	else
	{	
		//printf("non terminals = %s\n",AST->temphead->info.NodeSymbol);
		if(!strcmp((AST->temphead->info.NodeSymbol),"<booleanExpression>"))
		{	
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
			//your code goes here.....
			temp = AST->temphead;
			boolExprGen(AST,f);
			//printf("hello\n");
			AST->temphead = temp;
			AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
		}
		else
		{
			AST->temphead = AST->temphead->children[0];
		}
		conditionGen(AST,f);
	}
}
void statements(abstractSyntaxTree AST,FILE *f)
{
	int condition=0;
	Node temp;
	//printf("%s\n",AST->temphead->info.NodeSymbol);
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
			//printf("terminals = %s\n",AST->temphead->info.tokenName);
			if(!strcmp(AST->temphead->info.tokenName,"TK_END"))
				return ;
			if(!strcmp(AST->temphead->info.tokenName,"TK_ENDIF"))
				fprintf(f, "\tLabel%d:",LabelNo);
			if(!strcmp(AST->temphead->info.tokenName,"TK_ELSE"))
			{
				fprintf(f, "\tJMP Label%d\n",++LabelNo );
				fprintf(f, "\tLabel%d: ",LabelNo-1 );
			}
			if(!strcmp(AST->temphead->info.tokenName,"TK_ENDWHILE"))
			{
				fprintf(f, "\tJMP Label%d\n",LabelNo-1 );
				fprintf(f, "\tLabel%d: ",LabelNo );
			}
			
			if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		statements(AST,f);
	}
	else
	{	
		//printf("non terminals = %s\n",AST->temphead->info.NodeSymbol);
		if(!strcmp((AST->temphead->info.NodeSymbol),"<ioStmt>"))
		{	
			
			temp = AST->temphead;			
			//printf("You have reached here-%s\n", (AST->temphead->info.NodeSymbol));
			//your code goes here.....
			//AST->temphead = AST->temphead->children[0];
			if(!strcmp((AST->temphead->children[0]->info.NodeSymbol),"TK_READ")){
				if(f==NULL)printf("ERROR---File pointer is null\n");
				//printf("You have reached here-%s\n", (AST->temphead->children[0]->info.NodeSymbol));
				readGen(AST,f);
				
				//return f;
			}
			else{
				//printf("You have reached here-%s\n", (AST->temphead->children[0]->info.NodeSymbol));
				writeGen(AST,f);
				

			}
			AST->temphead = temp;
			AST->temphead = AST->temphead->children[0];
		}
		
		else if(!strcmp((AST->temphead->info.NodeSymbol),"TK_ASSIGNOP"))
		{	
			
			temp = AST->temphead;
			//your code goes here.....
			//printf("You have reached here-%s\n", (AST->temphead->info.NodeSymbol));
			assignGen(AST,f);
			AST->temphead = temp;
			AST->temphead = AST->temphead->children[0];
		}
		else if(!strcmp((AST->temphead->info.NodeSymbol),"<iterativeStmt>"))
		{	
			
			temp = AST->temphead;
			//your code goes here.....
			//printf("You have reached here-%s\n", (AST->temphead->info.NodeSymbol));
			iterateGen(AST,f);
			AST->temphead = temp;
			AST->temphead = AST->temphead->children[0];
		}
		
		else if(!strcmp((AST->temphead->info.NodeSymbol),"<conditionalStmt>"))
		{	
			
			temp = AST->temphead;
			//your code goes here.....
			//printf("You have reached here-%s\n", (AST->temphead->info.NodeSymbol));
			conditionGen(AST,f);
			AST->temphead = temp;
			AST->temphead = AST->temphead->children[0];
		}
		else
		{
			AST->temphead = AST->temphead->children[0];
		}
		statements(AST,f);
	}
}

void code(abstractSyntaxTree AST,FILE *f)
{
	//fprintf(f,".CODE\n.STARTUP\n");
	int condition=0;
	Node temp;
	//printf("%s\n",AST->temphead->info.NodeSymbol);
	if(AST->temphead->children[0] == NULL)
	{
		while(!condition)
		{
			//printf("terminals = %s\n",AST->temphead->info.tokenName);
			if(!strcmp(AST->temphead->info.tokenName,"TK_END") && !checkifremaining(AST))
				return ;
			if((AST->temphead->positioninallchildren)+1 < AST->temphead->parent->numch)
			{
				AST->temphead = AST->temphead->parent->children[(AST->temphead->positioninallchildren)+1];
				condition = 1;
			}
			else
				AST->temphead = AST->temphead->parent;
		}
		code(AST,f);
	}
	else
	{	
		//printf("non terminals = %s\n",AST->temphead->info.NodeSymbol);
		if(!strcmp((AST->temphead->info.NodeSymbol),"<mainFunction>"))
		{	
			//printf("in main = %s\n",AST->temphead->info.NodeSymbol);
			//your code goes here.....
			temp = AST->temphead;
			statements(AST,f);
			AST->temphead = temp;
			AST->temphead = AST->temphead->children[0];
		}
		else
		{
			AST->temphead = AST->temphead->children[0];
		}
		code(AST,f);
	}


}





void MASM(abstractSyntaxTree ptree,SPointer sp,FILE *fx)
{
		fprintf(fx,".MODEL TINY\n.STACK\n.DATA\n");
		varDec(sp,fx);
		fseek(fx,0,SEEK_END);
		fprintf(fx,".CODE\nSTART:\n");
		code(ptree,fx);
		fprintf(fx,"END START\n");
		fclose(fx);
}








