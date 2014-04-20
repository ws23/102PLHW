#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
/* Variable */
int charClass; 
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken; 
int fail; 
FILE *fin;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
void error();

/* Parser declarations */

void expr();
void term();
void factor();
void Exp();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define POINT 2
#define UNKNOWN 99

/* Token codes */
#define NEWLINE 9
#define INT_LIT 10
#define REAL_LIT 11
#define IDENT 12
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define EXP_OP 25
#define LEFT_PAREN 26
#define RIGHT_PAREN 27

/***************************************************************/

int main(){
	fin = fopen("input.txt", "r");
	getChar();
	do{
		fail = 0;

		lex();
		if(nextToken==NEWLINE)
			continue;
		if(nextToken==EOF)
			break;

		expr();

		if(nextToken!=EOF && nextToken!=NEWLINE)
			fail = 1;

		if(fail)
			printf("parse fail\n");
		else
			printf("parse ok\n");
	}while(nextToken!=EOF);

	return 0;
}

/***************************************************************/

/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch){
	switch(ch){
		case '(':
			addChar();
			nextToken = LEFT_PAREN;
			break;
		case ')':
			addChar();
			nextToken = RIGHT_PAREN; 
			break;
		case '+':
			addChar();
			nextToken = ADD_OP;
			break;
		case '-':
			addChar();
			nextToken = SUB_OP;
			break;
		case '*':
			addChar();
			nextToken = MULT_OP;
			break;
		case '/':
			addChar();
			nextToken = DIV_OP;
			break;
		case '^':
			addChar();
			nextToken = EXP_OP;
			break;
		case '\n':
			addChar();
			nextToken = NEWLINE;
			break;
		default: 
			addChar();
			nextToken = EOF; 
			break;
	}
	return nextToken;
}


/* addChar - a function to add nextChar to lexeme */
void addChar(){
	if(lexLen<=98){
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		printf("Error: lexeme is too long!!\n");
}


/* getChar - a function to get the next character of input and determine its character class */
void getChar(){
	if((nextChar = getc(fin))!=EOF){
		if(isalpha(nextChar))
			charClass = LETTER; 
		else if(isdigit(nextChar))
			charClass = DIGIT;
		else if(nextChar=='.')
			charClass = POINT;
		else
			charClass = UNKNOWN; 
	}
	else
		charClass = EOF; 
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank(){
	while(isspace(nextChar) && nextChar!='\n')
		getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int lex(){
	int point = 0;
	lexLen = 0;
	getNonBlank();
	switch(charClass){
		/* Parse identifires */
		case LETTER:
			addChar();
			getChar();
			while(charClass==LETTER||charClass==DIGIT){
				addChar();
				getChar();
			}
			nextToken = IDENT;
			break;

		/* Parse integer literals */
		case DIGIT:
			addChar();
			getChar();
			while(charClass==DIGIT||(charClass==POINT&&point==0)){
				if(charClass==POINT)
					point = 1;
				addChar();
				getChar();
			}
			if(point==1)
				nextToken = REAL_LIT;
			else
				nextToken = INT_LIT;
			break;
		/* Parentheses and operators */
		case UNKNOWN: 
			lookup(nextChar);
			getChar();
			break;
		/* EOF */
		case EOF:
			nextToken = EOF;
			strcpy(lexeme, "EOF");
			break;
	}
	return nextToken;
}


void error(){
	fail = 1;
}

/****************************************************************/
/* <expr> -> <term> { (+|-) <term> } */
void expr(){
	term();
	while(nextToken==ADD_OP||nextToken==SUB_OP){
		lex();
		term();
	}
}

/* <term> -> <factor> { (*|/) <factor> } */
void term(){
	factor();
	while(nextToken==MULT_OP||nextToken==DIV_OP){
		lex();
		factor();
	}
}

/* <factor> -> <exp> { ^ <exp> } */
void factor(){
	Exp();
	while(nextToken==EXP_OP){
		lex();
		Exp();
	}
}

/* <exp> -> id | int_lit | real_lit | (<expr>) */
void Exp(){
	if(nextToken==IDENT||nextToken==INT_LIT||nextToken==REAL_LIT)
		lex();
	else if(nextToken==LEFT_PAREN){
		lex();
		expr();
		if(nextToken==RIGHT_PAREN)
			lex();
		else
			error();
	}
	else
		error();
}
