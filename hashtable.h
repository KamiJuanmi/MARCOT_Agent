#include <stdio.h>
#include <stdlib.h>
#include "array.h"

typedef union
{
    char *nombre;
    int posicion;
} str_int;

typedef struct 
{
    int uniontype;
    char* key;
    str_int cont; 
} hashableCont;

typedef struct
{
    hashableCont *ht;
    Array memoria_keys;
    size_t used;
    size_t size;
} HashTable;

void initHashTable(HashTable *ht, size_t initialSize);

void insertHashTable(HashTable *ht, char* key, hashableCont element);

void reallocateHashTable(HashTable *ht, hashableCont *cont_antiguo);

int dimePosicion(HashTable *ht, char* key);

void hashNombre(HashTable *ht, char* key, char* nombre);

void hashInt(HashTable *ht, char* key, int elemento);

bool checkeaKey(HashTable ht, char* key);

hashableCont* searchHash(HashTable *ht, char* key);

char* searchNombreHash(HashTable *ht, char* key);

int searchIntHash(HashTable *ht, char* key);

void freeHashTable(HashTable *ht);

void printHashTable(HashTable ht);

void printMemoria(HashTable ht);