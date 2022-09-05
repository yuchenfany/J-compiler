#include <stdio.h>

#include "token.h"

typedef struct {
  int function; //1 if defun is declared, else 0
  int separators; //number separators (for if/else/endif statements)
  int stack[500]; //stack for if statements
  int i;
} Assembler;


//write token to file
int convert_token(Assembler *assembler, token *theToken, FILE * theFile);

















