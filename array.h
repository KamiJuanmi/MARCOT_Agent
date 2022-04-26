#include <stdio.h>
#include <stdlib.h>
#include <indigo/indigo_xml.h>

typedef union
{
    char *nombre;
    int num_array; // Usado para indicar el tipo de dispositivo con el que estamos trabajando
    indigo_property *propiedad;
} Un_Contenido;

typedef struct 
{
    int uniontype;
    Un_Contenido cont;
} Contenido;

typedef struct
{
    Contenido *array;
    size_t used;
    size_t size;
} Array;

int hashCode(const char* key, const int size);

int doubleHash(const char* key, const int size);

void initArray(Array *a, size_t initialSize);

void insertArray(Array *a, Contenido element);

void insertNombre(Array *a, char* nombre);

void insertInt(Array *a, int elemento);

void insertPropiedad(Array *a, indigo_property *property);

void updatePropiedad(Array *a, indigo_property *property, int posicion_propiedad);

bool estaAlmacenadoNombre(Array a, char* nombre);

int getPosicionNombre(Array a, char *nombre);

void freeArray(Array *a);

void printArray(Array a);