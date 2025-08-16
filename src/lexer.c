#include "lexer.h"


char next_token(char* json_string,int i){
    // int index = *i;
    // (*i)++;
    return json_string[i];
}


int consume_token(char token,Token* token_container,int i,char* json_string,int idx_tc){

    Token t;
    // printf("%c token\n",token);
    // printf("index %d",idx_tc);

    // if(token>='0' && token<='9') printf("Number \n");

    int index = i;
    char *tc = (char*)malloc(sizeof(char)*2);
    switch (token)
    {
    case '{':
        // strncpy(t.ch,&token,1);
        // strncpy(t.ch,token);
        tc[0] = token;
        tc[1] = '\0';
        // strncat(t.ch,tc,1);
        strcpy(t.ch,tc);
        t.t_type = StartObject;
        break;
    case '}':

        // tc = (char*)malloc(sizeof(char));
        tc[0] = token;
        tc[1] = '\0';
        // strncat(t.ch,tc,1);
        strcpy(t.ch,tc);
        t.t_type = EndObject;
        break;

    case '"':
        // printf("here");
        char *string = malloc(sizeof(char)*100);
        strcpy(string,"");
        // int idx = i;
        // idx++;
        size_t idx = i+1;
        char token_c = next_token(json_string,idx);
        while(token_c!='"' && idx<strlen(json_string)){
            strncat(string,&token_c,1);
            ++idx;
            token_c = next_token(json_string,idx);
        }
        token_c = next_token(json_string,++idx);
        // printf("tokenc after loop%c\n",token_c);
        if(token_c==':'){
            // printf("reached here");
            strcpy(t.ch,string);
            t.t_type = StringKey;
        }else{
            strcpy(t.ch,string);
            // t.ch = string;
            t.t_type = StringValue;
        }
        // string[strlen(string)]  = '\0';
        // printf("%s ",t.ch);
        // printf("string is %s %c is nexttoken\n",string,next_token(json_string,++idx));
        free(string);
        i = idx-1;
        // printf("%c last consumed",json_string[i]);
        break;

    case ':':
        // t.ch = &token;
        tc[0] = token;
        tc[1] = '\0';
        // *tc = token+"\0";
        // strncat(t.ch,tc,1);
        strcpy(t.ch,tc);
        t.t_type = KeyValueSeperator;
        break;
    case ',':
        tc[0] = token;
        tc[1] = '\0';
        strcpy(t.ch,tc);
        t.t_type = Comma;
        break;
    case '0' ... '9':
        // printf("reaching here %c\n",token);
        char *stringN = (char*)malloc(sizeof(char)*100);
        strcpy(stringN,"");
        // strcpy(stringN,&token);
        // printf("%s\n",token_c)//
        strncat(stringN,&token,1);
        idx = i+1;
        char token_c_n = next_token(json_string,idx);
        // printf("%c token_c \n",token_c_n);
        while(token_c_n>='0' && token_c_n<='9' && strlen(json_string)>idx){
            // printf("%c\n",token_c);
            // printf("reaching here %c\n",token_c_n);
            strncat(stringN,&token_c_n,1);
            // printf("%s after reaching\n",stringN);
            ++idx;
            token_c_n = next_token(json_string,idx);    
        }
        // printf("%s\n",stringN);
        strcat(stringN,"\0");
        strcpy(t.ch,stringN);
        // printf("%s number\n",stringN);
        free(stringN);
        t.t_type = Integer;
        i = idx-1;
        break;
    case '[':
        tc[0] = token;
        tc[1] = '\0';
        strcpy(t.ch,tc);
        t.t_type = ArrayStart;
        break;
    case ']':
        tc[0] = token;
        tc[1] = '\0';
        strcpy(t.ch,tc);
        t.t_type = ArrayEnd;
        break;
    case 'n':
        index = i+1;
        char null[4];
        null[0] = 'n';
        for(int j=0;j<3;j++){
            token_c = next_token(json_string,index);
            // printf("%c,",token_c);
            if(j==0 && token_c!='u') break;
            if(j==1 && token_c!='l') break;
            if(j==2 && token_c!='l') break;
            null[j+1] = token_c;
            index++;
        }
        if(strcmp(null,"null")==0){
            // printf("here");
            strcpy(t.ch,null);
            t.t_type = NullValue;
            // printf("%s",t.ch);
            i = index-1;
        }
        break;
    case 't': 
        index = i+1;
        char true_[4];
        true_[0] = 't';
        for(int j=0;j<3;j++){
            token_c = next_token(json_string,index);
            // printf("%c,",token_c);
            if(j==0 && token_c!='r') break;
            if(j==1 && token_c!='u') break;
            if(j==2 && token_c!='e') break;
            true_[j+1] = token_c;
            index++;
        }
        if(strcmp(true_,"true")==0){
            // printf("here");
            strcpy(t.ch,true_);
            t.t_type = BooleanTrue;
            // printf("%s",t.ch);
            i = index-1;
        }
        break;

    default:
        fprintf(stderr,"Lexical Error: %c not valid\n",token);
        free(tc);
        return 0;
        break;
    }

    free(tc);
    
    // printf("%s is char",t.ch);
    token_container[idx_tc] = t;
    return ++i;
}



void remove_whitespace(char* json_string) {
    if (json_string == NULL) {
        return;
    }

    char* write_ptr = json_string;
    char* read_ptr = json_string;

    while (*read_ptr != '\0') {
        if (!isspace((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0'; // Add the null terminator at the end of the new string
}

struct Response lexer(char* json_string){
    
    remove_whitespace(json_string);
    size_t len_json_string = strlen(json_string);

    // printf("%s is the string,%zu is size\n",json_string,len_json_string);
    // Token* token_container = (Token*)(sizeof(Token)*len_json_string);

    Token* token_container = (Token*) malloc(len_json_string*sizeof(Token));
    // printf("reaching here");
    // int i = 0;
    int idx_tc = 0;
    for (size_t i=0;i<len_json_string;){
        char token = next_token(json_string,i);
        // printf("%c token\n",token);
        i = consume_token(token,token_container,i,json_string,idx_tc);
        if(i==0){
            free(token_container);
            exit(EXIT_FAILURE);
        }
        idx_tc++;
    }
    //debug_print(token_container,idx_tc);
    struct Response resp;
    resp.token_container = token_container;
    resp.length = idx_tc;



    return resp;
}


const char* enum_to_string(enum TokenType t_type){
    switch (t_type)
    {
    case StartObject:
        /* code */
        return "StartObject";
    case EndObject:
        return "EndObject";
    case StringKey:
        return "StringKey";
    case StringValue:
        return "StringValue";
    case Comma:
        return "Comma";
    case Integer:
        return "Integer";
    case ArrayStart:
        return "ArrayStart";
    case ArrayEnd:
        return "ArrayEnd";
    case NullValue:
        return "Null";
    case BooleanTrue:
        return "BooleanTrue";
    case BooleanFalse:
        return "BooleanFalse";
    case KeyValueSeperator:
        return "KeyValueSeperator";
    default:
        return "Invalid";
        break;
    }
}

void debug_print(Token* token_container,int len){
    for(int i=0;i<len;i++){
        printf("<%s,%s>\n",token_container[i].ch,enum_to_string(token_container[i].t_type));
    }
}


