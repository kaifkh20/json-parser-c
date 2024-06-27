#include "lexer.h"
#include "parser.h"

#include <stdio.h>
int main(int argc,char* argv[]){


    char* file_name = argv[2];

    FILE* file;
    file = fopen(file_name,"r");
    fseek(file,0,SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    char* string = malloc(fsize+1);
    size_t n = fread(string,fsize,1,file);

    fclose(file);

    string [fsize] = 0;

    struct Response res_lexer;

    // printf("Enter your json string\n");
    // char* json_string;
    // scanf("%s",string);
    printf("%s is string\n",string);
    res_lexer = lexer(string);

    // printf("Lexer completed");

    parser(res_lexer.token_container,res_lexer.length);    

    free(res_lexer.token_container);
    free(string);    
    printf("Succesfully Parsed\n");

}