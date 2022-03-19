#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <indigo/indigo_bus.h>
#include <indigo/indigo_client.h>
#include <indigo/indigo_xml.h>

#define SIZE 10

struct Dispositivo
{
    char * nombre;
    indigo_property *propiedadConnect;
    indigo_property *propiedadExposicion;
    indigo_property *imagen;
};

//https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm

struct Dispositivo* stDisp[SIZE];

int hashCode(const char * nombre);
struct Dispositivo *search(const char *nombre);
void insert(char * nombre);
void display();

void almacena_foto(struct Dispositivo *disp);

struct Lista_prop
{
    indigo_property *prop;
    struct Lista_prop *prev, *next;
};

//Funciones
void almacena_propiedad();
