#include "lexer.h"


char next_token(char* json_string,int i){
    // int index = *i;
    // (*i)++;
    return json_string[i];
}


int consume_token(char token, Token* token_container, int i, char* json_string, int idx_tc) {
    int invalid_flag = 0;
    Token t;

    int index = i;
    char *tc = (char*)malloc(sizeof(char)*2);
    switch (token) {
    case '{':
        tc[0] = token; tc[1] = '\0';
        strcpy(t.ch, tc);
        t.t_type = StartObject;
        break;
    case '}':
        tc[0] = token; tc[1] = '\0';
        strcpy(t.ch, tc);
        t.t_type = EndObject;
        break;
    case '"': {
        char *string = malloc(sizeof(char) * 100);
        strcpy(string, "");
        size_t idx = i + 1;
        char token_c = next_token(json_string, idx);
        while (token_c != '"' && idx < strlen(json_string)) {
            strncat(string, &token_c, 1);
            ++idx;
            token_c = next_token(json_string, idx);
        }
        token_c = next_token(json_string, ++idx);
        if (token_c == ':') {
            strcpy(t.ch, string);
            t.t_type = StringKey;
        } else {
            strcpy(t.ch, string);
            t.t_type = StringValue;
        }
        free(string);
        i = idx - 1;
        break;
    }
    case ':':
        tc[0] = token; tc[1] = '\0';
        strcpy(t.ch, tc);
        t.t_type = KeyValueSeperator;
        break;
    case ',':
        tc[0] = token; tc[1] = '\0';
        strcpy(t.ch, tc);
        t.t_type = Comma;
        break;
    case '-':
    case '0' ... '9': {
        char buffer[256];
        size_t buf_idx = 0;

        buffer[buf_idx++] = token; // first char (could be '-' or digit)

        size_t idx = i + 1;
        char token_c_n = next_token(json_string, idx);

        int count_of_points_in_float = 0;

        while ((token_c_n >= '0' && token_c_n <= '9') || token_c_n == '.') {
            if (token_c_n == '.') {
                count_of_points_in_float++;
                if (count_of_points_in_float > 1) {
                    free(tc);
                    return -1; // invalid number
                }
            }

            if (buf_idx >= sizeof(buffer) - 1) {
                free(tc);
                return -1; // too long
            }

            buffer[buf_idx++] = token_c_n;
            ++idx;
            token_c_n = next_token(json_string, idx);
        }

        buffer[buf_idx] = '\0';

        strcpy(t.ch, buffer);
        t.t_type = (count_of_points_in_float == 0) ? Integer : Float;

        i = idx - 1;
        break;
    }
    case '[':
        tc[0] = token; tc[1] = '\0';
        strcpy(t.ch, tc);
        t.t_type = ArrayStart;
        break;
    case ']':
        tc[0] = token; tc[1] = '\0';
        strcpy(t.ch, tc);
        t.t_type = ArrayEnd;
        break;
    case 'n': {
        index = i + 1;
        char null[5] = {'n', '\0'};
        for (int j = 0; j < 3; j++) {
            char token_c = next_token(json_string, index);
            if ((j == 0 && token_c != 'u') ||
                (j == 1 && token_c != 'l') ||
                (j == 2 && token_c != 'l')) {
                free(tc);
                return -1;
            }
            null[j + 1] = token_c;
            index++;
        }
        strcpy(t.ch, null);
        t.t_type = NullValue;
        i = index - 1;
        break;
    }
    case 't': {
        index = i + 1;
        char true_[5] = {'t', '\0'};
        for (int j = 0; j < 3; j++) {
            char token_c = next_token(json_string, index);
            if ((j == 0 && token_c != 'r') ||
                (j == 1 && token_c != 'u') ||
                (j == 2 && token_c != 'e')) {
                free(tc);
                return -1;
            }
            true_[j + 1] = token_c;
            index++;
        }
        strcpy(t.ch, true_);
        t.t_type = BooleanTrue;
        i = index - 1;
        break;
    }
    case 'f': {
        index = i + 1;
        char false_[6] = {'f', '\0'};
        for (int j = 0; j < 4; j++) {
            char token_c = next_token(json_string, index);
            if ((j == 0 && token_c != 'a') ||
                (j == 1 && token_c != 'l') ||
                (j == 2 && token_c != 's') ||
                (j == 3 && token_c != 'e')) {
                free(tc);
                return -1;
            }
            false_[j + 1] = token_c;
            index++;
        }
        strcpy(t.ch, false_);
        t.t_type = BooleanFalse;
        i = index - 1;
        break;
    }
    default:
        fprintf(stderr, "Lexical Error: %c not valid\n", token);
        free(tc);
        return -1;
    }

    free(tc);
    if (invalid_flag == 1) return -1;

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
    int len_json_string = strlen(json_string);

    // printf("%s is the string,%zu is size\n",json_string,len_json_string);
    // Token* token_container = (Token*)(sizeof(Token)*len_json_string);

    Token* token_container = (Token*) malloc(len_json_string*sizeof(Token));
    // printf("reaching here");
    // int i = 0;
    int idx_tc = 0;
    for (int i=0;i<len_json_string;){
        char token = next_token(json_string,i);
        // printf("%c token\n",token);
        i = consume_token(token,token_container,i,json_string,idx_tc);
        if(i<=0){
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
    case Float:
        return "Float";
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


