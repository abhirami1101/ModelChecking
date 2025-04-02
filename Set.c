#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "kripke.h"
#include "Set.h"





unsigned int hash(char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash % TABLE_SIZE;
}

// Initialize hash set
void initSet(HashSet* set) {
    if (set == NULL) {
        printf("Error: HashSet is NULL\n");
        exit(1);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        set->buckets[i] = NULL;
    }
    set->count = 0;
}


// Check if key exists in set
bool contains(HashSet* set, state* key ) {
    unsigned int index = hash(key->name);
    Node* node = set->buckets[index];
    while (node) {
        if (strcmp(node->key->name, key->name) == 0) return true;
        node = node->next;
    }
    return false;
}

// Insert into set
void add(HashSet* set, state* key) {
    if (set == NULL || key == NULL || key->name == NULL) {
        printf("Error: Invalid state or HashSet\n");
        return;
    }
    if (contains(set, key)) return;
    unsigned int index = hash(key->name);
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->key = key;
    newNode->next = set->buckets[index];
    set->buckets[index] = newNode;
    set->count++;
}


// Remove from set
void removeElement(HashSet* set, state* key) {
    unsigned int index = hash(key->name);
    Node* node = set->buckets[index], *prev = NULL;
    while (node) {
        if (strcmp(node->key->name, key->name) == 0) {
            if (prev) prev->next = node->next;
            else set->buckets[index] = node->next;
            free(node);
            set->count--; 
            return;
        }
        prev = node;
        node = node->next;
    }
}

int hashset_size(HashSet* set) {
    return set->count;
}

void printSet(HashSet* set) {
    printf("{ ");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* node = set->buckets[i];
        while (node) {
            printf("%s ", node->key->name);
            node = node->next;
        }
    }
    printf("}\n");
}
