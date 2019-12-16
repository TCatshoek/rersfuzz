//
// Created by tom on 12/16/19.
//
// Allows storing strings in memory dynamically, making sure each string is only stored once
// Memory is never freed, as this doesn't really matter for our use case.

#ifndef RERSFUZZ_STRINGSTORAGE_H
#define RERSFUZZ_STRINGSTORAGE_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

unsigned long djb2(unsigned char*);

struct stringstorage {
    char** storageindex;
    int storagelen;
};

struct stringstorage* strst_init(int size){
    // Init memory stuff
    struct stringstorage* s = (struct stringstorage*) malloc(sizeof(struct stringstorage));
    s->storagelen = size;
    s->storageindex = (char**) malloc(sizeof(char*) * size);

    // Zero out unoccupied memory spots
    for (int i = 0; i < size; i++){
        s->storageindex[i] = NULL;
    }

    return s;
}

// Attempts to store a string and return it's pointer.
// If an identical string is already stored,
// A pointer to it will be returned instead
char* strst_put(struct stringstorage* s, char* string) {

    // Hash the string and calculate the index offset
    u_int offset = (u_int) (djb2(string) % s->storagelen);

    // See if this string is already in memory
    char* index = s->storageindex[offset];

    // Not in memory yet
    if (index == NULL) {
        // Store string in the heap
        s->storageindex[offset] = strdup(string);
    }
    // Spot already occupied
    else {
        // If the string is already stored
        if (strcmp(string, index) == 0) {
            return index;
        }
        // Spot occupied, but not the string we are looking for
        else {
            // Scan until we find the next empty spot, or the string we are looking for
            int retries = 0;
            while(index != NULL && (strcmp(string, index) != 0)){
                offset = (offset + 1) % s->storagelen;
                index = s->storageindex[offset];

                // Make sure we don't endlessly keep looping if there are no free spots anymore
                retries++;
                assert(retries <= s->storagelen);
            }
            if (index == NULL) {
                // Store string in the heap
                s->storageindex[offset] = strdup(string);
            } else{
                return index;
            }
        }
    }
}

// djb2 String hashing from https://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned long djb2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

// Random string generator for testing purposes
static char* rand_string(char* str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

//s = strst_init(1024);
//
//size_t len = 10;
//char* buf[len];
//
//for (int i = 0; i < 1024; i++) {
//rand_string(buf, len);
//strst_put(s, buf);
//}
//
//printf("yay");

#endif //RERSFUZZ_STRINGSTORAGE_H
