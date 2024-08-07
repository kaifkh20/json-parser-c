#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include "lexer.h"
#include "parser.h"
#include "utility_func.h"
#include <ctype.h>


// #include <stdio.h>
ResponseKV json_parser(int argc,char* argv[]){


    char* file_name = argv[2];

    FILE* file;
    file = fopen(file_name,"r");
    if(file==NULL){
        printf("File does not exsist\n");
        exit(EXIT_FAILURE);
    }
    fseek(file,0,SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    char* string = (char*)malloc(fsize+1);
    size_t n = fread(string,fsize,1,file);

    fclose(file);

    string [fsize] = 0;

    struct Response res_lexer;
    
    //Flat lining string 

    char *out = string;
    for(char *in=string;*in!='\0';in++){
        if(!isspace(*in)){
            *out++ = *in;
        }
    }
    *out = '\0';

    // printf("Enter your json string\n");
    // char* json_string;
    // scanf("%s",string);
    // printf("%s is string\n",string);
    res_lexer = lexer(string);

    // printf("Lexer completed\n");
    ResponseKV res;
    res = parser(res_lexer.token_container,res_lexer.length);    
    res.freemem = free_mem;

    
    free(res_lexer.token_container);
    free(string);    
    printf("Succesfully Parsed\n");

    return res;

}
#endif