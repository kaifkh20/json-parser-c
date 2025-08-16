#include "json_parser.h"


// #include <stdio.h>
ResponseKV json_parser(char* json_string){

    struct Response res_lexer;
    
    
    // printf("Enter your json string\n");
    // char* json_string;
    // scanf("%s",string);
    // printf("%s is string\n",string);
    res_lexer = lexer(json_string);
    
    //printf("Lexer completed\n");
    ResponseKV res;
    res = parser(res_lexer.token_container,res_lexer.length);    
    res.freemem = free_mem;
    
    
    free(res_lexer.token_container);
    if(res.type == ERROR_RESPONSE){
       free_mem(res); 
       free(json_string);
       exit(EXIT_FAILURE);
    }

    
    printf("Succesfully Parsed\n");

    return res;

}
