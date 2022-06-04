#include "../include/st_disp.h"

Dispositivo *search(const char *nombre)
{
    int hashIndex = hashCode(nombre, SIZE);

    if (stDisp[hashIndex] == NULL)
        return NULL;

    // SI HUBIERA COLISIONES
    int s_hash = doubleHash(nombre, SIZE);
    while (stDisp[hashIndex] != NULL)
    {

        if (!strcmp(stDisp[hashIndex]->nombre, nombre))
            return stDisp[hashIndex];

        // go to next cell
        hashIndex += s_hash;

        // wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void insert(char *nombre)
{

    Dispositivo *nuevo = (Dispositivo *)malloc(sizeof(Dispositivo));
    nuevo->nombre = nombre;
    nuevo->type = get_tipo_device(nombre);
    initArray(&nuevo->propiedades, propiedades_tipo[nuevo->type].size);
    initHashTable(&nuevo->memoria_prop, propiedades_tipo[nuevo->type].size);

    int hashIndex = hashCode(nombre, SIZE);

    // Si hubiera colisiones
    int s_hash = doubleHash(nombre, SIZE);
    // move in array until an empty or deleted cell, no va a haber porque nunca se borra
    while (stDisp[hashIndex] != NULL)
    {
        // go to next cell
        hashIndex += s_hash;

        // wrap around the table
        hashIndex %= SIZE;
    }

    stDisp[hashIndex] = nuevo;
}

// Da segmentation fault pero weno ->
void display()
{
    int i = 0;

    for (i = 0; i < SIZE; i++)
    {

        if (stDisp[i] != NULL)
            printf(" %s ", stDisp[i]->nombre);
        else
            printf(" ~~ ");
    }

    printf("\n");
}

Array get_todo_mismo_tipo(const int type)
{
    Array posiciones;
    initArray(&posiciones, 2);
    for(int i=0; i<SIZE; i++)
    {
        if(stDisp[i] != NULL)
        {
            if(stDisp[i]->type == type)
                insertInt(&posiciones, i);
        }
    }
    return posiciones;
}

Dispositivo *get_disp_pos(const int pos){
    return stDisp[pos];
}

void store_update_property(indigo_property *property, Dispositivo *disp)
{
    int posicion_alm = searchIntHash(&disp->memoria_prop, property->name);
    // En el caso de que no este
    if (posicion_alm < 0)
    {
        insertPropiedad(&disp->propiedades, property);
        hashInt(&disp->memoria_prop, property->name, disp->propiedades.used - 1);
    }
    else
    {
        updatePropiedad(&disp->propiedades, property, posicion_alm);
    }
}

indigo_property *get_propiedad(Dispositivo *disp, const char *nombre)
{
    int posicion_alm = searchIntHash(&disp->memoria_prop, nombre);

    // En el caso de que no este
    if (posicion_alm < 0)
    {
        printf("Esa propiedad todavia no ha llegado\n");
    }
    else
    {
        return disp->propiedades.array[posicion_alm].cont.propiedad;
    }
}