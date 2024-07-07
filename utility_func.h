#ifndef FREE_H
#define FREE_H

#include "parser.h"

void free_mem_array(Array* array);

void free_mem_object(Object* object){
    // printf("Size of object %li",object->size);
    for(int i=0;i<object->size;++i){
        if(object->arr[i].Value.val_type==OBJECT_TYPE){
            // printf("There is a nested object size is %s \n",object->arr[i].Value.value.obj_val->arr[0].Value.value.string_val);
            // free_mem_object(object->arr[i].Value.value.obj_val);
        }else if(object->arr[i].Value.val_type==ARRAY){
            free_mem_array(object->arr[i].Value.value.arr_val);
        }
    }
    free(object);
}

void free_mem_array(Array* array){
    size_t size = array->size;
    for(int i=0;i<size;++i){
        if(array->value_array[i].val_type==ARRAY){
            free_mem_array(array->value_array[i].value.arr_val);
        }else if(array->value_array[i].val_type==OBJECT_TYPE){
            free_mem_object(array->value_array[i].value.obj_val);
        }
    }
    free(array->value_array);
    free(array);
}

void free_mem(ResponseKV* kv){ //Free Memory include only at the end of program
    // Freeing memory 
    for(int i=0;i<kv->response_value.obj.size;i++){
        if(kv->response_value.obj.arr[i].Value.val_type==OBJECT_TYPE){
            free_mem_object(kv->response_value.obj.arr[i].Value.value.obj_val);
        }else if(kv->response_value.obj.arr[i].Value.val_type==ARRAY){
            // if(kv.object.arr[i].Value.value.arr_val->value_array->)
            free_mem_array(kv->response_value.obj.arr[i].Value.value.arr_val);
        }
        // else if(kv->object.arr[i].Value.val_type==OBJECT_TYPE){
        //     free(kv->object.arr[i].Value.value.obj_val);
        // }
    }
    // free(kv);
    
}

void free_val(Value* value){
    if(value->val_type==OBJECT_TYPE){
        free(value->value.obj_val);
    }else if(value->val_type==ARRAY){
        free(value->value.arr_val->value_array);
        free(value->value.arr_val);
    }
    free(value);
}

char* typeOf(Value* value){
    if(value->val_type==STRING){
        return "\e[1;33mSTRING";
    }else if(value->val_type==INTEGER){
        return "\e[1;33mINTEGER";
    }else if(value->val_type==OBJECT_TYPE){
        return "\e[1;32mOBJECT";
    }else if(value->val_type==ARRAY){
        return "\e[1;32mARRAY";
    }
}


#endif