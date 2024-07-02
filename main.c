#include "json_parser.h"

// void printArray(int len,)


int main(int argc,char* argv[]){
    ResponseKV kv = json_parser(argc,argv);
    
    // printf("%d",kv.size);
    // printf("%d",argc);
    // printf("%s",argv[3]);
    // printf("%d",kv.object.size);
    

    
    if(argc>3 && strcmp(argv[3],"--test")==0){
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
    else{
        Value *value = kv.lookup("key",&kv);
        Value *value1 = kv.lookup("key1",&kv);
        Value *value2 = kv.lookup("key2",&kv);
        Value *value3 = kv.lookup("key3",&kv);

        printf("%s\n",typeOf(value));
        printf("%s\n",typeOf(value1));
        printf("%s\n",typeOf(value2));
        printf("%s\n",typeOf(value3));

        free_val(value);
        free_val(value1);
        free_val(value2);
        free_val(value3);
    }
    // kv.freemem(&kv);
    // free(kv.lookup);
    // free(kv)

    // kv.lookup()
    // printf("%s\n",value);
    // for(int i=0;i<kv.size;i++){
    //     printf("key:%s , value:%s\n",kv.arr[i].Key.key,kv.arr[i].Value.value);
    // }
    // printf("%s\n",kv.arr[0].Key.key);
    // while(1){}
    return 0;
}