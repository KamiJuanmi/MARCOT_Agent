#include "array.h"

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
    nuevo.nombre = nombre;
    insertArray(a, nuevo);
}

void insertPropiedad(Array *a, indigo_property *property)
{
    Contenido nuevo;
    nuevo.propiedad = property;
    insertArray(a, nuevo);
}

bool estaAlmacenadoNombre(Array a, char *nombre)
{
    for (int i = 0; i < a.used; i++)
    {
        for (int j = 0; j < strlen(nombre) && j < strlen(a.array[i].nombre); j++)
        {
            if (!(nombre[j] == a.array[i].nombre[j]))
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

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}