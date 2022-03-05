#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <indigo/indigo_bus.h>
#include <indigo/indigo_client.h>
#include <indigo/indigo_xml.h>

#define SIZE 10

struct Dispositivo
{
    char * nombre = NULL;
    indigo_property *propiedadConnect = NULL;
    indigo_property *propiedadExposicion = NULL;
    indigo_property *imagen = NULL;
}

//https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm

struct Dispositivo* hashArray[SIZE];

int hashCode(char * nombre);
struct Dispositivo *search(char *nombre);
void insert(char * nombre);

/*
struct Lista_disp
{
    struct Dispositivo *dsp;
    struct Lista_disp *prev, *next;
}

//Funciones
void ld_add(struct Lista_disp*, struct Dispositivo); // Anade un nuevo dispositivo al final de la lista
struct Dispositivo* nuevo_disp(int); // Crea un nuevo dispositivo
struct Dispositivo* get_disp(int); // Devuelve el dispositivo en la posicion pasada
*/