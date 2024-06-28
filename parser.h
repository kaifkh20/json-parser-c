#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stack.h"

enum ValueType{
    STRING,
};

typedef enum ValueType ValueType;

struct Key{
    char key[100];
};

typedef struct Key Key;

struct Value{
    ValueType val;
    char value[100];
};

typedef struct Value Value;

struct KeyValue{
    Key Key;
    Value Value;
    // char* (*lookup)(char* key);
};

typedef struct KeyValue KeyValue;

struct ResponseKV{
    KeyValue arr[100];
    size_t size;
    char* (*lookup)(char*,struct ResponseKV*);
};

typedef struct ResponseKV ResponseKV; 

char* lookup(char* key,ResponseKV* self){

    for(int i=0;i<self->size;++i){

        if(strcmp(self->arr[i].Key.key,key)==0){
            return self->arr[i].Value.value;
        }
    }
    return "Invalid Key";
}

void parse_object(Token* token_container,int len,int idx,KeyValue arr[],size_t* idx_arr){

    // printf("%d",idx);
    struct Stack* stack = createStack(len);
    for(int i=idx;i<len;i++){
        char* token = token_container[i].ch;
        // TokenType type = token_container[i].t_type; 
        enum TokenType type_token = token_container[i].t_type;
        // printf("%s\n",token);
        if(strcmp(token,"{")==0){
            // parse_object(token_container,len,i++,arr);
            push(stack,token_container[i].ch);
        }
        else if(token=="}"){
            if(isEmpty(stack)==1 || pop(stack)!="{"){
                printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
                free(stack);
                exit(EXIT_FAILURE);
            }else{
                return ;
            } 
        }else if(type_token==StringKey){
            // printf("reaching here");
            Key key;
            strcpy(key.key,token);
            arr[*idx_arr].Key = key;
            // printf("%s",key.key);
        }else if(type_token==StringValue){
            Value val;
            strcpy(val.value,token);
            arr[*idx_arr].Value = val;
            (*idx_arr)++;
        }  

    } 

    free(stack);
}


ResponseKV parser(Token* token_container,int len){
    // printf("working");
    KeyValue arr[100];
    size_t idx_arr = 0;
    parse_object(token_container,len,0,arr,&idx_arr);
    ResponseKV res;
    memcpy(res.arr,arr,100*sizeof(KeyValue));
    res.size = idx_arr;
    res.lookup = lookup;
    // res.lookup = lookup;
    return res;
}

#endif