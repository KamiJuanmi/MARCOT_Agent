#include "../include/funcionalidades.h"

void conecta_dispositivo(indigo_client *my_client, char *nom_disp)
{
    Dispositivo *entrada = search(nom_disp);

    while (entrada == NULL)
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

    while (propiedad_connect == NULL)
    {

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

void conecta_all_cameras(indigo_client *my_client)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        conecta_dispositivo_disp(my_client, get_disp_pos(posicion));
    }
    freeArray(&posiciones);
}

void desconecta_dispositivo_disp(indigo_client *my_client, Dispositivo *disp)
{
    indigo_property *propiedad_connect;
    propiedad_connect = get_propiedad(disp, CONNECTION_PROPERTY_NAME);

    while (propiedad_connect == NULL)
    {

        // printf("Esperando a propiedad connect\n");
        indigo_usleep(ONE_SECOND_DELAY);
        propiedad_connect = get_propiedad(disp, CONNECTION_PROPERTY_NAME);
    }

    // printf("Ya he conseguido la propiedad connect\n");

    if (!indigo_get_switch(propiedad_connect, CONNECTION_CONNECTED_ITEM_NAME))
    {
        // printf("No estaba conectado\n");


        // printf("Me conecto yo\n");
    }
    else
    {
        
        indigo_device_disconnect(my_client, disp->nombre);
        // printf("Ya estaba conectado\n");
    }
}

void desconecta_all_cameras(indigo_client *my_client)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        desconecta_dispositivo_disp(my_client, get_disp_pos(posicion));
    }
    freeArray(&posiciones);
}

void set_exposure_camera(indigo_client *my_client, char *camara, const double sec)
{
    Dispositivo *entrada = search(camara);

    while (entrada == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
        entrada = search(camara);
    }

    set_exposure_camera_disp(my_client, entrada, sec);
}

void set_exposure_camera_disp(indigo_client *my_client, Dispositivo *disp, const double sec)
{
    static const char *items[] = {CCD_EXPOSURE_ITEM_NAME};
    static double values[] = {0};
    values[0] = sec;
    indigo_change_number_property(my_client, disp->nombre, CCD_EXPOSURE_PROPERTY_NAME, 1, items, values);
}

void set_exposure_all_cameras(indigo_client *my_client, const double sec)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        set_exposure_camera_disp(my_client, get_disp_pos(posicion), sec);
    }
    freeArray(&posiciones);
}

void set_wait_exposure_cameras(indigo_client *my_client, const double sec)
{
    bool TEST = false;

    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        if (!TEST)
        {
            set_exposure_camera_disp(my_client, get_disp_pos(posicion), sec);
        }
        else
        {
            double aux = 2.0 * (posiciones.used - i);
            set_exposure_camera_disp(my_client, get_disp_pos(posicion), aux);
        }
    }

    Array acabadas;
    initArray(&acabadas, posiciones.used);

    while (acabadas.used < posiciones.used)
    {
        indigo_usleep(ONE_SECOND_DELAY);
        for (int i = 0; i < posiciones.used; i++)
        {
            if (!estaAlmacenadoInt(acabadas, i))
            {
                int posicion = posiciones.array[i].cont.num_array;
                indigo_property *propiedad_exposure;
                propiedad_exposure = get_propiedad(get_disp_pos(posicion), CCD_EXPOSURE_PROPERTY_NAME);
                if (propiedad_exposure != NULL)
                {
                    double valor;
                    valor = propiedad_exposure->items[0].number.value;

                    if (valor < 1e-9)
                    {
                        insertInt(&acabadas, i);
                    }
                }
            }
        }
    }
    freeArray(&acabadas);
    freeArray(&posiciones);
}