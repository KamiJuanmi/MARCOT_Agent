#include "st_disp.h"

int hashCode(char *nombre)
{
    if (nombre == NULL)
    {

        return 0;
    }
    else if (nombre[0] == '\0')
    {

        return 0;
    }

    char actual;
    int i = 0;

    do
    {
        actual = i[nombre];
        ++i;
    } while (i[nombre] != '\0');

    printf("%d\n", (int)actual - 48);

    return 0;
}

struct Dispositivo *search(char *nombre)
{
    int hashIndex = hashCode(nombre);

    if (!strcmp(hashArray[hashIndex]->nombre, nombre))
            return hashArray[hashIndex];

    return NULL

    /*SI HUBIERA COLISIONES
    // move in array until an empty
    while (hashArray[hashIndex] != NULL)
    {

        if (!strcmp(hashArray[hashIndex]->nombre, nombre))
            return hashArray[hashIndex];

        // go to next cell
        ++hashIndex;

        // wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
    */
}

void insert(char * nombre)
{

    struct Dispositivo *nuevo = (struct Dispositivo *)malloc(sizeof(struct Dispositivo));
    nuevo->nombre = nombre;

    int hashIndex = hashCode(nombre);

    /*Si hubiera colisiones
    // move in array until an empty or deleted cell
    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
    {
        // go to next cell
        ++hashIndex;

        // wrap around the table
        hashIndex %= SIZE;
    }*/

    hashArray[hashIndex] = nuevo;
}

/*
void ld_add(struct Lista_disp* ld, struct Dispositivo disp){

}

struct Dispositivo* nuevo_disp(int n_disp){

}

struct Dispositivo* get_disp(int n_disp){

}*/