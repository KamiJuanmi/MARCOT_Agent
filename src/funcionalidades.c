#include "../include/funcionalidades.h"

int n_camaras;

indigo_result connect_all_dev(indigo_server_entry **server)
{
    int port_ini = 8000;
    char host[9] = "indigo_x";

    for (int i = 0; i < num_devices; i++)
    {
        host[7] = i + '0';
        indigo_connect_server(host, host, port_ini + i, server);
    }
    indigo_usleep(0.1 * ONE_SECOND_DELAY);
}

indigo_result connect_config(indigo_server_entry **server)
{
    for (int i = 0; i < host_puertos.used; i += 2)
    {
        char *host = host_puertos.array[i].cont.nombre;
        int puerto = host_puertos.array[i + 1].cont.num_array;
        indigo_connect_server(host, host, puerto, server);
    }
    indigo_usleep(0.1 * ONE_SECOND_DELAY);
}

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
        indigo_usleep(ONE_SECOND_DELAY * 0.3);
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
    if (posiciones.used == 0)
        return;
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
        indigo_usleep(ONE_SECOND_DELAY * 0.3);
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
    int posicion = posiciones.array[0].cont.num_array;

    conecta_dispositivo_disp(my_client, get_disp_pos(posicion));
    freeArray(&posiciones);
}

void desconecta_montura(indigo_client *my_client)
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(1);
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
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
    if (posiciones.used == 0)
        return;
    int posicion = posiciones.array[0].cont.num_array;
    if (value)
        indigo_change_switch_property_1(my_client, get_disp_pos(posicion)->nombre, MOUNT_PARK_PROPERTY_NAME, MOUNT_PARK_PARKED_ITEM_NAME, value);
    else
        indigo_change_switch_property_1(my_client, get_disp_pos(posicion)->nombre, MOUNT_PARK_PROPERTY_NAME, MOUNT_PARK_UNPARKED_ITEM_NAME, !(value));
}

void almacena_foto_disp(Dispositivo *disp, int n_disp)
{
    indigo_property *property = get_propiedad(disp, CCD_IMAGE_PROPERTY_NAME);
    /* URL blob transfer is available only in client - server setup.
           This will never be called in case of a client loading a driver. */
    if (*property->items[0].blob.url && indigo_populate_http_blob_item(&property->items[0]))
        indigo_log("image URL received (%s, %d bytes)...", property->items[0].blob.url, property->items[0].blob.size);
    
        
    if (property->items[0].blob.value)
    {
        char name[50];
        sprintf(name, "./img/img_disp_%i.jpeg", n_disp);
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

void almacena_todas_las_fotos()
{
    Array posiciones;
    posiciones = get_todo_mismo_tipo(0);
    if (posiciones.used == 0)
        return;
    for (int i = 0; i < posiciones.used; i++)
    {
        int posicion = posiciones.array[i].cont.num_array;
        almacena_foto_disp(get_disp_pos(posicion), i);
    }
    
    freeArray(&posiciones);
    procesa_fotos();
}

void procesa_fotos()
{
    char *comando = "montage -label \%f -frame 7 -background '#451080' -geometry +2+2 -resize 10\% img/img_disp_*.jpeg img/img_proc.jpeg";
    int error = system(comando);
}
