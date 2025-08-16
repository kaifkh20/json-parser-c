#include "json_parser.h"
#include "test/test.h"
#include <string.h>
// void printArray(int len,)


int main(int argc,char* argv[]){

    
    char* file_name = argv[1];
        
    FILE* file;
    file = fopen(file_name,"r");
    if(file==NULL){
        printf("File does not exsist\n");
        exit(EXIT_FAILURE);
    }
    fseek(file,0,SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    char* string = (char*)malloc(fsize+1);
    size_t n = fread(string,fsize,1,file);

    fclose(file);

    string [fsize] = 0;

    ResponseKV kv = json_parser(string);   
    
    if(argc>2 && strcmp(argv[2],"--test")==0){
        print_test(kv);
    }
    else{
        Value *value = kv.lookup("key",&kv);
        Value *value1 = kv.lookup("key1",&kv);

        printf("%s\n",typeOf(value));
        printf("%s\n",typeOf(value1));
        
        if(value->val_type==OBJECT_TYPE){
            print_object(value->value.obj_val);
        }
        free_val(value);
        free_val(value1);
    }
    
    free_mem(kv);
    free(string);
    return 0;
}
