/*
 * token.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 200


typedef enum { DEFUN, IDENT, RETURN, 
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
  int comment;
} token;


int read_token (token *theToken, FILE *theFile);

// Extra functions which you may wish to define and use , or not


void print_token (token *theToken);

