#ifndef cinder_table_h
#define cinder_table_h

#include "common.h"
#include "value.h"

// a hash table entry is a key-value pair.
typedef struct {
  ObjString* key;
  Value value;
} Entry;

// a hash table is a dynamically sized array of hash table entries.
typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

// initialize the given hash table.
void init_table(Table* table);

// free the memory from the given hash table.
void free_table(Table* table);

// get the value associated with the given key from the given hash table.
bool table_get(Table* table, ObjString* key, Value* value);

// add a key-value pair to the given hash table.
bool table_set(Table* table, ObjString* key, Value value);

// delete the key-value pair associated with the given key from the given hash table.
bool table_delete(Table* table, ObjString* key);

// copy all the values from one hash table to another.
void table_add_all(Table* from, Table* to);

// find the string in the given hash table.
ObjString* table_find_string(Table* table, const char* chars, int length, uint32_t hash);

void table_remove_white(Table* table);

void mark_table(Table* table);

// delete the key-value pair associated with the given key from the given hash table.
bool table_delete(Table* table, ObjString* key);

#endif // cinder_table_h
