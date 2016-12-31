#ifndef CSIM_H
#define CSIM_H
#include <string.h>
#include <ctype.h>
#include <math.h>

int isPowerOfTwo (unsigned int x);
int hits;
int misses;
int reads;
int writes;


typedef struct Tokenizer{
	char** tokenArray;
	char** functionArray;
	int noOfTokens;
} Tokenizer;

typedef struct Data{
	int valid;
	int tagNumber;
	int dataBits;
} LineData;

typedef struct Line
{
	LineData* data;
} SetLine;

typedef struct Set{
	int noOfLines;
	SetLine* line;
	int counter;
} CacheSet;

typedef struct Cache {
	/* fill in your code here */
	int noOfSets;
	CacheSet* set;
	int blockSize;
	int setSize;
	int tagSize;
	} mainCache;



void simulateDirect(mainCache* cache, Tokenizer* tokenizer);
void simulateAssoc(mainCache* cache, Tokenizer* tokenizer);
void createTokens(Tokenizer* tokenizer,char* traceFile);
void killEverything(Tokenizer* tokenizer, mainCache* cache, int policy);
void printAll();
#endif
