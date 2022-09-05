#include "token.h"


/*
#define MAX_TOKEN_LENGTH 250

#include <stdio.h>

typedef enum { DEFUN, IDENT, RETURN, //ident = name of defun or name of function to execute
	       PLUS, MINUS, MUL, DIV, MOD, 
	       AND, OR, NOT, 
	       LT, LE, EQ, GE, GT,
	       IF, ELSE, ENDIF, 
	       DROP, DUP, SWAP, ROT,
	       ARG,
	       LITERAL, 
	       BAD_TOKEN } token_type; 

typedef struct {
  token_type type;
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH];  // This character array stores the token string
} token;

/ line_number : You can use this to keep track of the current line number which can be useful in reporting syntax errors.
// Be sure to include a line that says 'int line_number;' in token.c outside of any functions if you would l/ike to make use of this.

extern int line_number

*/

const char* type_string[] = {
		  "defun", "", "return", //0-2
		  "+", "-", "*", "/", "%", //3-7
		  "and","or", "not", //8-10
		  "lt", "le", "eq", "ge", "gt", //11-15
	      "if", "else", "endif", //16-18
	      "drop", "dup", "swap", "rot", //19-22
	      "arg%d",  //23
	      "%d"};  //24 (literal)


//return 0 if successful, nonzero if not

int read_token (token *theToken, FILE *theFile) {

	printf("Token: %s\n", theToken -> str);

	for (int pos = 0; pos < MAX_TOKEN_LENGTH; pos++) {
		if ((theToken -> str)[pos] == ';') {
			theToken -> comment = 1;
			return 0;
		} else if (theToken -> str[pos] == '\0') {
			break;
		}
	}

	theToken -> type = -1;

	for (int pos = 0; pos < 24; pos++) {
		if (strcmp(theToken -> str, type_string[pos]) == 0) {
			theToken -> type = pos;
		}
	}

	if (theToken -> type == -1) {
		if (sscanf(theToken->str, "arg%d", &(theToken->arg_no)) == 1) {
			theToken -> type = ARG;
		} else if (sscanf(theToken->str, "0x%X", &(theToken->literal_value)) == 1) {
			theToken -> type = LITERAL;
			printf("%d", theToken->literal_value);
		} else if (sscanf(theToken->str, "%d", &(theToken->literal_value)) == 1) {
			theToken -> type = LITERAL;
			printf("%d", theToken->literal_value);
		} else {
			theToken -> type = IDENT;
		}
	}
	printf("Token type: %d\n", theToken -> type);

	return 0;

}


// Extra functions which you may wish to define and use , or not

void print_token (token *theToken) {

}



















