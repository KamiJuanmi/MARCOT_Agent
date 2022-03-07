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