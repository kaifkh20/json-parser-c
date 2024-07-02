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
    for(int i=0;i<kv->object.size;i++){
        if(kv->object.arr[i].Value.val_type==OBJECT_TYPE){
            free_mem_object(kv->object.arr[i].Value.value.obj_val);
        }else if(kv->object.arr[i].Value.val_type==ARRAY){
            // if(kv.object.arr[i].Value.value.arr_val->value_array->)
            free_mem_array(kv->object.arr[i].Value.value.arr_val);
        }
        // else if(kv->object.arr[i].Value.val_type==OBJECT_TYPE){
        //     free(kv->object.arr[i].Value.value.obj_val);
        // }
    }
    // free(kv);
    
}



#endif