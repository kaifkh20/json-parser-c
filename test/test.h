#ifndef TEST_H
#define TEST_H

#include "../parser.h"

void print_test(ResponseKV kv){
    for(int i=0;i<kv.object.size;i++){
        // printf(/)
        if(kv.object.arr[i].Value.val_type==OBJECT_TYPE){
            printf("%s:<Object>\n",kv.object.arr[i].Key.key);
            // continue;
        }else if(kv.object.arr[i].Value.val_type==INTEGER){
            printf("%s:%lli\n",kv.object.arr[i].Key.key,kv.object.arr[i].Value.value.int_val);
        }else if(kv.object.arr[i].Value.val_type==ARRAY){
            printf("%s: ",kv.object.arr[i].Key.key);
            // printf("size of array %li\n",kv.object.arr[i].Value.value.arr_val->size);
            printf("<Array ");
            for(int j=0;j<kv.object.arr[i].Value.value.arr_val->size;++j){
                if(kv.object.arr[i].Value.value.arr_val->value_array[j].val_type ==INTEGER){
                    printf("%lli",kv.object.arr[i].Value.value.arr_val->value_array[j].value.int_val);
                }
                else if(kv.object.arr[i].Value.value.arr_val->value_array[j].val_type==OBJECT_TYPE){
                    printf("<Object>");
                } 
                else if(kv.object.arr[i].Value.value.arr_val->value_array[j].val_type==ARRAY){
                    printf("<Array %li>",kv.object.arr[i].Value.value.arr_val->value_array[j].value.arr_val->size);
                }
                else printf("%s",kv.object.arr[i].Value.value.arr_val->value_array[j].value.string_val);

                if(j!=kv.object.arr[i].Value.value.arr_val->size-1){
                    printf(", ");
                }   
            }
            printf(" >\n");
        }
        else{
            printf("%s:%s\n",kv.object.arr[i].Key.key,kv.object.arr[i].Value.value.string_val);
        }
    }
}

#endif