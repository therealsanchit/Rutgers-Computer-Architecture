/*
 * CS211 CompArch
 * Liu Liu
 * main.c
 */

#include "tokenizer.h"
/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the file name.
 */

int main(int argc, char ** argv){
	/*check the argument*/

	TokenizerT* tokenizer;
	char* fileName = argv[1];


	tokenizer = TKCreate(fileName);
	/*call TKCreate() with the filename*/
	TKPrint(tokenizer);
	
	/*call TKPrint() with the returned object and print out the result + error message*/

	TKDestroy(tokenizer);

	/*destroy all the dynamically allocated memory*/
	dummyFunc();

	return 0;
}
