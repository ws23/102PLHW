#include <stdio.h>
#include <ctype.h>
#include <string.h>

void error();
void expr();
void term();
void factor();
void exp();

typedef struct _phrase{
	char str[100];
} phrase; 

phrase s[100], *token;

int main(){
	int i, j;
	char in[1000], *tok;
	freopen("input.txt", "r", stdin);
	while(gets(in)){
		i = 0;
		tok = strtok(in, " ");
		while(tok != NULL){
			strcpy(s[i].str, tok);
			i++;
			tok = strtok(NULL, " ");	
		}
		for(j=0;j<i;j++)
			printf("phs[%d].str = %s\n", j, s[j].str);
		token = s;
		expr();	
		if(token!=NULL)
			printf("parse ok\n");
		else
			printf("parse fail\n");
		//break;
	}
	return 0;
}

void error(){
	token = NULL;
}

/* <expr> -> <term> { (+|-) <term> }  */
void expr(){
//	printf("<expr> token => %c\n", *token);
/*	
	term();
	while(*token || token!=NULL){
		if(*token=='+' || *token=='-'){
			token++;
			term();
		}
		else
			error();
	}
//*/
	printf("<expr> end; \n"); 
}

/* <term> -> <factor { (*|/) <factor> }> */
void term(){
//	printf("<term> token => %c\n", *token);
/*
	factor();
	while(*token=='*' || *token=='/' || token!=NULL){
			token++;
			factor();
	}
//*/
	printf("<term> end; \n");
}

/* <factor> -> <exp { ^ <exp> }> */
void factor(){
//	printf("<factor> token => %c\n", *token);
/*
	exp();
	while(*token=='^' || token!=NULL){
		token++;
		exp();
	}
//*/
	printf("<factor> end;\n");
}

/* <exp> -> id | int_lit | real_lit | (<expr>) */
void exp(){
//	printf("<exp> token => %c\n", *tok);
/*
	if(*token=='('){
		token++;
		expr();
		if(*token!=')')
			error();
	}
	else if(isdigit(*token)){
		token++;
		while(isdigit(*token))
			token++;
		if(*token=='.'){
			token++;
			if(isdigit(*token))
				token++;
			else
				error();
			while(isdigit(*token))
				token++;
		}
	}
	else if(isalpha(*token))
		token++;	
	else
		error();
//*/		
	printf("<exp> end;\n");
}
