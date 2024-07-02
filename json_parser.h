#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include "lexer.h"
#include "parser.h"
#include <ctype.h>

void free_mem(ResponseKV kv){
    // Freeing memory 
    for(int i=0;i<kv.object.size;i++){
        if(kv.object.arr[i].Value.val_type==OBJECT_TYPE){
            free(kv.object.arr[i].Value.value.obj_val);
        }else if(kv.object.arr[i].Value.val_type==ARRAY){
            // if(kv.object.arr[i].Value.value.arr_val->value_array->)
            size_t size = kv.object.arr[i].Value.value.arr_val->size;
            for(int j=0;j<size;++j){
                if(kv.object.arr[i].Value.value.arr_val->value_array[j].val_type==ARRAY)
                    free(kv.object.arr[i].Value.value.arr_val->value_array[j].value.arr_val);
                    // free(kv.object.arr[i].Value.value.arr_val->value_array);
            }
            free(kv.object.arr[i].Value.value.arr_val->value_array);
            free(kv.object.arr[i].Value.value.arr_val);
        }else if(kv.object.arr[i].Value.val_type==OBJECT_TYPE){
            free(kv.object.arr[i].Value.value.obj_val);
        }
    }
    
}

// #include <stdio.h>
ResponseKV json_parser(int argc,char* argv[]){


    char* file_name = argv[2];

    FILE* file;
    file = fopen(file_name,"r");
    if(file==NULL){
        printf("File does not exits\n");
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

    
    free(res_lexer.token_container);
    free(string);    
    printf("Succesfully Parsed\n");

    return res;

}
#endif