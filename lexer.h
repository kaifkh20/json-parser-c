#ifndef LEXER_H
#define LEXER_H

#include<stdio.h>
#include <string.h>
#include <stdlib.h>

enum TokenType{
    StartObject,
    EndObject
};

struct Token{
    char ch;
    enum TokenType t_type;
};


typedef struct Token Token;

struct Response{
    Token* token_container;
    size_t length;
};

char* enum_to_string(enum TokenType t_type){
    switch (t_type)
    {
    case StartObject:
        /* code */
        return "StartObject";
    case EndObject:
        return "EndObject";
    default:
        return "Invalid";
        break;
    }
}

void consume_token(char token,Token* token_container,int i){

    Token t;

    switch (token)
    {
    case '{':
        /* code */
        t.ch = token;
        t.t_type = StartObject;
        break;
    case '}':
        t.ch = token;
        t.t_type = EndObject;
        break;
    default:
        printf("%c not valid",token);
        exit(EXIT_FAILURE);
        break;
    }
    
    token_container[i] = t;

}

void debug_print(Token* token_container,int len){
    for(int i=0;i<len;i++){
        printf("<%c,%s>\n",token_container[i].ch,enum_to_string(token_container[i].t_type));
    }
}

struct Response lexer(char* json_string){

    size_t len_json_string = strlen(json_string);

    printf("%s is the string,%zu is size\n",json_string,len_json_string);
    // Token* token_container = (Token*)(sizeof(Token)*len_json_string);

    Token* token_container = (Token*) malloc(len_json_string*sizeof(Token));

    for(int i=0;i<len_json_string;++i){
        consume_token(json_string[i],token_container,i);
    }

    debug_print(token_container,len_json_string);
    // free(token_container);
    struct Response resp;
    resp.token_container = token_container;
    resp.length = len_json_string;

    return resp;
}


#endif