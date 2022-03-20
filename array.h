#include <stdio.h>
#include <stdlib.h>
#include <indigo/indigo_xml.h>

typedef union
{
    char *nombre;
    int tipo_disp; // Usado para indicar el tipo de dispositivo con el que estamos trabajando
    indigo_property *propiedad;
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

bool estaAlmacenadoNombre(Array a, char* nombre);

void freeArray(Array *a);