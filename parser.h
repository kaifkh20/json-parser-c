#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stack.h"

typedef struct Object Object;
// typedef struct KeyValue KeyValue;

typedef union value value;

typedef enum ValueType{
    STRING,
    OBJECT_TYPE
}ValueType;


typedef struct Key{
    char key[100];
}Key;

union value  
{
    Object* obj_val;
    // Object obj_val;
    char string_val[100];
        /* data */
};

typedef struct Value{
    ValueType val_type;
    union value value;
}Value;

typedef struct KeyValue{
    Key Key;
    Value Value;    
    // char* (*lookup)(char* key);
}KeyValue;

// typedef struct Key Key;
typedef struct Object{
    KeyValue arr[100];
    size_t size;
}Object;




typedef union value value;

// typedef struct KeyValue KeyValue;

// typedef struct Value Value;




struct ResponseKV{
    // KeyValue arr[100];
    Object object;
    // size_t size;
    char* (*lookup)(char*,struct ResponseKV*);
};

typedef struct ResponseKV ResponseKV; 

char* lookup(char* key,ResponseKV* self){

    for(int i=0;i<self->object.size;++i){

        if(strcmp(self->object.arr[i].Key.key,key)==0){
            // if(self->object.arr[i].Value.value.string_val)
            return self->object.arr[i].Value.value.string_val;
        }
    }
    return "Invalid Key";
}

Object* parse_object(Token* token_container,int len,int* idx,struct Stack* stack,int if_sub_obj){

    // printf("Function called with params %d %d\n",len,*idx);
    // printf("%d",*idx);
    KeyValue arr[100];
    size_t idx_arr = 0;
    Object *obj = (Object*)malloc(sizeof(Object));
    // struct Stack* stack = createStack(len);

    // printf("%s starting\n",peek(stack));

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
            // printf("Object returned %d\n",obj.size);
            // printf("idx after obj returns %d",*idx);
            // return parse_object(token_container,len,i++,arr,idx_arr);
        }
        else if(strcmp(token,"}")==0){
            // printf("%d",strcmp("{",peek(stack))|| isEmpty(stack));
            // printf("%s %d %d",peek(stack),strcmp("{",peek(stack)),isEmpty(stack));
            // printf("%d is empty or not",isEmpty(stack) |);
            // printf("%s before if",peek(stack));
            if((isEmpty(stack)==1 || strncmp("{",peek(stack),1)!=0)){
                // printf("reaching here");
                // printf("%d",isEmpty(stack));
                // printf("%s",peek(stack));
                // printf("%d",(isEmpty(stack)||strncmp("{",peek(stack),1)));
                printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
                free(stack);
                exit(EXIT_FAILURE);
            }
            pop(stack);        
            if(if_sub_obj){
                
                
                memcpy(obj->arr,arr,100*sizeof(KeyValue));
                obj->size = idx_arr;

                // if(obj->size==0){
                //     free(obj);
                // }
                // printf("Size of idx_arr %d",obj.size);
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
        }  
        
    }
        // if(i!=(len-1)&&isEmpty(stack)==1){
        //     // printf("")
        //     printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
        //     exit(EXIT_FAILURE);
        // }
        
    

    if(i==len && !isEmpty(stack)){
        // printf("%d",isEmpty(stack));
        // printf("reaching here");
        printf("%s\n",peek(stack));
        printf("Parser Error : Invalid Syntax\nOperation Aborted\n");
        free(stack);
        exit(EXIT_FAILURE);
    }
    memcpy(obj->arr,arr,100*sizeof(KeyValue));
    obj->size = idx_arr;

    // if(obj->size==0){
    //     free(obj);
    // }
    // printf("Size of idx_arr %d",obj.size);
    *idx = i;
    return obj;
    // free(stack);

}


ResponseKV parser(Token* token_container,int len){
    // printf("working");
    // KeyValue arr[100];
    struct Stack* stack = createStack(len);
    // Object obj;
    int idx = 0;
    Object* obj = parse_object(token_container,len,&idx,stack,0);
    ResponseKV res;
    // memcpy(obj.arr,arr,100*sizeof(KeyValue));
    // obj.size = idx_arr;
    res.object = *obj;
    res.lookup = lookup;
    // res.lookup = lookup;

    free(stack);
    return res;
}

#endif