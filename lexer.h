#ifndef LEXER_H
#define LEXER_H

#include<stdio.h>
#include <string.h>
#include <stdlib.h>

enum TokenType{
    StartObject,
    EndObject,
    StringKey,
    StringValue,
    KeyValueSeperator,
    Comma
};

struct Token{
    char ch[100];
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
    case StringKey:
        return "StringKey";
    case StringValue:
        return "StringValue";
    case Comma:
        return "Comma";
    default:
        return "Invalid";
        break;
    }
}

char next_token(char* json_string,int i){
    // int index = *i;
    // (*i)++;
    return json_string[i];
}


int consume_token(char token,Token* token_container,int i,char* json_string,int idx_tc){

    Token t;
    // printf("%c token",token);
    // printf("index %d",idx_tc);

    int index = i;
    char *tc = (char*)malloc(sizeof(char)*2);
    switch (token)
    {
    case '{':
        // strncpy(t.ch,&token,1);
        // strncpy(t.ch,token);
        tc[0] = token;
        tc[1] = '\0';
        // strncat(t.ch,tc,1);
        strcpy(t.ch,tc);
        t.t_type = StartObject;
        break;
    case '}':

        // tc = (char*)malloc(sizeof(char));
        tc[0] = token;
        tc[1] = '\0';
        // strncat(t.ch,tc,1);
        strcpy(t.ch,tc);
        t.t_type = EndObject;
        break;

    case '"':
        // printf("here");
        char *string = malloc(sizeof(char)*100);
        strcpy(string,"");
        // int idx = i;
        // idx++;
        int idx = i+1;
        char token_c = next_token(json_string,idx);
        while(token_c!='"' && idx<strlen(json_string)){
            strncat(string,&token_c,1);
            ++idx;
            token_c = next_token(json_string,idx);
        }
        token_c = next_token(json_string,++idx);
        // printf("tokenc after loop%c\n",token_c);
        if(token_c==':'){
            // printf("reached here");
            strcpy(t.ch,string);
            t.t_type = StringKey;
        }else{
            strcpy(t.ch,string);
            // t.ch = string;
            t.t_type = StringValue;
        }
        // string[strlen(string)]  = '\0';
        // printf("%s ",t.ch);
        // printf("string is %s %c is nexttoken\n",string,next_token(json_string,++idx));
        free(string);
        i = idx-1;
        // printf("%c last consumed",json_string[i]);
        break;

    case ':':
        // t.ch = &token;
        tc[0] = token;
        tc[1] = '\0';
        // *tc = token+"\0";
        // strncat(t.ch,tc,1);
        strcpy(t.ch,tc);
        t.t_type = KeyValueSeperator;
        break;
    case ',':
        tc[0] = token;
        tc[1] = '\0';
        strcpy(t.ch,tc);
        t.t_type = Comma;
        break;
    default:
        printf("Lexical Error: %c not valid\n",token);
        exit(EXIT_FAILURE);
        break;
    }

    free(tc);
    
    // printf("%s is char",t.ch);
    token_container[idx_tc] = t;
    return ++i;
}

void debug_print(Token* token_container,int len){
    for(int i=0;i<len;i++){
        printf("<%s,%s>\n",token_container[i].ch,enum_to_string(token_container[i].t_type));
    }
}



struct Response lexer(char* json_string){

    size_t len_json_string = strlen(json_string);

    // printf("%s is the string,%zu is size\n",json_string,len_json_string);
    // Token* token_container = (Token*)(sizeof(Token)*len_json_string);

    Token* token_container = (Token*) malloc(len_json_string*sizeof(Token));
    // printf("reaching here");
    // int i = 0;
    int idx_tc = 0;
    for (int i=0;i<len_json_string;){
        char token = next_token(json_string,i);
        // printf("%c token\n",token);
        i = consume_token(token,token_container,i,json_string,idx_tc);
        idx_tc++;
    }
    // debug_print(token_container,idx_tc);
    struct Response resp;
    resp.token_container = token_container;
    resp.length = idx_tc;



    return resp;
}


#endif