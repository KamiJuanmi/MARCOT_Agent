#include "st_disp.h"


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
        hashIndex+=s_hash;

        // wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
    
}

void insert(char *nombre)
{

    Dispositivo *nuevo = (Dispositivo *)malloc(sizeof(Dispositivo));
    initArray(&nuevo->propiedades, 2);
    nuevo->nombre = nombre;

    // printf("%s\n",nuevo->nombre);

    int hashIndex = hashCode(nombre, SIZE);

    // Si hubiera colisiones
    int s_hash = doubleHash(nombre, SIZE);
    // move in array until an empty or deleted cell, no va a haber porque nunca se borra
    while (stDisp[hashIndex] != NULL)
    {
        // go to next cell
        hashIndex+=s_hash;

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

void almacena_foto(Dispositivo *disp)
{
    indigo_property *property = disp->imagen;
    /* URL blob transfer is available only in client - server setup.
           This will never be called in case of a client loading a driver. */
    if (*property->items[0].blob.url && indigo_populate_http_blob_item(&property->items[0]))
        indigo_log("image URL received (%s, %d bytes)...", property->items[0].blob.url, property->items[0].blob.size);

    if (property->items[0].blob.value)
    {
        int n_disp = hashCode(disp->nombre, SIZE);
        char name[32];
        sprintf(name, "img_disp_%02d.jpeg", n_disp);
        FILE *f = fopen(name, "wb");
        fwrite(property->items[0].blob.value, property->items[0].blob.size, 1, f);
        fclose(f);
        indigo_log("image saved to %s...", name);
        /* In case we have URL BLOB transfer we need to release the blob ourselves */
        if (*property->items[0].blob.url)
        {
            free(property->items[0].blob.value);
            property->items[0].blob.value = NULL;
        }
    }
}
