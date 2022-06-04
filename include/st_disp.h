#include "utilidades.h"

#define SIZE 13

//https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm

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
    HashTable memoria_prop;
} Dispositivo;

Dispositivo* stDisp[SIZE];

Dispositivo *search(const char *nombre);
void insert(char * nombre);
void display();

Array get_todo_mismo_tipo(const int type);

Dispositivo *get_disp_pos(const int pos);

void store_update_property(indigo_property *property, Dispositivo* disp);

indigo_property *get_propiedad(Dispositivo *disp, const char* nombre);
