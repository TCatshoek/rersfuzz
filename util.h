//
// Created by tom on 12/16/19.
//
#ifndef RERSFUZZ_UTIL_H
#define RERSFUZZ_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <cwalk.h>
#include <string.h>
#include <regex.h>
#include <assert.h>

char** get_alphabet(const char* path) {
    FILE* fp;
    char* line;
    size_t len = 0;
    ssize_t read;

    char** result;

    // Get filename of source file by appending ".c"
    char cpath[strlen(path) + 3];
    strcpy(cpath, path);
    strcat(cpath, ".c");

    // Compile regex
    regex_t regex;
    if (regcomp(&regex, "int inputs\\[\\] \\= {\\(.*\\)};", 0)) {
        perror("Failed to compile regex");
        exit(-1);
    }

    fp = fopen(cpath, "r");
    if (fp == NULL) {
        perror("Couldn't open source file");
        exit(-1);
    }

    regmatch_t pmatch[2];
    char* str_1;
    char* tmp;
    bool found = false;
    while (read != -1) {
        read = getline(&line, &len, fp);
        if (!regexec(&regex, line, 2, pmatch, 0)) {
            // match, get the first group
            str_1 = line + pmatch[1].rm_so;
            str_1[pmatch[1].rm_eo - pmatch[1].rm_so] = 0;

            printf("%s\n", str_1);

            // Count |A|
            int count = 0;
            int idx = 0;
            while(str_1[idx] != 0) {
                if (str_1[idx] == ',') {
                    count++;
                }
                idx++;
            }
            count++;

            // Allocate array for storing the alphabet tokens
            result = (char**) malloc(sizeof(char*) * (count + 1));
            result[count] = NULL;

            // Copy the alphabet tokens to memory
            tmp = strtok(str_1, ",");
            idx = 0;
            while (tmp != NULL) {
                result[idx] = strdup(tmp);
                printf("%s\n", tmp);
                tmp = strtok(NULL, ",");
                idx++;
            }

            // Alphabet found, we are done
            found = true;
            break;
        }
    }

    assert(found);

    free(line);
    fclose(fp);
    return result;
}

void displayalphabet(const char** alphabet) {
    printf("Using alphabet: [");
    int i = 0;
    while (alphabet[i] != NULL) {
        printf("%s", alphabet[i]);
        if (alphabet[i + 1] != NULL) {
            printf(", ");
        }
        i++;
    }
    printf("]\n");
}

#endif //RERSFUZZ_UTIL_H
