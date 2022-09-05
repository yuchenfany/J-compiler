#include "assembly.h"

char prologue[] = "STR R7, R6, #-2	;; save return addres\n"
				  "STR R5, R6, #-3	;; save base pointer\n"
				  "ADD R6, R6, #-3\n"
				  "ADD R5, R6, #0\n";

char epilogue[] = "LDR R7, R6, #0\n"
				 "ADD R6, R5, #0	;; pop locals off stack\n"
				 "ADD R6, R6, #3	;; free space for return address, base pointer, and return value\n"
				 "STR R7, R6, #-1	;; store return value\n"
				 "LDR R5, R6, #-3	;; restore base pointer\n"
				 "LDR R7, R6, #-2	;; restore return address\n"
				 "RET\n";

char arithmetic[] = "LDR R0, R6, #0 ;; R0 = a\n"
					"ADD R6, R6, #1\n"
					"LDR R1, R6, #0\n";


//write token to file
int convert_token(Assembler *assembler, token *theToken, FILE * theFile) {

	token_type type = theToken -> type;

	if (type == DEFUN) {
		if (assembler -> function == 1) {
			printf("Cannot declare function within fxn\n");
			return 1;
		} else {
			assembler -> function = 1;
			fprintf(theFile, ".CODE\n");
			fprintf(theFile, ".FALIGN\n");

		}

	} else if (type == IDENT) {
		if (assembler -> function == 1) { //declaring new function
			assembler -> function = 0;
			fprintf(theFile, "%s\n", theToken -> str);
			fprintf(theFile, "%s", prologue);
		} else { //calling function
			fprintf(theFile, "JSR %s\n", theToken -> str);
			fprintf(theFile, "ADD R6, R6, #-1 ; put RV on top of stack\n");
		}
	} else if (type == RETURN) {
		fprintf(theFile, "%s", epilogue); 
		return 0;
	} else if (type == PLUS || type == MINUS || type == MUL || type == DIV || type == MOD) {

		fprintf(theFile, "%s", arithmetic);
		if (type == PLUS) {
			fprintf(theFile, "ADD ");
		} else if (type == MINUS) {
			fprintf(theFile, "SUB ");
		} else if (type == MUL) {
			fprintf(theFile, "MUL ");
		} else if (type == DIV) {
			fprintf(theFile, "DIV ");
		} else {
			fprintf(theFile, "MOD ");
		}

		fprintf(theFile, "R0, R0, R1\n");
		fprintf(theFile, "STR R0, R6, #0\n");
		return 0;
	} else if (type == AND || type == OR || type == NOT) {

		
		if (type == AND) {
			fprintf(theFile, "%s", arithmetic);
			fprintf(theFile, "AND R0, R0, R1\n");
			
		} else if (type == OR) {
			fprintf(theFile, "%s", arithmetic);
			fprintf(theFile, "OR R0, R0, R1\n");
			
		} else if (type == NOT) {
			fprintf(theFile, "LDR R0, R6, #0\n");
			fprintf(theFile, "NOT R0, R0");
		} 

		fprintf(theFile, "STR R0, R6, #0\n");

	} else if (type == LT || type == LE || type == EQ || type == GE || type == GT) {
		fprintf(theFile, "%s", arithmetic);
		fprintf(theFile, "ADD R6, R6, #1\n"); //pop 2nd value off stack
		fprintf(theFile, "CMP R0, R1\n"); 
		
		if (type == LT) { //branch if equal/positive
			fprintf(theFile, "BRzp ");
		} else if (type == LE) { //branch if positive
			fprintf(theFile, "BRp ");

		} else if (type == EQ) {
			fprintf(theFile, "BRnp ");
		} else if (type == GE) {
			fprintf(theFile, "BRn ");
		} else { //GT
			fprintf(theFile, "BRnz ");
		} 

	 } else if (type == IF || type == ELSE || type == ENDIF) {

	 	if (type == IF) { //push matching statement 
	 		
	 		fprintf(theFile, "L%d_branch\n", assembler -> separators); 
	 		assembler -> i ++; //increment pointer to make room on stack
	 		assembler -> stack[assembler -> i] = assembler -> separators;
	 		assembler -> separators ++;

	 	} else if (type == ELSE) { 
	 		int label = assembler -> stack[assembler -> i]; //pop off prev separator
	 		fprintf(theFile, "JMP L%d_branch\n", assembler -> separators); //jump to endif once if block executed
	 		fprintf(theFile, "L%d_branch\n", label); 
	 		assembler -> stack[assembler -> i] = assembler -> separators; 
	 		assembler -> separators ++;
	 	} else { //endif
	 		int label = assembler -> stack[assembler -> i];
	 		fprintf(theFile, "L%d_branch\n", label);
	 		assembler -> i --; //decrement pointer
	 	}


	 } else if (type == DROP || type == DUP || type == SWAP || type == ROT) {

	 	if (type == DROP) {

	 		fprintf(theFile, "ADD R6, R6, #1\n"); 

	 	} else if (type == DUP) { //duplicate
	 		fprintf(theFile, "LDR R0, R6, #0\n");
	 		fprintf(theFile, "ADD R6, R6, #-1\n");
	 		fprintf(theFile, "STR R0, R6, #0\n");

	 	} else if (type == SWAP) {
	 		fprintf(theFile, "LDR R0, R6, #0\n");
	 		fprintf(theFile, "LDR R1, R6, #1\n");
	 		fprintf(theFile, "STR R0, R6, #1\n"); //store top element
	 		fprintf(theFile, "STR R1, R6, #0\n"); //store 2nd element
	 	} else if (type == ROT) {
	 		fprintf(theFile, "LDR R0, R6, #0\n"); //top 
	 		fprintf(theFile, "LDR R1, R6, #1\n"); 
	 		fprintf(theFile, "LDR R2, R6, #2\n");

	 		fprintf(theFile, "STR R1, R6, #0\n"); //new top
	 		fprintf(theFile, "STR R2, R6, #1\n"); //2nd el
	 		fprintf(theFile, "STR R0, R6, #2\n"); //3rd el
	 	}

	 } else if (type == ARG) { //R5 + 2, + (number)

	 	int val = 2 + (theToken -> arg_no); 
	 	fprintf(theFile, "LDR R0, R5, #%d\n", val); 
	 	fprintf(theFile, "ADD R6, R6 #-1\n");
	 	fprintf(theFile, "STR R0, R6, #0\n"); 

	 } else if (type == LITERAL) {

	 	int const_val = (theToken -> literal_value) & 0xFF; //lower 8 digits
	 	int hiconst = ((theToken -> literal_value) >> 8) & 0xFF; //high 8 digits

	 	fprintf(theFile, "CONST R0, #%d\n", const_val);
	 	fprintf(theFile, "HICONST R0, #%d\n", hiconst);
	 	fprintf(theFile, "ADD R6, R6, #-1\n");
	 	fprintf(theFile, "STR R0, R6, #0\n");

	 } else {
	 	fprintf(theFile, "Not implemented\n");

	 }
	
	return 0;

}














































