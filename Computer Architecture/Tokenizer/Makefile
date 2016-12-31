#Variable definition
CC  = gcc 
CCFLAGS = -Wall 

#rules definition
all : main

main : main.c tokenizer.c tokenizer.h
	$(CC) $(CCFLAGS) -o main main.c tokenizer.c

#This rule cleans up executable file
clean: 
	rm -f main 


	
