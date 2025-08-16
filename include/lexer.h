#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h> // For size_t
#include <stdio.h>  // For FILE
#include <string.h>
#include <ctype.h>
// --- Public Data Structures and Enums ---
enum TokenType {
    StartObject,
    EndObject,
    StringKey,
    StringValue,
    KeyValueSeperator,
    Comma,
    Integer,
    ArrayStart,
    ArrayEnd,
    NullValue,
    BooleanTrue,
    BooleanFalse

};

struct Token {
    char ch[100];
    enum TokenType t_type;
};

typedef struct Token Token;

struct Response {
    Token* token_container;
    size_t length;
};

// --- Public Function Declarations ---

/**
 * @brief Converts a TokenType enum to its string representation.
 * @param t_type The TokenType enum value.
 * @return A string literal corresponding to the token type.
 */
const char* enum_to_string(enum TokenType t_type);

void remove_whitespace(char* json_string);


/**
 * @brief Performs lexical analysis on a JSON string.
 * @param json_string The input JSON string to tokenize.
 * @return A Response struct containing the array of tokens and its length.
 */
struct Response lexer(char* json_string);

/**
 * @brief Prints the contents of the token container for debugging purposes.
 * @param token_container A pointer to the array of tokens.
 * @param len The number of tokens in the array.
 */
void debug_print(Token* token_container, int len);

#endif // LEXER_H
