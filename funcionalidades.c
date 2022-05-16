#include "funcionalidades.h"

void conecta_dispositivo(indigo_client *my_client, char* nom_disp){
    Dispositivo *entrada = search(nom_disp);

    while(entrada == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
        entrada = search(nom_disp);
    }

    conecta_dispositivo_disp(my_client, entrada);
}

void conecta_dispositivo_disp(indigo_client *my_client, Dispositivo *disp)
{    
    indigo_property *propiedad_connect;
    propiedad_connect = get_propiedad(disp, CONNECTION_PROPERTY_NAME);

    while (propiedad_connect == NULL){

        // printf("Esperando a propiedad connect\n");
        indigo_usleep(ONE_SECOND_DELAY);
        propiedad_connect = get_propiedad(disp, CONNECTION_PROPERTY_NAME);
    }

    // printf("Ya he conseguido la propiedad connect\n");

    if (!indigo_get_switch(propiedad_connect, CONNECTION_CONNECTED_ITEM_NAME))
    {
        // printf("No estaba conectado\n");

        indigo_device_connect(my_client, disp->nombre);

        // printf("Me conecto yo\n");
    }
    else
    {
        // printf("Ya estaba conectado\n");
    }
}

void conecta_all_cameras(indigo_client *my_client){
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for(int i=0; i<posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        conecta_dispositivo_disp(my_client, get_disp_pos(posicion));
    }
    freeArray(&posiciones);
}

void set_exposure_camera(indigo_client *my_client, char* camara){
    Dispositivo *entrada = search(camara);

    while(entrada == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
        entrada = search(camara);
    }

    set_exposure_camera_disp(my_client, entrada);
}

void set_exposure_camera_disp(indigo_client *my_client, Dispositivo *disp){
    static const char *items[] = {CCD_EXPOSURE_ITEM_NAME};
    static double values[] = {2.0};
    indigo_change_number_property(my_client, disp->nombre, CCD_EXPOSURE_PROPERTY_NAME, 1, items, values);
}

void set_exposure_all_cameras(indigo_client *my_client){
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for(int i=0; i<posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        set_exposure_camera_disp(my_client, get_disp_pos(posicion));
    }
    freeArray(&posiciones);

}