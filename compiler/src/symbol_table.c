#include "symbol_table.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolTable* create_symbol_table() {
    SymbolTable* table = calloc(1, sizeof(SymbolTable));
    if (!table) {
        panic("Error: failed to allocate memory for SymbolTable\n");
    }

    return table;
}

void free_symbol_table(SymbolTable* table) {
    if (!table)
        return;

    for (int i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
        SymbolNode* node = table->buckets[i];
        while (node) {
            SymbolNode* next = node->next;

            // Free name if dynamically allocated
            free(node->entry.name);

            free(node);
            node = next;
        }
    }

    free(table);
}

// string hashing function
unsigned int djb2hash(const char* str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash % SYMBOL_TABLE_SIZE;
}

int add_symbol(SymbolTable* table, const char* name) {
    if (!table || !name)
        return 0;

    unsigned int index = djb2hash(name);
    SymbolNode* node = table->buckets[index];

    // Check if symbol already exists
    while (node) {
        if (strcmp(node->entry.name, name) == 0) {
            return 1; // already exists
        }
        node = node->next;
    }

    // Create and insert new node at the head
    SymbolNode* new_node = malloc(sizeof(SymbolNode));
    if (!new_node) {
        panic("Error: failed to allocate memory for SymbolNode\n");
    }

    new_node->entry.name = strdup(name);
    if (!new_node->entry.name) {
        free(new_node);
        panic("Error: failed to allocate memory for symbol name\n");
    }

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;

    return 1;
}

int symbol_exists(SymbolTable* table, const char* name) {
    if (!table || !name)
        return 0;

    unsigned int index = djb2hash(name);
    SymbolNode* node = table->buckets[index];

    while (node) {
        if (strcmp(name, node->entry.name) == 0) {
            return 1;
        }
        node = node->next;
    }

    return 0;
}

void print_symbol_table(SymbolTable* table) {
    if (!table) {
        printf("Symbol table is NULL\n");
        return;
    }

    printf("=== Symbol Table ===\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
        SymbolNode* node = table->buckets[i];
        if (!node)
            continue; // Skip empty buckets

        printf("Bucket [%d]: ", i);
        while (node) {
            printf("\"%s\"", node->entry.name);
            if (node->next) {
                printf(" -> ");
            }
            node = node->next;
        }
        printf("\n");
    }
    printf("====================\n");
}
