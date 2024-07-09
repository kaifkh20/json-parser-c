#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stack.h"

typedef struct Object Object;
// typedef struct KeyValue KeyValue;

typedef union value value;

typedef struct Array Array;

// typedef struct Value Value;

typedef enum ValueType{
    STRING,
    OBJECT_TYPE,
    INTEGER,
    ARRAY,
    NULL_TYPE
}ValueType;


typedef struct Key{
    char key[100];
}Key;


union value  
{
    Object* obj_val;
    // Object obj_val;
    char string_val[100];
    long long int int_val;
    Array* arr_val;
    void* null_val;
};

typedef struct Value{
    ValueType val_type;
    union value value;
}Value;

struct Array{
    size_t size;
    Value* value_array;        
};

typedef struct KeyValue{
    Key Key;
    Value Value;    
    // char* (*lookup)(char* key);
}KeyValue;

// typedef struct Key Key;
struct Object{
    KeyValue arr[100];
    size_t size;
};

typedef struct ResponseKV{
    // KeyValue arr[100];
    enum RESPONSE_TYPE {OBJECT_RESPONSE,ARRAY_RESPONSE} RESPONSE_TYPE;
    union response_value{
        Object obj;
        Array arr;
    }response_value;
    // size_t size;
    Value* (*lookup)(char*,struct ResponseKV*);
    void (*freemem)(struct ResponseKV*);
}ResponseKV;

// typedef struct ResponseKV ResponseKV; 



Value* lookup(char* key,ResponseKV* self){ // Freeing memory is user concern
    Value* val = (Value*)malloc(sizeof(Value));
    for(int i=0;i<self->response_value.obj.size;++i){
        if(strcmp(self->response_value.obj.arr[i].Key.key,key)==0){
            val->val_type = self->response_value.obj.arr[i].Value.val_type;
            val->value = self->response_value.obj.arr[i].Value.value;
            // return self->object.arr[i].Value;            
        }
    }
    return val;
}


Object* parse_object(Token* token_container,int len,int* idx,struct Stack* stack,int if_sub_obj);


Array* parse_array(Token* token_container,int i,int *idx,int len,struct Stack* stack){
    
    // printf("idx %d\n",*idx);
    Array *array = (Array*)malloc(sizeof(Array));
    // Array array;
    // array->value_array = ()
    array->value_array = (Value*)malloc(sizeof(Value)*100);
    int idx_Array = 0;
    while(token_container[i].t_type!=ArrayEnd){

        if(token_container[i].t_type==StringValue){
            // printf("reaching string in array");
            array->value_array[idx_Array].val_type = STRING;
            strcpy(array->value_array[idx_Array].value.string_val,token_container[i].ch);
            // printf("string after copying %s\n",array->value_array[idx_Array].value.string_val);
            idx_Array++; 
        }else if(token_container[i].t_type==Integer){
            // printf("reaching int in array");
            array->value_array[idx_Array].val_type = INTEGER;
            array->value_array[idx_Array].value.int_val = atoi(token_container[i].ch);
            // printf("%li in array",array->value_array[idx_Array].value.int_val);
            idx_Array++;
        }else if(token_container[i].t_type==StartObject){
            array->value_array[idx_Array].val_type = OBJECT_TYPE;
            *idx = i;
            // push(stack,"{");
            array->value_array[idx_Array].value.obj_val = parse_object(token_container,len,idx,stack,1);
            // printf("index after object %d\n",*idx);
            i = *idx;
            idx_Array++;   
        }
        else if(token_container[i].t_type==NullValue){
            Value val;
            val.val_type = NULL_TYPE;
            val.value.null_val = NULL;
            array->value_array[idx_Array] = val;
            idx_Array++;
        }       
        else if(token_container[i].t_type==ArrayStart){
            // printf("reaching here");
            array->value_array[idx_Array].val_type = ARRAY;
            array->value_array[idx_Array].value.arr_val = parse_array(token_container,i+1,idx,len,stack);
            i = *idx;
            idx_Array++;
        }
        i++;
    }

    array->size = idx_Array;
    // array->size = idx_Array;
    // // arr[idx_arr]
    // arr[idx_arr].Value.val_type = ARRAY;
    // arr[idx_arr].Value.value.arr_val = array;
    // // arr[idx_arr].Value.value.arr_val->value_array = array->value_array;
    // // memcpy(arr[idx_arr].Value.value.arr_val.value_array,array.value_array,sizeof(Value)*100);
    // idx_arr++;
    *idx = i;
    return array;
}

Object* parse_object(Token* token_container,int len,int* idx,struct Stack* stack,int if_sub_obj){

    // printf("Function called with params %d %d\n",len,*idx);
    // printf("%d",*idx);
    KeyValue arr[100];
    size_t idx_arr = 0;
    Object *obj = (Object*)malloc(sizeof(Object));

    int i = 0;
    // printf("%d,%d",*idx,++(*idx));
    for(i=*idx;i<len;i++){
        // printf("%d idx\n",i);
        char* token = token_container[i].ch;
        // printf("%d %s\n",i,token_container[i].ch);
        // TokenType type = token_container[i].t_type; 
        enum TokenType type_token = token_container[i].t_type;
        // printf("%s\n",token);
        if(strcmp(token,"{")==0){
            push(stack,token_container[i].ch);
            // printf("%s pushed into stack\n",token);
            if(i>0 && strcmp(token_container[i-1].ch,":")==0){
                // printf("reaching here");
                *idx = i;
                *idx = ++(*idx);
                // printf("Index after idx op %d\n",*idx);

                obj = parse_object(token_container,len,idx,stack,1);            
                i = *idx;
                // printf("%d after\n",i);
                // idx_arr++;
                char* key = token_container[i-2].ch;
                // printf("Key for object %s\n",key);
                Value val;
                val.val_type = OBJECT_TYPE;
                val.value.obj_val = obj;
                arr[idx_arr].Value =  val;
                idx_arr++;
            // printf("returned object");
            }
        }
        else if(strcmp(token,"}")==0){
            if((isEmpty(stack)==1 || strncmp("{",peek(stack),1)!=0)){
                printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
                free(stack);
                exit(EXIT_FAILURE);
            }
            pop(stack);        
            if(if_sub_obj){
                memcpy(obj->arr,arr,100*sizeof(KeyValue));
                obj->size = idx_arr;
                *idx = i;
                return obj;
            }
        }else if(type_token==StringKey && isEmpty(stack)==0){
            // printf("reaching here");
            Key key;
            strcpy(key.key,token);
            arr[idx_arr].Key = key;
            // printf("%s",key.key);
        }else if(type_token==StringValue){
            Value val;
            strcpy(val.value.string_val,token);
            val.val_type = STRING;
            arr[idx_arr].Value = val;
        
            idx_arr++;
        }else if(type_token==Integer){
            Value val;
            val.val_type = INTEGER;
            val.value.int_val = atoi(token);
            arr[idx_arr].Value = val;

            idx_arr++;
            // strcpy(val.value)
        }
        else if(type_token==ArrayStart){
            Array* array = parse_array(token_container,i+1,idx,len,stack);
            arr[idx_arr].Value.val_type = ARRAY;
            arr[idx_arr].Value.value.arr_val = array;
            idx_arr++;
            i = *idx;
        }
        else if(type_token==NullValue){
            Value val;
            val.val_type = NULL_TYPE;
            val.value.null_val = NULL;
            arr[idx_arr].Value = val;
            idx_arr++;
        }       
    }
        
    

    if(i==len && !isEmpty(stack)){
        printf("%s\n",peek(stack));
        printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
        free(stack);
        exit(EXIT_FAILURE);
    }
    memcpy(obj->arr,arr,100*sizeof(KeyValue));
    obj->size = idx_arr;
    *idx = i;
    return obj;
}


ResponseKV parser(Token* token_container,int len){
    // printf("working");
    // KeyValue arr[100];
    struct Stack* stack = createStack(len);
    // Object obj;
    int idx = 0;
    ResponseKV res;
    if(token_container[0].t_type==ArrayStart){
        Array* arr = parse_array(token_container,1,&idx,len,stack);
        res.response_value.arr = *arr;
        res.RESPONSE_TYPE = ARRAY_RESPONSE;
    }else if(token_container[0].t_type==StartObject){
        Object* obj = parse_object(token_container,len,&idx,stack,0);
        res.response_value.obj = *obj;
        res.RESPONSE_TYPE = OBJECT_RESPONSE;
    }else{
        printf("Parser Error: Invalid Opening Tags\nOperation Aborted\n");
        exit(EXIT_FAILURE);
    }
    // memcpy(obj.arr,arr,100*sizeof(KeyValue));
    // obj.size = idx_arr;
    res.lookup = lookup;
    // res.freemem = free_mem;
    // res.lookup = lookup;
    free(stack->array);
    free(stack);
    return res;
}

#endif
