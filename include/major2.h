#ifndef MAJOR2_H
#define MAJOR2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <signal.h>


#define TRUE 1
#define FALSE 0

#define MAX_BUFFER 1024
#define MAX_ARGS 512
#define MAX_PIPES 24

#define READ 0
#define WRITE 1


int main(int argc, char** argv);
int parse_input(char* input);
int process_command(char* input);
int process_handler(char* input,int pipeNum);
int exec_cmd(char *const *argv);
void exec_cd(char* input);

// parse.c
int strtrim(char** src);
int isStrWhiteSpace(const char* str);
char** tokenize(char* str, const char token_delimiters[]);
int free_tokens(int index,char** tokens);
// myhistory.c
int myhistory(char *input, int * , char ** , int*);

#endif
