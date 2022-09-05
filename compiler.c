#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembly.h"

#define MAX_LINE_LENGTH 500

int main(int argc, char **argv) {

	printf("Running compiler\n");
	char input[MAX_LINE_LENGTH];

	FILE *inputFile;
	inputFile = fopen(argv[1], "r");

	if (inputFile == NULL) {
		printf ("Couldn't open file\n");
	    return 0;
	}

	char output_file[MAX_LINE_LENGTH];

	int ptr = 0;
	while (1) {
		if (argv[1][ptr] != '.') {
			output_file[ptr] = argv[1][ptr];
			ptr++;
		} else {
			break;
		}
	}

	output_file[ptr] = '.';
	output_file[ptr+1] = 'a';
	output_file[ptr+2] = 's';
	output_file[ptr+3] = 'm';
	output_file[ptr+4] = '\0';

	FILE *output;
	output = fopen(output_file, "w");

	if (output == NULL) {
		printf("Couldn't open output file\n");
		return 0;
	}

	Assembler *assembler = (Assembler*) malloc(sizeof(Assembler));
	assembler -> function = 0;
	assembler -> separators = 0;
	assembler -> i = -1;
	for (int num = 0; num < 500; num++) {
		assembler -> stack[num] = 0;
	}

	while (fgets (input, MAX_LINE_LENGTH, inputFile)) {
	    printf ("Line read: %s", input);

	   	char * word;
	    word = strtok (input, " \t");

	    while (word != NULL) {

	    	char string[MAX_TOKEN_LENGTH];
	    	if (sscanf(word, "%s", string) == 1) {



	    		token *curr = (token*) malloc(sizeof(token)); 
	    		curr -> type = -1;
	    		curr -> arg_no = 0;
	    		curr -> literal_value = 0;
		    	curr -> comment = 0;    	
		    	strcpy (curr -> str, string); 
		    	
		    	int retVal = 0;
		    	retVal = read_token(curr, output);

		    	if (retVal != 0) {
		    		printf("Error\n");
		    		free(curr);
		    		return 0;
		    	} else if (curr -> comment == 1) {
		    		free(curr);
		    		break; 
		    	}

		    	convert_token(assembler, curr, output);
		    	free(curr);
	    	}
	    	word = strtok (NULL, " \t");
	    }
	    
  	}

  	free(assembler);

  	fclose(inputFile);
  	fclose(output);

  	return 0;


}













