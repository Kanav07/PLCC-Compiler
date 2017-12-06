#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"lexerDef.h"

// Batch         - 55
// Avinav Singla - 2010A7PS125P
// Kanav         - 2010A7PS152P


FILE *fileEdit(char *filename){
	FILE *fp,*fp1;
	fp=fopen(filename,"r");
	fp1=fopen("testcopy.txt","w");
	char x;
	while(x!=-1){
		x=getc(fp);
		if(x!=-1)
		fprintf(fp1, "%c",x );

	}
	fprintf(fp1, "%c",'\n');
	fprintf(fp1, "%c",'$');
	rewind(fp1);
	x=getc(fp1);
	while(x!=-1){
		//printf("%c",x );
		x=getc(fp1);
	}
	rewind(fp1);
	fclose(fp1);
	fclose(fp);
	return fp1;
}
int isAllowed(char c){
	if((c>=65 && c<=90) || (c>=97 && c<=122) || (c>=48 && c<=57) || (c==' ') || (c=='\t') || (c=='\n')
	 || c=='<' || c=='-' || c=='=' || c=='%' || c=='.' || c=='_' || c=='#' || c=='[' || c==']' || c==';' 
	 || c==':' || c=='(' || c==')' || c=='+' ||c=='*' || c=='/' || c=='&' || c=='@' || c=='~' || c=='>' || c=='!' || c==',' || c==0 )
	return 1;
	else
	return 0;
}

char *kewordsFieldId(char temp[]){
	int i=0;
	for(i=0;i<25;i++){
		if(!strcmp(lexemes[i],temp)){
			return tokens[i];
		}
	}
	return "TK_FIELDID";
}

FILE *getStream(FILE *fp, buffer B, buffersize k)
{
		char c=getc(fp);
		if(c==EOF){ 
		return fp;
		 }
		 else{
		 	ungetc(c,fp);
	B=fgets(B,k,fp);
	return fp;}
}
TokenList initialize()
{
	TokenList pointer;
	pointer.head=NULL;
	return pointer;
}
//This function adds a newnode token to the TokenList
link addToList(tokenInfo t, link head){
	link curr,newnode;
	newnode=(link)malloc(sizeof(tokenNode));
	strcpy(newnode->t.tokenName,t.tokenName);
	strcpy(newnode->t.lexemeName,t.lexemeName);
	newnode->t.lineno=t.lineno;
	newnode->next=NULL;
	if(head==NULL)
	{
		head=newnode;
	}
	else{
		curr=head;	
		while(curr->next!=NULL)
			curr=curr->next;
		curr->next=newnode;
	}
	// PRINTING THE LIST
	curr=head;
	/*printf("\nTOKEN ADDED SUCCESSFULLY\n");
	while(curr!=NULL){
		printf("Token name-%s 	Lexeme Name-%s 	 Line Number-%d\n",curr->t.tokenName,curr->t.lexemeName, curr->t.lineno );
		curr=curr->next;
	}*/
	return head;
}
tokenInfo getNextToken( FILE *fp, link *head)
{
	int flag=1;
	while(flag){
		if ((*head)!=NULL) {
			//list is not empty
			//return a token
			if (!strcmp((*head)->t.tokenName,"END"))
			{
				exit(1);
			}
			flag=0;
			link temp;
			temp=*head;
			*head=(*head)->next;
			//printf("Success\n");
			tokenInfo x;
			x=temp->t;
			temp->next=NULL;
			free(temp); 	//delete the returned token and shift list header right
			return x;		
		}
		else{
			flag=0;
			int state;
			static int lineNo=0;
			int cursor=0;
			int errorPrint=0;
			char ch;
			buffer stream;
			buffersize size=100;
			tokenInfo t;
			stream=(buffer)malloc(size*sizeof(char));
			if (stream==NULL)
			{
				printf("Buffer allocation failed\n");
			}
			fp=getStream(fp,stream,size);
			if (feof(fp))
			{
				tokenInfo t;
				strcpy(t.tokenName,"END");
				strcpy(t.lexemeName,"$");
				t.lineno = lineNo;
				return t;
			}
			int i=0;
	/*		while(stream[i]!=0)
				printf("stream[%d]--%c\n",i++,stream[i] );*/
			state=0;
			lineNo++;
			while( stream[cursor]!='$'  && cursor!=100 && stream[cursor]!=0){	//stream[cursor]!='$'
				ch=stream[cursor];
					//printf("%c",stream[cursor]);
					//errorPrint=0;
					switch(state){
						case 0 :
							if(ch=='['){
								strcpy(t.tokenName,"TK_SQL");
								strcpy(t.lexemeName,"[");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch==']'){
								strcpy(t.tokenName,"TK_SQR");
								strcpy(t.lexemeName,"]");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch==';'){
								strcpy(t.tokenName,"TK_SEM");
								strcpy(t.lexemeName,";");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch==':'){
								strcpy(t.tokenName,"TK_COLON");
								strcpy(t.lexemeName,":");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='.'){
								strcpy(t.tokenName,"TK_DOT");
								strcpy(t.lexemeName,".");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='('){
								strcpy(t.tokenName,"TK_OP");
								strcpy(t.lexemeName,"(");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch==')'){
								strcpy(t.tokenName,"TK_CL");
								strcpy(t.lexemeName,")");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='+'){
								strcpy(t.tokenName,"TK_PLUS");
								strcpy(t.lexemeName,"+");
								t.lineno = lineNo;
								cursor++;
								;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='-'){
								strcpy(t.tokenName,"TK_MINUS");
								strcpy(t.lexemeName,"-");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='*'){
								strcpy(t.tokenName,"TK_MUL");
								strcpy(t.lexemeName,"*");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='/'){
								strcpy(t.tokenName,"TK_DIV");
								strcpy(t.lexemeName,"/");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='%'){
								strcpy(t.tokenName,"TK_COMMENT");
								strcpy(t.lexemeName,"%");
								t.lineno = lineNo;
								while(stream[cursor]!='\0')
									cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}	
							else if(ch==','){
								strcpy(t.tokenName,"TK_COMMA");
								strcpy(t.lexemeName,",");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='~'){
								strcpy(t.tokenName,"TK_NOT");
								strcpy(t.lexemeName,"~");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='&'){
								cursor++;
								state=1;
							}
							else if(ch=='@'){
								cursor++;
								state = 3;
							}
							else if(ch=='='){
								cursor++;
								state = 5;
							}
							else if(ch=='>'){
								cursor++;
								state = 6;
							}
							else if(ch=='<'){
								cursor++;
								state = 7;
							}
							else if(ch=='!'){
								cursor++;
								state = 10;
							}
							else if(ch=='#'){
								cursor++;
								state = 11;
							}
							else if(ch=='_'){
								cursor++;
								strcpy(t.lexemeName,"_");
								state = 13;
							}
							else if(ch >='0' && ch <= '9'){
								cursor++;
								strcpy(t.lexemeName,"\0");
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								state = 20;
							}
							else if(ch=='b' || ch=='d'){
								cursor++;
								strcpy(t.lexemeName,"\0");
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								state=24;
							}
							else if(ch=='c'){
								cursor++;
								
								strcpy(t.lexemeName,"c");
								state=27;
							}
							else if((ch>= 'a' && ch<='z') && ch!='b'&& ch!='c' && ch!='d'){
								cursor++;
								strcpy(t.lexemeName,"\0");
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								state=28;
							}
							else if(ch=='\n'){
								fp=getStream(fp,stream,size);
								cursor=0;
								ch=stream[cursor];
								state=0;
								lineNo++;
								//continue;
							}
							else if(ch==' ' || ch=='\t'){
								cursor++;
								state=0;
								flag=1;
							}

							else if(!isAllowed(ch)){
									strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"<");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR 2 - Unknowm symbol  %c at line number %d\n", ch,lineNo);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 1:
							if (ch=='&')	
							{
								cursor++;
								state=2;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"&");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									state=0;
									cursor++;
									flag=3;
							}
							break;
						case 2:
							if (ch=='&')	
							{
								strcpy(t.tokenName,"TK_AND");
								strcpy(t.lexemeName,"&&&");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"&&");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 3:
							if (ch=='@')	{
								cursor++;
								state=4;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"@");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 4:
							if (ch=='@')	
							{
								strcpy(t.tokenName,"TK_OR");
								strcpy(t.lexemeName,"@@@");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"@@");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 5:
							if (ch=='=')	
							{
								strcpy(t.tokenName,"TK_EQ");
								strcpy(t.lexemeName,"==");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"=");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 6:
							if (ch=='=')	
							{
								strcpy(t.tokenName,"TK_GE");
								strcpy(t.lexemeName,">=");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else{
								strcpy(t.tokenName,"TK_GT");
								strcpy(t.lexemeName,">");
								t.lineno = lineNo;
								*head=addToList(t,*head);
								state=0;
							}
							break;
						case 7:
							if (ch=='=')	
							{
								strcpy(t.tokenName,"TK_LE");
								strcpy(t.lexemeName,"<=");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else if(ch=='-'){
								cursor++;
								state=8;
							}
							else {
								strcpy(t.tokenName,"TK_LT");
								strcpy(t.lexemeName,"<");
								t.lineno = lineNo;
								*head=addToList(t,*head);
								state=0;
								flag=1;
								}	
							break;
						case 8:
							if(ch=='-'){
								cursor++;
								state=9;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"<-");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 9:
							if (ch=='-')	
							{
								strcpy(t.tokenName,"TK_ASSIGNOP");
								strcpy(t.lexemeName,"<---");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"<--");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 10:
							if (ch=='=')	
							{
								strcpy(t.tokenName,"TK_NE");
								strcpy(t.lexemeName,"!=");
								t.lineno = lineNo;
								cursor++;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"!");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s at lineNo %d\n", t.lexemeName,t.lineno);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 11:
							if (ch>= 'a' && ch<='z')	
							{
								strcpy(t.lexemeName,"#");
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=12;
							}
							else{
								strcpy(t.tokenName,"TK_ERROR");
									strcpy(t.lexemeName,"#");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 12:
							if (ch>= 'a' && ch<='z')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=12;
							}
							else{
								strcpy(t.tokenName,"TK_RECORDID");
								t.lineno = lineNo;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							break;
						case 13:
							if (((ch>= 'a' && ch<='z')||(ch>='A' && ch<='Z'))&& ch!='m')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								if(strlen(t.lexemeName)>20 && !errorPrint){
									strcpy(t.tokenName,"TK_ERROR");
									printf("ERROR_1 : Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNo);
									errorPrint=1;
								}
								cursor++;
								state=14;
							}
							else if (ch=='m')
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=16;	
							}
							else{
									strcpy(t.lexemeName,"_");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 14:
							if ((ch>= 'a' && ch<='z')||(ch>='A' && ch<='Z'))	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								if(strlen(t.lexemeName)>20 && !errorPrint){
									printf("ERROR_1 : Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNo);
									errorPrint=1;
								}
								cursor++;								
								state=14;
							}
							else if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=15;
							}
							else{
								strcpy(t.tokenName,"TK_FUNID");
								t.lineno = lineNo;
								errorPrint=0;
								*head=addToList(t,*head);
								state=0;
							}
							break;
						case 15:
							if(ch >='0' && ch <= '9'){
								
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;	
								if(strlen(t.lexemeName)>20 && !errorPrint){
									printf("ERROR_1 : Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNo);
									errorPrint=1;
								}							
								cursor++;								
								state=15;
							}
							else{
								strcpy(t.tokenName,"TK_FUNID");
								t.lineno = lineNo;
								errorPrint=0;
								*head=addToList(t,*head);
								state=0;
							}
							break;
						case 16:
							if(ch =='a'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=17;
							}
							else if ((ch>= 'a' && ch<='z')||(ch>='A' && ch<='Z')&& ch!='a')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=14;
							}
							else if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=15;
							}
							else{
									strcpy(t.lexemeName,"_m");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 17:
							if(ch =='i'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=18;
							}
							else if ((ch>= 'a' && ch<='z')||(ch>='A' && ch<='Z')&& ch!='i')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=14;
							}
							else if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=15;
							}
							else{
									strcpy(t.lexemeName,"_ma");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 18:
							if(ch =='n'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=19;
							}
							else if ((ch>= 'a' && ch<='z')||(ch>='A' && ch<='Z')&& ch!='n')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=14;
							}
							else if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=15;
							}
							else{
									strcpy(t.lexemeName,"_mai");
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 19:
							if ((ch>= 'a' && ch<='z')||(ch>='A' && ch<='Z'))	{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=14;
							}
							else if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=15;
							}
							else{
								strcpy(t.tokenName,"TK_MAIN");
								strcpy(t.lexemeName,"_main");
								t.lineno = lineNo;
								*head=addToList(t,*head);
								state=0;
							}
							break;
						case 20:
							if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=20;
							}
							else if(ch =='.' ){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=21;
							}
							else{
								strcpy(t.tokenName,"TK_NUM");
								t.lineno = lineNo;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							break;
						case 21:
							if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=22;
							}
							else{
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 22:
							if(ch >='0' && ch <= '9'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=23;
							}
							else{
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 23:
							strcpy(t.tokenName,"TK_RNUM");
							t.lineno = lineNo;
							*head=addToList(t,*head);
							state=0;
							flag=1;
							break;
						case 24:
							if(ch >='2' && ch <= '7'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=25;
							}
							else if (ch>= 'a' && ch<='z')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=28;
							}
							else{
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 25:
							if(ch >='b' && ch <= 'd'){
								if(strlen(t.lexemeName)>20 && !errorPrint){
									printf("ERROR_1 : Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNo);
									errorPrint=1;
								}
								else{
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;
									t.lineno = lineNo;
								}
								cursor++;
								state=25;
							}
							else if (ch>= '2' && ch<='7')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								if(strlen(t.lexemeName)>20 && !errorPrint){
									printf("ERROR_1 : Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNo);
									errorPrint=1;
								}
								cursor++;
								state=26;
							}
							else{
								strcpy(t.tokenName,"TK_ID");
								t.lineno = lineNo;
								errorPrint=0;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							break;
						case 26:
							if (ch>= '2' && ch<='7')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								if((strlen(t.lexemeName)) > 20 && !errorPrint){
									printf("ERROR_1 : Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNo);
									errorPrint=1;
								}
								cursor++;
								state=26;
							}
							else{
								strcpy(t.tokenName,"TK_ID");
								t.lineno = lineNo;
								errorPrint=0;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							break;
						case 27:
							if(ch >='2' && ch <= '7'){
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								t.lineno = lineNo;
								cursor++;
								state=25;
							}
							else if (ch>= 'a' && ch<='z')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=28;
							}
							else{
									t.lexemeName[strlen(t.lexemeName)+1]='\0';
									t.lexemeName[strlen(t.lexemeName)]=ch;	
									t.lineno=lineNo;
									printf("ERROR_3: Unknown pattern %s\n", t.lexemeName);
									cursor++;
									state=0;
									flag=3;
							}
							break;
						case 28:
							if (ch>= 'a' && ch<='z')	
							{
								t.lexemeName[strlen(t.lexemeName)+1]='\0';
								t.lexemeName[strlen(t.lexemeName)]=ch;
								cursor++;
								state=28;
							}
							else{
								//call function for deciding keywords and fieldid.
								strcpy(t.tokenName,kewordsFieldId(t.lexemeName));
								t.lineno = lineNo;
								*head=addToList(t,*head);
								state=0;
								flag=1;
							}
							break;
						default : printf("Error file\n");
							break;
					}
				}

		}

	}
}


