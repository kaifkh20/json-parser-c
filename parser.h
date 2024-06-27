#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stack.h"


void parser(Token* token_container,int len){
    // printf("working");
    struct Stack* stack = createStack(len);
    for(int i=0;i<len;i++){
        char* token = token_container[i].ch;
        if(token=="{"){
            push(stack,token_container[i].ch);
        }
        else if(token=="}"){
            if(isEmpty(stack)==1 || pop(stack)!="{"){
                printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
                free(stack);
                exit(EXIT_FAILURE);
            } 
        }
    } 
    free(stack);
}

#endif