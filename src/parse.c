#include "major2.h"
int strtrim(char** src) {
    if (*src == NULL) return -1;

    char* trimmed = *src;

    // Trim leading whitespace
    while (isspace((unsigned char)*trimmed)) {
        trimmed++;
    }

    // Trim trailing whitespace
    char *end = trimmed + strlen(trimmed) - 1;
    while (end > trimmed && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';

    *src = trimmed;

    return 0;
}

int isStrWhiteSpace(const char* str) {
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return FALSE;  // Not a whitespace character found
        }
        str++;
    }
    return TRUE;  // Only whitespace characters found
}
/**
 * @brief Parses string and allocates memory into an array
 * @param str The String To Parse
 * @param delimiters The symbols to Parse str
 * @return A Null-terminated Array of Strings
*/
char** tokenize(char* str, const char delimiters[]) {
    char* tokenArr[MAX_ARGS];

    // Parse Input
    int token_count = 0;
    char* token;
    char* token_saveptr = NULL;
    token = strtok_r(str, delimiters, &token_saveptr);

    while (token != NULL && token_count < MAX_ARGS) {
        if (isStrWhiteSpace(token) == FALSE) {
            //printf("[%d] Token:%s\n", token_count, token);
            strtrim(&token);

            // Allocate memory for the token and check for allocation failure
            tokenArr[token_count] = strdup(token);
            if (tokenArr[token_count] == NULL) {
                perror("strdup error");
                // Free allocated memory before returning
                for (int i = 0; i < token_count; i++) {
                    free(tokenArr[i]);
                }
                return NULL;
            }

            token_count++;
        }
        token = strtok_r(NULL, delimiters, &token_saveptr);
    }

    // Allocate memory for the final array of tokens
    char** tokens = (char**)malloc((token_count + 1) * sizeof(char*));
    if (tokens == NULL) {
        perror("malloc error");
        // Free allocated memory before returning
        for (int i = 0; i < token_count; i++) {
            free(tokenArr[i]);
        }
        return NULL;
    }

    // Copy tokens to the final array
    memcpy(tokens, tokenArr, token_count * sizeof(char*));
    tokens[token_count] = NULL;

    return tokens;
}

/**
 * @brief free alls tokens created by tokenize
 * @param index The index to start from
 * @param tokens The Tokens to Free
 * @details could leak memory if you dont start at the correct index
 * 
*/
int free_tokens(int index,char** tokens){
    if(tokens == NULL){return EXIT_SUCCESS;}
    for (int i = index; tokens[i] != NULL; i++){free(tokens[i]);} // free each token
    free(tokens); // free the token array
    return EXIT_SUCCESS;
}
