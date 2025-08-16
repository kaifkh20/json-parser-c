#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdio.h>  // For FILE, fopen, etc.
#include <stdlib.h> // For EXIT_FAILURE, malloc, etc.
#include <ctype.h>
#include "parser.h" // For ResponseKV and other data structures
#include "utility_func.h"

/**
 * @brief Parses a JSON file and returns a structured representation.
 *
 * This function reads a JSON file from the specified path, tokenizes it,
 * and then parses the tokens into a ResponseKV object.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return A ResponseKV object representing the parsed JSON data.
 */
ResponseKV json_parser(char* json_string);

#endif // JSON_PARSER_H
