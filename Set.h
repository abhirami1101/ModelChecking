#ifndef SET_H
#define SET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "kripke.h"

#define TABLE_SIZE 101

typedef struct Node {
    state* key;
    struct Node* next;
} Node;

typedef struct {
    Node* buckets[TABLE_SIZE];
} HashSet;

unsigned int hash(char* str);
void initSet(HashSet* set);
bool contains(HashSet* set, state* key);
void add(HashSet* set, state* key);
void removeElement(HashSet* set, state* key);
void printSet(HashSet* set);


#endif