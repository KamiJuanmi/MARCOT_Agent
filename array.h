#include <stdio.h>
#include <stdlib.h>
#include <indigo/indigo_xml.h>

typedef union
{
    char *nombre; // tipo 0 
    indigo_property *propiedad; // tipo 1
} Contenido;

typedef struct
{
    Contenido *array;
    size_t used;
    size_t size;
} Array;


void initArray(Array *a, size_t initialSize);

void insertArray(Array *a, Contenido element);

void insertNombre(Array *a, char* nombre);

void insertPropiedad(Array *a, indigo_property *property);

void freeArray(Array *a);