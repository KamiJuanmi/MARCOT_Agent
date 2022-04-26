#include "utilidades.h"

#define SIZE 13

typedef struct 
{
    char * nombre;
    /*
    El tipo nos indica que tipo de dispositivo es:
    0 -> camara
    1 -> montura por ejemplo
    ...
    */
    int type;
    Array propiedades;
    indigo_property *propiedadConnect;
    indigo_property *propiedadExposicion;
    indigo_property *imagen;
} Dispositivo;

//https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm

Dispositivo* stDisp[SIZE];


Dispositivo *search(const char *nombre);
void insert(char * nombre);
void display();

void almacena_foto(Dispositivo *disp);


