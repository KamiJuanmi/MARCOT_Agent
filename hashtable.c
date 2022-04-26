#include "hashtable.h"

void initHashTable(HashTable *ht, size_t initialSize)
{
    ht->ht = malloc(initialSize * sizeof(hashableCont));
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
    while (ht->ht[hashIndex].uniontype > 0)
    {
        if (!strcmp(ht->ht[hashIndex].key, key))
            return hashIndex;
        // go to next cell
        hashIndex++;

        // wrap around the table
        hashIndex %= ht->size;
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

void freeHashTable(HashTable *ht)
{
    free(ht->ht);
    ht->ht = NULL;
    ht->used = ht->size = 0;
}

void printHashTable(HashTable ht)
{
    printf("\n");
    for(int i=0; i<ht.size; i++)
    {
        if(ht.ht[i].uniontype != 1 && ht.ht[i].uniontype != 2)
        {
            printf(" ~~ ");
        }else{
            switch (ht.ht[i].uniontype)
            {
            case 1: //Es un string
                printf(" %s ", ht.ht[i].cont.nombre);
                break;
            case 2: //Es un numero
                printf(" %i ", ht.ht[i].cont.posicion);
                break;
            default:
                break;
            }
        }
    }
    printf("\n");
}

void printAUX(HashTable ht, hashableCont *cont_antiguo)
{
    printf("\n");
    for(int i=0; i<ht.used; i++)
    {
        if(cont_antiguo[i].uniontype != 1 && cont_antiguo[i].uniontype != 2)
        {
            printf(" ~~ ");
        }else{
            switch (cont_antiguo[i].uniontype)
            {
            case 1: //Es un string
                printf(" %s ", cont_antiguo[i].cont.nombre);
                break;
            case 2: //Es un numero
                printf(" %i ", cont_antiguo[i].cont.posicion);
                break;
            default:
                break;
            }
        }
    }
    printf("\n");
}
