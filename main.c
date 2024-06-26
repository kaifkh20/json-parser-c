#include "lexer.h"
#include "parser.h"

#include <stdio.h>
int main(){

    struct Response res_lexer;
    res_lexer = lexer("{}}");

    parser(res_lexer.token_container,res_lexer.length);    

    free(res_lexer.token_container);
    

}