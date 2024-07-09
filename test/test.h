#ifndef TEST_H
#define TEST_H

#include "../parser.h"

void print_test(ResponseKV kv){
    if(kv.RESPONSE_TYPE==ARRAY_RESPONSE){
        printf("<Array ");
        for(int i=0;i<kv.response_value.arr.size;++i){
            if(kv.response_value.arr.value_array[i].val_type==STRING){
                printf("%s",kv.response_value.arr.value_array[i].value.string_val);
            }
            else if(kv.response_value.arr.value_array[i].val_type==INTEGER){
                printf("%lli",kv.response_value.arr.value_array[i].value.int_val);
            }else if(kv.response_value.arr.value_array[i].val_type==ARRAY){
                printf("Array");
            }else if(kv.response_value.arr.value_array[i].val_type==OBJECT_TYPE){
                printf("Object");
            }else if(kv.response_value.arr.value_array[i].val_type==NULL_TYPE){
                printf("null");
            }else if(kv.response_value.arr.value_array[i].val_type==BOOLEA)
            if(i!=kv.response_value.arr.size-1){
                printf(", ");
            }
        }printf(">\n");
        // printf("No object\n");
        exit(EXIT_SUCCESS);
    }

    for(int i=0;i<kv.response_value.obj.size;i++){
        // printf(/)
        if(kv.response_value.obj.arr[i].Value.val_type==OBJECT_TYPE){
            printf("%s:<Object>\n",kv.response_value.obj.arr[i].Key.key);
            // continue;
        }else if(kv.response_value.obj.arr[i].Value.val_type==INTEGER){
            printf("%s:%lli\n",kv.response_value.obj.arr[i].Key.key,kv.response_value.obj.arr[i].Value.value.int_val);
        }else if(kv.response_value.obj.arr[i].Value.val_type==ARRAY){
            printf("%s: ",kv.response_value.obj.arr[i].Key.key);
            // printf("size of array %li\n",kv.response_value.obj.arr[i].Value.value.arr_val->size);
            printf("<Array ");
            for(int j=0;j<kv.response_value.obj.arr[i].Value.value.arr_val->size;++j){
                if(kv.response_value.obj.arr[i].Value.value.arr_val->value_array[j].val_type ==INTEGER){
                    printf("%lli",kv.response_value.obj.arr[i].Value.value.arr_val->value_array[j].value.int_val);
                }
                else if(kv.response_value.obj.arr[i].Value.value.arr_val->value_array[j].val_type==OBJECT_TYPE){
                    printf("<Object>");
                } 
                else if(kv.response_value.obj.arr[i].Value.value.arr_val->value_array[j].val_type==ARRAY){
                    printf("<Array %li>",kv.response_value.obj.arr[i].Value.value.arr_val->value_array[j].value.arr_val->size);
                }
                else printf("%s",kv.response_value.obj.arr[i].Value.value.arr_val->value_array[j].value.string_val);

                if(j!=kv.response_value.obj.arr[i].Value.value.arr_val->size-1){
                    printf(", ");
                }   
            }
            printf(" >\n");
        }else if(kv.response_value.obj.arr[i].Value.val_type==NULL_TYPE){
            printf("null");
        }
        else{
            printf("%s:%s\n",kv.response_value.obj.arr[i].Key.key,kv.response_value.obj.arr[i].Value.value.string_val);
        }
    }
}

#endif
