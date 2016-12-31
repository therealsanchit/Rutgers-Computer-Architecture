#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c-sim.h"
#include <math.h>


int main(int argc, char** argv) {

Tokenizer* tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));
mainCache* cache = (mainCache*) malloc(sizeof(mainCache));


	if(argc != 5){
		printf("%s\n", "Arguments not in correct form. Please enter, ./c-sim <cache size> <associativity> <block size> <trace file> ");
		return -1;
	}

	char* cSize = argv[1];
	char* policy = argv[2];
	char* bSize = argv[3];
	char* fileName = argv[4];

	int cacheSize = atoi(cSize);
	int blockSize = atoi(bSize);
	int noSets;

	int x;

	if(isPowerOfTwo(cacheSize) != 1){
		printf("%s\n", "< cachesize >  Should be a power of 2");
		return -1;
	}


	if(strcmp(policy,"direct") == 0){
			 noSets = cacheSize/ blockSize;
			 printf("%d\n", noSets);
			cache->noOfSets = noSets;
			cache->set = (CacheSet*)malloc(sizeof(CacheSet)*cache->noOfSets);
			cache->set->noOfLines = 1;
			x = 1;
	}
	else if(strcmp(policy,"assoc") == 0){
			noSets = cacheSize/(4*(blockSize));
			cache->noOfSets = noSets;
			cache->set = (CacheSet*)malloc(sizeof(CacheSet)*cache->noOfSets);
			cache->set->noOfLines = 4;
			x=-1;
	}
	else{
		printf("%s\n", "Please enter correct Associativity");
		return -1;
	}

	int bitCacheBlock = floor(log(blockSize)/log(2)); 
	int bitSetBlock = floor(log(noSets)/log(2)) ; 
	int bitTagBlock = 32 - bitSetBlock - bitCacheBlock; 
	cache->blockSize = bitCacheBlock;
	cache->setSize = bitSetBlock;
	cache->tagSize = bitTagBlock;

	createTokens(tokenizer,fileName);

	if(x==1)
		simulateDirect(cache, tokenizer);
	if(x==-1)
		simulateAssoc(cache,tokenizer);


	killEverything(tokenizer, cache, x);


	return 0;
}



void createTokens(Tokenizer* tokenizer,char* fileName){

	tokenizer->noOfTokens = 0;
	tokenizer->tokenArray = (char**) malloc(sizeof(char*) * (tokenizer->noOfTokens)); 
	tokenizer->functionArray = (char**) malloc(sizeof(char*) * (tokenizer->noOfTokens));

	char line[50];

	FILE* file = fopen(fileName,"r");

	while(fgets(line,sizeof(line),file)){
		
		char* pch = strchr(line,' ');
		if(pch == NULL) break;

		char* dup = strdup(pch);

		char* function = strtok(dup, " ");

		pch = pch + 3;
		dup = strdup(pch);
		char* token = strtok(dup, " ");

		tokenizer->tokenArray[tokenizer->noOfTokens] = (char*)malloc(strlen(token) + 1);
		tokenizer->functionArray[tokenizer->noOfTokens] = (char*)malloc(strlen(function) + 1);
		strcpy(tokenizer->tokenArray[tokenizer->noOfTokens],token);
		strcpy(tokenizer->functionArray[tokenizer->noOfTokens], function);
		tokenizer->noOfTokens++;
		tokenizer->tokenArray = (char**) realloc(tokenizer->tokenArray ,sizeof(char*) * (tokenizer->noOfTokens) + 1);
		tokenizer->functionArray = (char**) realloc(tokenizer->functionArray ,sizeof(char*) * (tokenizer->noOfTokens) + 1);
	}
	fclose(file);
	return;
}

void simulateDirect(mainCache* cache, Tokenizer* tokenizer){

	long memCacheBlock;
	long memSetBlock;
	long memTagBlock;

		int k;
	for(k=0;k<cache->noOfSets;k++){
		cache->set[k].line = (SetLine*)malloc(sizeof(SetLine));
		cache->set[k].line->data = (LineData*)malloc(sizeof(LineData));
		cache->set[k].line->data->valid = 0;
	}
		int i;
	for(i=0;i<tokenizer->noOfTokens;i++){

		char* function = tokenizer->functionArray[i];
		char* address = tokenizer->tokenArray[i];
		long adr = (long)strtol(address, NULL, 16);

		int c = pow(2,cache->blockSize);
		memCacheBlock = adr%(c);
		adr /= c;

		c = pow(2,cache->setSize);
		memSetBlock = adr%(c);
		adr /=  (c);

		memTagBlock = adr;


		mainCache* rCache = cache;
		CacheSet* rSet = &rCache->set[memSetBlock]; //Selected Set 
		SetLine* rLine = rSet->line; //Line of Selected Set
		LineData* rData = rLine->data; //Data of Selected Line

		if(strcmp(function, "R") == 0){ //Read
            if(rData->valid == 1){
                if(rData->tagNumber == memTagBlock){
                    hits++;
                }
                else{
                    rData->tagNumber = memTagBlock; //write to cache
                    reads++;
                    misses++;
                }
            }
            else{
                rData->valid = 1;
                rData->tagNumber = memTagBlock;
                misses++; //Acessed for first time
				reads++;                
            }
        } 
		else{ //Write
			writes++;
			if(rData->valid == 1){
				if(rData->tagNumber == memTagBlock){ //Data is Already Cached with Same tag
					rData->dataBits = memCacheBlock;
					hits++;
				}else{
					rData->tagNumber = memTagBlock; //Data Cached but Different Tag 
					rData->dataBits = memCacheBlock;
					reads++;
					misses++;
				}
			}else{
				rData->valid = 1;
				rData->tagNumber = memTagBlock;
				rData->dataBits = memCacheBlock;
				reads++;
				misses++;
				}
		}

}
printAll();
return;
}



void simulateAssoc(mainCache* cache, Tokenizer* tokenizer){
	long memCacheBlock;
	long memSetBlock;
	long memTagBlock;

		int k;
	for(k=0;k<cache->noOfSets;k++){
		cache->set[k].line = (SetLine*)malloc(sizeof(SetLine) * cache->set->noOfLines);
		cache->set[k].counter = 0;
				int j;
			for(j=0;j<4;j++){
		cache->set[k].line[j].data = (LineData*)malloc(sizeof(LineData));
		cache->set[k].line[j].data->valid = 0;
		}
	}	
		int i;
	for(i=0;i<tokenizer->noOfTokens;i++){

		char* function = tokenizer->functionArray[i];
		char* address = tokenizer->tokenArray[i];
		long adr = (long)strtol(address, NULL, 16);

		int c = pow(2,cache->blockSize);
		memCacheBlock = adr%(c);
		adr /= c;

		c = pow(2,cache->setSize);
		memSetBlock = adr%(c);
		adr /=  (c);

		memTagBlock = adr;

		mainCache* rCache = cache;
		CacheSet* rSet = &rCache->set[memSetBlock]; //Selected Set
		
		int h = strcmp(function,"R");
		if(h!=0) writes++;
		rSet->counter=0;
			int v;
		for(v =0;v<4;v++){
			SetLine* rLine = &rSet->line[v];
			LineData* rData = rLine->data;

			if(rData->valid == 1){
				if(rData->tagNumber == memTagBlock){
						hits++;
						break;
				}
				else{
					if(v==3){
						SetLine* nLine = &rSet->line[((rSet->counter)%4)];
						LineData* nData = nLine->data;
						nData->tagNumber = memTagBlock;
						misses++;
						reads++;
						rSet->counter = 0;
						break;
					}
					rSet->counter++;
					continue;
				}
				}else{
					rData->valid = 1;
					rData->tagNumber = memTagBlock;
					misses++;
					reads++;
					break;
				}

		}

	}
printAll();
return;

}



void killEverything(Tokenizer* tk, mainCache* cache, int policy){


			int i;
		for(i=0;i<tk->noOfTokens;i++){
			free(tk->tokenArray[i]);
			free(tk->functionArray[i]);
		}


		if(policy==-1){ //asoc
			for(i=0;i<cache->noOfSets;i++){
			int j;
				for(j=0;j<4;j++)
					free(cache->set[i].line[j].data);
			free(cache->set[i].line);
			}
		}else{
			for(i=0;i<cache->noOfSets;i++){
				free(cache->set[i].line->data);
				free(cache->set[i].line);
		}
	}

		free(cache->set);
		free(cache);
		free(tk->tokenArray);
		free(tk->functionArray);
		free(tk);

		tk->tokenArray = NULL; tk->functionArray = NULL; tk = NULL; cache->set->line = NULL; cache->set = NULL; cache = NULL;

		return;
}


void printAll(){

printf("%s\n", "");

	printf("Memory Reads: %d\n", reads);
	printf("Memory Writes: %d\n", writes);
	printf("Cache Hits: %d\n", hits);
	printf("Cache Misses: %d\n", misses);
}


int isPowerOfTwo (unsigned int x)
{
 while (((x % 2) == 0) && x > 1) /* While x is even and > 1 */
   x /= 2;
 return (x == 1);
}

