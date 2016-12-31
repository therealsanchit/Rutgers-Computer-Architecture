/*
 * CS211 CompArch
 * Liu Liu
 * tokenizer->c
 */

#include "tokenizer.h"
/* change the 'return NULL' after you finished the code */
/* functions specifications are in tokenizer->h */

int i=0;
int* iptr = &i;
void dummyFunc(){
	printf("I succesfully write a makefile\n");

}

TokenizerT *TKCreate(char * ts){
	/* fill in your code here */

		char const* const fileName = ts;


		FILE* file = fopen(fileName,"r");
		FILE* checkFile = fopen(fileName,"r");

		fseek(checkFile, 0, SEEK_END);
    		unsigned long len = (unsigned long)ftell(checkFile);

		if(len==0){
			fclose(checkFile);
			fclose(file);
			printf("%s\n","FILE IS EMPTY");
			return NULL;
		}
		


		
		TokenizerT *tokenizer = (TokenizerT *) malloc(sizeof(TokenizerT));
		tokenizer->numTokens = 0;
		tokenizer->tokenArray = (char**) malloc(sizeof(char*) * (tokenizer->numTokens)); 
		tokenizer->resultArray = (char**) malloc(sizeof(char*) * (tokenizer->numTokens));

		
		char line[50];
		


		while(fgets(line,sizeof(line),file)){


				*iptr =  0;

				if(is_empty(line) == 1)
					continue;

				char* lineptr = line;
				char* token;


				token = TKGetNextToken(lineptr);
				tokenizer->numTokens++;
				tokenizer->tokenArray = (char**) realloc(tokenizer->tokenArray ,sizeof(char*) * (tokenizer->numTokens));
				tokenizer->resultArray = (char**) realloc(tokenizer->resultArray ,sizeof(char*) * (tokenizer->numTokens));
				tokenizer->tokenArray[tokenizer->numTokens-1] = (char*)malloc(strlen(token) + 1);
				strcpy(tokenizer->tokenArray[tokenizer->numTokens-1],token);
				lineptr +=(*iptr + strlen(tokenizer->tokenArray[tokenizer->numTokens-1]));
				free(token);

				while(token != NULL){
					*iptr =  0;
					token = TKGetNextToken(lineptr);
						if(token == NULL)
							break;

					tokenizer->numTokens++;

					tokenizer->tokenArray = (char**)realloc(tokenizer->tokenArray, sizeof(char*) * (tokenizer->numTokens));
					tokenizer->resultArray = (char**)realloc(tokenizer->resultArray, sizeof(char*) * (tokenizer->numTokens));

					tokenizer->tokenArray[tokenizer->numTokens-1] = (char*)malloc(strlen(token) + 1);
					strcpy(tokenizer->tokenArray[tokenizer->numTokens-1],token);
					free(token);
				    lineptr += (strlen(tokenizer->tokenArray[tokenizer->numTokens-1]) + *iptr);
				}
		}



			int x;
		for(x = 0; x<tokenizer->numTokens;x++){

							if(checkDecimal(tokenizer->tokenArray[x]) == 1){
 									tokenizer->resultArray[x] = "Decimal"; 
 								}

 							else if(checkFloat(tokenizer->tokenArray[x]) == 1){
 									tokenizer->resultArray[x] = "Float"; 		

 								}

 							else if(checkHex(tokenizer->tokenArray[x]) == 1){
 									tokenizer->resultArray[x] = "Hex"; 		

 								}	

 							else if(checkOctal(tokenizer->tokenArray[x]) == 1){
 									tokenizer->resultArray[x] = "Octal";
 								}

 							else {
 									char* sanchit = tokenizer->tokenArray[x];
 									char* hold = malloc(10);
 									sprintf(hold, "[0x%x]",*sanchit);
 									tokenizer->resultArray[x] = (char*)malloc(7);
 									strcpy(tokenizer->resultArray[x],hold);
 									free(hold);
 	 							}

				}


 	fclose(file);
	fclose(checkFile);
	return tokenizer;
}

void TKDestroy( TokenizerT * tk ) {
	/* fill in your code here */
		
	char* result;

		if(tk==NULL)
		return;

		for(i=0;i<tk->numTokens;i++){
			free(tk->tokenArray[i]);

			result = tk->resultArray[i];

			if(result[0] == '[')
				free(tk->resultArray[i]);

		}



		free(tk->tokenArray);
		free(tk->resultArray);
		free(tk);

		tk->tokenArray = NULL; tk->resultArray = NULL; tk = NULL;

		return;
			
}

/*
 * TKGetNextToken is a helper function that would be used by TKCreate when traversing through each line in the file
 * This function would take a char* as input, indicating where it should start
 * Then returns a char* pointing to a string which is a copy of the next token
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( char * start ) {
    	/* fill in your code here */

	char* string = start; //pointer to line
	char*	temp ;
	char*	token;
	char c;



			if(isspace(*string)){
					string++;
					*iptr = *iptr +1;
					return TKGetNextToken(string);
			}

			if(*string == '\0'){
					return NULL;
			}
			


				int i;
			for(i=0;i<strlen(string);i++){
					if(isspace(string[i])){
							c = string[i];
							break;
					}
			}

			if(!isspace(*string)){
				temp = strchr(string, c);
				int x = strlen(temp);
				int size = strlen(string) - x;
				token = calloc(1,size);
				memcpy(token,string,size);
				return token;
			}

return NULL;
}

void TKPrint(TokenizerT *tk){
	/* fill in your code here */

	TokenizerT* tokenizer = tk;


	if(tk==NULL)
		return;

	int x = tokenizer->numTokens;

	char** tokens = tokenizer->tokenArray;
	char** result = tokenizer->resultArray;

	FILE* file1 = fopen("./result","w");
	FILE* file2 = fopen("./error.msg","w");
		int i;
	for(i=0;i<x;i++){

		char* token = tokens[i];
		char* answer = result[i];

		printf("%s -------   %s\n", token,answer);

		if(answer[0] == '['){
			fprintf(file2, "%s\n", answer);
			continue;
		}

		fprintf(file1, "%s    -:- 	%s\n", token,answer);

	}

	fclose(file1);
	fclose(file2);
	return;

}

int is_empty(char *s) {
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

int checkFloat(char *s)
{
    char *str = NULL;
    (void) strtol(s, &str, 0);

    if (!*str)
        return 0;  

    if(*str == '.')
    	if( *++s == 0)
    		return 0;

    if (*str == 'e'  || *str == 'E'  ||  *str == '.')
        return 1;
    
    return 0;  
}


int checkHex(char *s)
{


    if(*s == '0')
    {
        ++s;
        if((*s == 'x') || (*s == 'X')){
                ++s;
            if (s[strspn(s, "0123456789abcdefABCDEF")] == 0){
                return 1;
            }     

    }
    return 0;
        }
        return 0;
}


int checkOctal(char *s)
{
    

    if (*s != '0')  return 0;
     
    	while (isdigit(*s) && *s != '8' && *s != '9')
    	{
     	   ++s;
     	   if(*s == '\0')
     	   		return 1;
   		 }

    return 0;
}

int checkDecimal(char *s)
{


	char* string = s;

	if(*string == '0' && strlen(string) == 1)
		return 1;
    
 if(*s == '0') return 0;

    while (*s != '\0')
    {
        if (isdigit(*s) == 0)
        {
            return 0;
        }
        s++;
    }

    return 1;
}
