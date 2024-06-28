#include "json_parser.h"

int main(int argc,char* argv[]){
    ResponseKV kv = json_parser(argc,argv);
    char* value = kv.lookup("kaif",&kv);
    // kv.lookup()
    printf("%s\n",value);
    // for(int i=0;i<kv.size;i++){
    //     printf("key:%s , value:%s\n",kv.arr[i].Key.key,kv.arr[i].Value.value);
    // }
    // printf("%s\n",kv.arr[0].Key.key);
    return 0;
}