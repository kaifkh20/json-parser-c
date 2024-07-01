#include "json_parser.h"

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
                printf("%s:%li\n",kv.object.arr[i].Key.key,kv.object.arr[i].Value.value.int_val);
            }else{
                printf("%s:%s\n",kv.object.arr[i].Key.key,kv.object.arr[i].Value.value.string_val);
            }
        }
    }
    
    // Freeing memory 
    for(int i=0;i<kv.object.size;i++){
        if(kv.object.arr[i].Value.val_type==OBJECT_TYPE){
            free(kv.object.arr[i].Value.value.obj_val);
        }
    }

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