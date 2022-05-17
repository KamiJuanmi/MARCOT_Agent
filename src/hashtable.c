#include "../include/hashtable.h"

void initHashTable(HashTable *ht, size_t initialSize)
{
    ht->ht = malloc(initialSize * sizeof(hashableCont));
    hashableCont nuevo;
    nuevo.cont.posicion = 0;
    nuevo.uniontype = 0;
    nuevo.key = NULL;
    for(int i = 0; i<initialSize; i++)
    {
        ht->ht[i] = nuevo;
    }
    ht->used = 0;
    ht->size = initialSize;
}

void insertHashTable(HashTable *ht, char* key, hashableCont element)
{
    if (ht->used == ht->size)
    {
        ht->size *= 2;
        hashableCont* cont_antiguo = ht->ht;
        reallocateHashTable(ht, cont_antiguo);
    }

    int hashIndex = hashCode(key, ht->size);

    // Si hubiera colisiones
    // move in array until an empty or deleted cell, no va a haber porque nunca se borra
    while (ht->ht[hashIndex].uniontype > 0)
    {
        // go to next cell
        hashIndex++;
        
        // wrap around the table
        hashIndex %= ht->size;
        
    }
    ht->used++;
    ht->ht[hashIndex] = element;
}

void insertHashPrivate(hashableCont *nuevo_alm, int nuevo_tam, char* key, hashableCont element)
{    
    int hashIndex = hashCode(key, nuevo_tam);

    // Si hubiera colisiones
    // move in array until an empty or deleted cell, no va a haber porque nunca se borra
    while (nuevo_alm[hashIndex].uniontype > 0)
    {
        // go to next cell
        hashIndex++;

        // wrap around the table
        hashIndex %= nuevo_tam;
        
    }
    
    nuevo_alm[hashIndex] = element;
}

void reallocateHashTable(HashTable *ht, hashableCont *cont_antiguo)
{
    hashableCont* nuevo_alm = malloc(ht->size * sizeof(hashableCont));
    for(int i = 0; i<ht->used; i++)
    {
        insertHashPrivate(nuevo_alm, ht->size, cont_antiguo[i].key, cont_antiguo[i]);
    }

    
    free(ht->ht);

    ht->ht = nuevo_alm;

}

int dimePosicion(HashTable *ht, char* key)
{
    int hashIndex = hashCode(key, ht->size);


    if (ht->ht[hashIndex].uniontype == 0)
        return -1;

    // SI HUBIERA COLISIONES
    int n_veces = 0;
    while (ht->ht[hashIndex].uniontype > 0 && n_veces<ht->size)
    {
        if (!strcmp(ht->ht[hashIndex].key, key))
            return hashIndex;
        // go to next cell
        hashIndex++;

        // wrap around the table
        hashIndex %= ht->size;
        n_veces ++;
    }

    return -1;
}

hashableCont *searchHash(HashTable *ht, char* key)
{
    int posicion = dimePosicion(ht, key);
    if(posicion < 0)
    {
        return NULL;
    }
    else
        return &(ht->ht[posicion]); 
}

char* searchNombreHash(HashTable *ht, char* key)
{
    hashableCont *nuevo;
    nuevo = searchHash(ht, key);
    if(nuevo == NULL)
        return NULL;

    return nuevo->cont.nombre;
}

int searchIntHash(HashTable *ht, char* key)
{
    hashableCont *nuevo;
    nuevo = searchHash(ht, key);
    if(nuevo == NULL)
        return -1;
    
    return nuevo->cont.posicion;
}

void hashNombre(HashTable *ht, char* key, char *nombre)
{
    hashableCont nuevo;
    str_int aux_nuevo;
    aux_nuevo.nombre = nombre;
    nuevo.cont = aux_nuevo;
    nuevo.uniontype = 1;
    nuevo.key = key;
    insertHashTable(ht, key, nuevo);
}

void hashInt(HashTable *ht, char* key, int elemento)
{
    hashableCont nuevo;
    str_int aux_nuevo;
    aux_nuevo.posicion = elemento;
    nuevo.cont = aux_nuevo;
    nuevo.uniontype = 2;
    nuevo.key = key;
    insertHashTable(ht, key, nuevo);
}

bool checkeaKey(HashTable *ht, char* key){
    hashableCont *nuevo;
    
    nuevo = searchHash(ht, key);
    if(nuevo == NULL)
        return false;

    return true;
}

void freeHashTable(HashTable *ht)
{
    free(ht->ht);
    ht->ht = NULL;
    ht->used = ht->size = 0;
}

void printHashTable(HashTable ht)
{
    printf("\n");
    printf("El tamano es de %i\n", ht.size);
    for(int i=0; i<ht.size; i++)
    {
        if(ht.ht[i].uniontype != 1 && ht.ht[i].uniontype != 2)
        {
            printf(" ~~ \n");
        }else{
            printf(" %s ", ht.ht[i].key);
            switch (ht.ht[i].uniontype)
            {
            case 1: //Es un string
                printf(" %s\n", ht.ht[i].cont.nombre);
                break;
            case 2: //Es un numero
                printf(" %i\n", ht.ht[i].cont.posicion);
                break;
            default:
                break;
            }
        }
    }
    printf("\n");
}