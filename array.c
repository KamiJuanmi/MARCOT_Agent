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
    aux_nuevo.tipo_disp = elemento;
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
            printf("%d", a.array[i].cont.tipo_disp);
            break;
        }
        printf("\t");
    }
    printf("\n");
}