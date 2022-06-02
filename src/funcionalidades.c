#include "../include/funcionalidades.h"

int n_camaras;

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
        indigo_usleep(ONE_SECOND_DELAY*0.3);
        propiedad_connect = get_propiedad(disp, CONNECTION_PROPERTY_NAME);
    }


    if (!indigo_get_switch(propiedad_connect, CONNECTION_CONNECTED_ITEM_NAME))
    {
        indigo_device_connect(my_client, disp->nombre);
    }
}

void conecta_all_cameras(indigo_client *my_client)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    n_camaras = posiciones.used;
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
        indigo_usleep(ONE_SECOND_DELAY*0.3);
        propiedad_connect = get_propiedad(disp, CONNECTION_PROPERTY_NAME);
    }

    if (indigo_get_switch(propiedad_connect, CONNECTION_CONNECTED_ITEM_NAME))
    {
        indigo_device_disconnect(my_client, disp->nombre);
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

void conecta_montura(indigo_client *my_client)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(1);
    int posicion = posiciones.array[0].cont.num_array;

    conecta_dispositivo_disp(my_client, get_disp_pos(posicion));
    freeArray(&posiciones);
}

void desconecta_montura(indigo_client *my_client)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(1);
    int posicion = posiciones.array[0].cont.num_array;

    desconecta_dispositivo_disp(my_client, get_disp_pos(posicion));
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

void set_ganancia_camera_disp(indigo_client *my_client, Dispositivo *disp, const double ganancia)
{
    static const char *items[] = {CCD_GAIN_ITEM_NAME};
    static double values[] = {0};
    values[0] = ganancia;
    indigo_change_number_property(my_client, disp->nombre, CCD_GAIN_PROPERTY_NAME, 1, items, values);
}

void set_ganancia_all_cameras(indigo_client *my_client, const double ganancia)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        set_ganancia_camera_disp(my_client, get_disp_pos(posicion), ganancia);
    }
    freeArray(&posiciones);
}

void set_cooler_status_camera_disp(indigo_client *my_client, Dispositivo *disp, bool value)
{
    if (value)
        indigo_change_switch_property_1(my_client, disp->nombre, CCD_COOLER_PROPERTY_NAME, CCD_COOLER_ON_ITEM_NAME, value);
    else
        indigo_change_switch_property_1(my_client, disp->nombre, CCD_COOLER_PROPERTY_NAME, CCD_COOLER_OFF_ITEM_NAME, !(value));
}

void set_cooler_status_all_cameras(indigo_client *my_client, bool value)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        set_cooler_status_camera_disp(my_client, get_disp_pos(posicion), value);
    }
    freeArray(&posiciones);
}

void set_cooler_temp_camera_disp(indigo_client *my_client, Dispositivo *disp, const double temp)
{
    static const char *items[] = {CCD_TEMPERATURE_ITEM_NAME};
    static double values[] = {0};
    values[0] = temp;
    indigo_change_number_property(my_client, disp->nombre, CCD_TEMPERATURE_PROPERTY_NAME, 1, items, values);
}

void set_cooler_temp_all_cameras(indigo_client *my_client, const double temp)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        set_cooler_temp_camera_disp(my_client, get_disp_pos(posicion), temp);
    }
    freeArray(&posiciones);
}

void set_RA_coord_montura(indigo_client *my_client, const double coord)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(1);
    int posicion = posiciones.array[0].cont.num_array;
    static const char *items[] = {MOUNT_EQUATORIAL_COORDINATES_RA_ITEM_NAME};
    static double values[] = {0};
    values[0] = coord;
    indigo_change_number_property(my_client, get_disp_pos(posicion)->nombre, MOUNT_EQUATORIAL_COORDINATES_PROPERTY_NAME, 1, items, values);
}

void set_DEC_coord_montura(indigo_client *my_client, const double coord)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(1);
    int posicion = posiciones.array[0].cont.num_array;
    static const char *items[] = {MOUNT_EQUATORIAL_COORDINATES_DEC_ITEM_NAME};
    static double values[] = {0};
    values[0] = coord;
    indigo_change_number_property(my_client, get_disp_pos(posicion)->nombre, MOUNT_EQUATORIAL_COORDINATES_PROPERTY_NAME, 1, items, values);
}

void set_parking_montura(indigo_client *my_client, const bool value)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(1);
    int posicion = posiciones.array[0].cont.num_array;
    if (value)
        indigo_change_switch_property_1(my_client, get_disp_pos(posicion)->nombre, MOUNT_PARK_PROPERTY_NAME, MOUNT_PARK_PARKED_ITEM_NAME, value);
    else
        indigo_change_switch_property_1(my_client, get_disp_pos(posicion)->nombre, MOUNT_PARK_PROPERTY_NAME, MOUNT_PARK_UNPARKED_ITEM_NAME, !(value));
}