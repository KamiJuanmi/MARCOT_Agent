#include "../include/array.h"

int hashCode(const char *nombre, const int size)
{
    // https://stackoverflow.com/questions/25894642/how-to-obtain-the-same-result-of-java-string-hashcode-in-objective-c
    int hash = 0;
    for (int i = 0; i<strlen(nombre); i++)
    {
        hash = (31*hash) + nombre[i];
    }
    hash %= size;
    if(hash <0)
        hash += size;
    return hash;
}

int doubleHash(const char *nombre, const int size)
{
    int hash = 0;
    for (int i = 0; i<strlen(nombre); i++)
    {
        hash = (61*hash) + nombre[i];
    }
    hash %= size;
    if(hash <0)
        hash += size;
    return hash;
}

void initArray(Array *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(Contenido));
    a->used = 0;
    a->size = initialSize;
}

void insertArray(Array *a, Contenido element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(Contenido));
    }
    a->array[a->used++] = element;
}

void insertNombre(Array *a, char *nombre)
{
    Contenido nuevo;
    Un_Contenido aux_nuevo;
    aux_nuevo.nombre = nombre;
    nuevo.cont = aux_nuevo;
    nuevo.uniontype = 0;
    insertArray(a, nuevo);
}

void insertInt(Array *a, int elemento)
{
    Contenido nuevo;
    Un_Contenido aux_nuevo;
    aux_nuevo.num_array = elemento;
    nuevo.cont = aux_nuevo;
    nuevo.uniontype = 1;
    insertArray(a, nuevo);
}

void insertPropiedad(Array *a, indigo_property *property)
{
    Contenido nuevo;
    Un_Contenido aux_nuevo;
    aux_nuevo.propiedad = property;
    nuevo.cont = aux_nuevo;
    nuevo.uniontype = 2;
    insertArray(a, nuevo);
}

void updatePropiedad(Array *a, indigo_property *property, int posicion_propiedad)
{
    Contenido nuevo;
    Un_Contenido aux_nuevo;
    aux_nuevo.propiedad = property;
    nuevo.cont = aux_nuevo;
    nuevo.uniontype = 2;
    a->array[posicion_propiedad] = nuevo;
}

bool estaAlmacenadoNombre(Array a, char *nombre)
{
    for (int i = 0; i < a.used; i++)
    {
        if(a.array[i].uniontype!=0)
            continue;
        for (int j = 0; j < strlen(nombre) && j < strlen(a.array[i].cont.nombre); j++)
        {
            if (!(nombre[j] == a.array[i].cont.nombre[j]))
            {
                break;
            }
            if(nombre[j] == '@')
            {
                return true;
            }
        }
    }
    return false;
}

bool estaAlmacenadoInt(Array a, int elemento)
{
    for (int i = 0; i < a.used; i++)
    {
        if(a.array[i].uniontype!=1)
            continue;

        // printf("El elemento a  comparar es este %i, comparandose con %i\n", elemento, a.array[i].cont.num_array);
        if(elemento == a.array[i].cont.num_array)
            return true;
    }
    return false;
}

int getPosicionNombre(Array a, char *nombre)
{
    for (int i = 0; i < a.used; i++)
    {
        if(a.array[i].uniontype!=0)
            continue;
        for (int j = 0; j < strlen(nombre) && j < strlen(a.array[i].cont.nombre); j++)
        {
            if (!(nombre[j] == a.array[i].cont.nombre[j]))
            {
                break;
            }
            if(nombre[j] == '@')
            {
                return i;
            }
        }
    }
    return -1;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void printArray(Array a)
{
    for(int i = 0; i < a.used; i++)
    {
        switch (a.array[i].uniontype)
        {
        case 0:
            printf("%s", a.array[i].cont.nombre);
            break;
        case 1:
            printf("%d", a.array[i].cont.num_array);
            break;
        }
        printf("\t");
    }
    printf("\n");
}