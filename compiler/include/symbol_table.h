#pragma once

typedef struct {
  char *name;
} SymbolEntry;

// Hash Table Entry (linked list bucket for collisions)
typedef struct SymbolNode {
  SymbolEntry entry;
  struct SymbolNode *next;
} SymbolNode;

#define SYMBOL_TABLE_SIZE 64 // change when there is a larger program

typedef struct {
  SymbolNode *buckets[SYMBOL_TABLE_SIZE];
} SymbolTable;

SymbolTable *create_symbol_table();
void free_symbol_table(SymbolTable *table);

unsigned int djb2hash(const char *str);

int add_symbol(SymbolTable *table, const char *name);
int symbol_exists(SymbolTable *table, const char *name);
void print_symbol_table(SymbolTable *table);
