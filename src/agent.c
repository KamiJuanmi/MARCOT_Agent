#define DRIVER_VERSION 0x0000
#define DRIVER_NAME "Juanmi driver"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>

#include <indigo/indigo_driver_xml.h>

#include "../include/agent.h"

static marcot_private_data *private_data = NULL;
static indigo_device *agent = NULL;
static indigo_client *agent_client = NULL;

int posicion_url = 0;
int act_exp = 0, act_power = 0, act_temp = 0, act_status = 0;

Array prop_disp;

static indigo_result agent_enumerate_properties(indigo_device *device, indigo_client *client, indigo_property *property);
static indigo_result oculta_x_disp(indigo_device *device, int value);

static indigo_result agent_define_conf_properties(indigo_device *device)
{
    indigo_define_property(device, N_DISP_PROPERTY, NULL);
}

static indigo_result agent_delete_conf_properties(indigo_device *device)
{
    N_DISP_PROPERTY->items->number.value = 0;
    indigo_update_property(device, N_DISP_PROPERTY, NULL);
    indigo_delete_property(device, N_DISP_PROPERTY, NULL);
    for (int i = 0; i < PRIVATE_DATA->current_disp; i++)
        oculta_x_disp(device, i);
    PRIVATE_DATA->current_disp = 0;
}

static indigo_result init_disp_properties(indigo_device *device)
{
    initArray(&prop_disp, 7);
    DISP_0_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 0", MAIN_GROUP, "Dispositivo 0", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_0_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_0_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_0_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_1_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 1", MAIN_GROUP, "Dispositivo 1", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_1_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_1_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_1_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_2_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 2", MAIN_GROUP, "Dispositivo 2", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_2_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_2_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_2_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_3_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 3", MAIN_GROUP, "Dispositivo 3", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_3_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_3_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_3_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_4_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 4", MAIN_GROUP, "Dispositivo 4", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_4_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_4_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_4_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_5_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 5", MAIN_GROUP, "Dispositivo 5", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_5_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_5_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_5_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_6_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 6", MAIN_GROUP, "Dispositivo 6", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_6_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_6_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_6_PROPERTY->items + 1, "Port", "Port", NULL);
    DISP_7_PROPERTY = indigo_init_text_property(NULL, device->name, "Dispositivo 7", MAIN_GROUP, "Dispositivo 7", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
    if (DISP_7_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_text_item(DISP_7_PROPERTY->items + 0, "Host", "Host", NULL);
    indigo_init_text_item(DISP_7_PROPERTY->items + 1, "Port", "Port", NULL);

    insertPropiedad(&prop_disp, DISP_0_PROPERTY);
    insertPropiedad(&prop_disp, DISP_1_PROPERTY);
    insertPropiedad(&prop_disp, DISP_2_PROPERTY);
    insertPropiedad(&prop_disp, DISP_3_PROPERTY);
    insertPropiedad(&prop_disp, DISP_4_PROPERTY);
    insertPropiedad(&prop_disp, DISP_5_PROPERTY);
    insertPropiedad(&prop_disp, DISP_6_PROPERTY);
    insertPropiedad(&prop_disp, DISP_7_PROPERTY);
}

static indigo_result muestra_x_disp(indigo_device *device, int value)
{
    indigo_define_property(device, prop_disp.array[value].cont.propiedad, NULL);
}

static indigo_result oculta_x_disp(indigo_device *device, int value)
{
    indigo_delete_property(device, prop_disp.array[value].cont.propiedad, NULL);
}

static indigo_result agent_habilita_conf_properties(indigo_device *device, bool value)
{
    if (value)
    {
        CONF_PREV_PROPERTY->state = INDIGO_IDLE_STATE;
        N_DISP_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_0_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_1_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_2_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_3_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_4_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_5_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_6_PROPERTY->state = INDIGO_IDLE_STATE;
        DISP_7_PROPERTY->state = INDIGO_IDLE_STATE;
    }
    else
    {
        CONF_PREV_PROPERTY->state = INDIGO_OK_STATE;
        N_DISP_PROPERTY->state = INDIGO_OK_STATE;
        DISP_0_PROPERTY->state = INDIGO_OK_STATE;
        DISP_1_PROPERTY->state = INDIGO_OK_STATE;
        DISP_2_PROPERTY->state = INDIGO_OK_STATE;
        DISP_3_PROPERTY->state = INDIGO_OK_STATE;
        DISP_4_PROPERTY->state = INDIGO_OK_STATE;
        DISP_5_PROPERTY->state = INDIGO_OK_STATE;
        DISP_6_PROPERTY->state = INDIGO_OK_STATE;
        DISP_7_PROPERTY->state = INDIGO_OK_STATE;
    }
    indigo_update_property(device, CONF_PREV_PROPERTY, NULL);
    indigo_update_property(device, N_DISP_PROPERTY, NULL);
    indigo_update_property(device, DISP_0_PROPERTY, NULL);
    indigo_update_property(device, DISP_1_PROPERTY, NULL);
    indigo_update_property(device, DISP_2_PROPERTY, NULL);
    indigo_update_property(device, DISP_3_PROPERTY, NULL);
    indigo_update_property(device, DISP_4_PROPERTY, NULL);
    indigo_update_property(device, DISP_5_PROPERTY, NULL);
    indigo_update_property(device, DISP_6_PROPERTY, NULL);
    indigo_update_property(device, DISP_7_PROPERTY, NULL);
}

static indigo_result crea_prop_img_proc(indigo_device *device)
{
    IMG_PROC_PROPERTY = indigo_init_blob_property(NULL, device->name, "Imagen procesada", GRUPO_CONTROL_CAMARA, "Imagen procesada", INDIGO_OK_STATE, 1);
    if (IMG_PROC_PROPERTY == NULL)
        return INDIGO_FAILED;
    indigo_init_blob_item(IMG_PROC_PROPERTY_ITEM, "Imagen procesada", "Imagen procesada");
    indigo_define_property(device, IMG_PROC_PROPERTY, NULL);

    char *file_name = "img/img_proc.jpeg";
    FILE *in = fopen(file_name, "rb");

    fseek(in, 0, SEEK_END);
    long fsize = ftell(in);
    fseek(in, 0, SEEK_SET);

    char buf[fsize];
    size_t bytes_read;

    bytes_read = fread(buf, 1, fsize, in);
    if (bytes_read != fsize)
        return;
        
    IMG_PROC_PROPERTY_ITEM->blob.value=buf;

    IMG_PROC_PROPERTY_ITEM->blob.size = fsize;
    strcpy(IMG_PROC_PROPERTY_ITEM->blob.format, ".jpeg");

    IMG_PROC_PROPERTY->state = INDIGO_OK_STATE;

    fclose(in);

    indigo_update_property(device, IMG_PROC_PROPERTY, NULL);
}

static indigo_result agent_attach(indigo_device *device)
{
    assert(device != NULL);
    assert(PRIVATE_DATA != NULL);
    assert(read_agent_conf());
    assert(!read_json_client());
    PRIVATE_DATA->current_disp = 0;
    if (indigo_device_attach(device, DRIVER_NAME, DRIVER_VERSION, 0) == INDIGO_OK)
    {
        CONFIG_PROPERTY->hidden = true;
        PROFILE_PROPERTY->hidden = true;
        indigo_set_log_level(INDIGO_LOG_DEBUG);
        // -------------------------------------------------------------------------------- Inicio la propiedad exposicion
        EXPOSICION_PROPERTY = indigo_init_number_property(EXPOSICION_PROPERTY, device->name, "Exposicion", GRUPO_CONTROL_CAMARA, "Empezar exposicion", INDIGO_OK_STATE, INDIGO_RW_PERM, 1);
        if (EXPOSICION_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_number_item(EXPOSICION_PROPERTY_ITEM, EXPOSICION_PROPERTY_NAME, "Empezar exposicion", 0, 10000, 1, 0);
        strcpy(EXPOSICION_PROPERTY_ITEM->number.format, "%g");
        // -------------------------------------------------------------------------------- Inicio la propiedad de ganancia
        GANANCIA_PROPERTY = indigo_init_number_property(GANANCIA_PROPERTY, device->name, "Ganancia", GRUPO_CONTROL_CAMARA, "Ganancia", INDIGO_OK_STATE, INDIGO_RW_PERM, 1);
        if (GANANCIA_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_number_item(GANANCIA_PROPERTY_ITEM, GANANCIA_PROPERTY_NAME, "Ganancia", 0, 500, 1, 100);
        strcpy(GANANCIA_PROPERTY_ITEM->number.format, "%g");
        // -------------------------------------------------------------------------------- Inicio de todo lo relacionado con la refrigeracion
        // -------------------------------------------------------------------------------- Refrigeracion on/off
        REFRIG_STATUS_PROPERTY = indigo_init_switch_property(NULL, device->name, "Refrigeracion on/off", GRUPO_REFRIGERACION, "Refrigeracion on/off", INDIGO_OK_STATE, INDIGO_RW_PERM, INDIGO_ONE_OF_MANY_RULE, 2);
        if (REFRIG_STATUS_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_switch_item(REFRIG_STATUS_PROPERTY_ITEM_ON, CCD_COOLER_ON_ITEM_NAME, "Encendida", false);
        indigo_init_switch_item(REFRIG_STATUS_PROPERTY_ITEM_OFF, CCD_COOLER_OFF_ITEM_NAME, "Apagada", true);
        // -------------------------------------------------------------------------------- Potencia de refrigeracion
        REFRIG_POWER_PROPERTY = indigo_init_number_property(NULL, device->name, "Potencia", GRUPO_REFRIGERACION, "Potencia refrigeracion", INDIGO_IDLE_STATE, INDIGO_RO_PERM, 1);
        if (REFRIG_POWER_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_number_item(REFRIG_POWER_PROPERTY_ITEM, "Potencia", "Potencia (%)", 0, 100, 1, 0);
        // // -------------------------------------------------------------------------------- Temperatura de las camaras
        REFRIG_TEMP_PROPERTY = indigo_init_number_property(NULL, device->name, "Temperatura", GRUPO_REFRIGERACION, "Sensor de temperatura", INDIGO_OK_STATE, INDIGO_RW_PERM, 1);
        if (REFRIG_TEMP_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_number_item(REFRIG_TEMP_PROPERTY_ITEM, "Temperatura", "Temperatura (\u00B0C)", -50, 50, 1, 0);
        // -------------------------------------------------------------------------------- Inicio de todo lo relacionado con la montura
        // -------------------------------------------------------------------------------- MOUNT_PARK
        MONTURA_PARK_PROPERTY = indigo_init_switch_property(NULL, device->name, "Parking", GRUPO_MONTURA, "Parking", INDIGO_OK_STATE, INDIGO_RW_PERM, INDIGO_ONE_OF_MANY_RULE, 2);
        if (MONTURA_PARK_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_switch_item(MONTURA_PARK_PARKED_ITEM, MOUNT_PARK_PARKED_ITEM_NAME, "Montura aparcá", true);
        indigo_init_switch_item(MONTURA_PARK_UNPARKED_ITEM, MOUNT_PARK_UNPARKED_ITEM_NAME, "Montura sin aparcá", false);
        // -------------------------------------------------------------------------------- MOUNT_EQUATORIAL_COORDINATES
        MONTURA_COORDENADAS_PROPERTY = indigo_init_number_property(NULL, device->name, "Coordenadas equatoriales", GRUPO_MONTURA, "Coordenadas equatoriales", INDIGO_OK_STATE, INDIGO_RW_PERM, 2);
        if (MONTURA_COORDENADAS_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_sexagesimal_number_item(MONTURA_COORDENADAS_RA_ITEM, "RA", "Ascension recta (0 to 24 hrs)", 0, 24, 0, 0);
        indigo_init_sexagesimal_number_item(MONTURA_COORDENADAS_DEC_ITEM, "DEC", "Declinacion (-90 to 90°)", -90, 90, 0, 90);
        // -------------------------------------------------------------------------------- Inicio de todo lo relacionado con la configuracion
        // -------------------------------------------------------------------------------- Inicio de la propiedad dispositivos a conectar
        N_DISP_PROPERTY = indigo_init_number_property(NULL, device->name, "Conexion", MAIN_GROUP, "Dispositivos a conectar", INDIGO_OK_STATE, INDIGO_RW_PERM, 1);
        if (EXPOSICION_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_number_item(N_DISP_PROPERTY->items + 0, "Dispositivos", "Dispositivos", 0, 7, 1, 0);
        // -------------------------------------------------------------------------------- Inicio de la propiedad de configuracion previa
        CONF_PREV_PROPERTY = indigo_init_switch_property(NULL, device->name, "Configuracion previa", MAIN_GROUP, "Configuracion previa", INDIGO_OK_STATE, INDIGO_RW_PERM, INDIGO_ONE_OF_MANY_RULE, 2);
        if (CONF_PREV_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_switch_item(CONF_PREV_PROPERTY_ITEM_ON, "Configuracion previa", "Configuracion previa", true);
        indigo_init_switch_item(CONF_PREV_PROPERTY_ITEM_OFF, "Nueva configuracion", "Nueva configuracion", false);
        // Se llama para que aparezca en el main
        agent_enumerate_properties(device, NULL, NULL);
        // -------------------------------------------------------------------------------- Iniciacion de propiedades de nombres
        init_disp_properties(device);
        // -------------------------------------------------------------------------------- Iniciacion del mutex
        pthread_mutex_init(&PRIVATE_DATA->mutex, NULL);
    }

    return 0;
}

static indigo_result agent_enumerate_properties(indigo_device *device, indigo_client *client, indigo_property *property)
{
    if (client == agent_client)
        return INDIGO_OK;
    indigo_result result = INDIGO_OK;
    if ((result = indigo_agent_enumerate_properties(device, client, property)) == INDIGO_OK)
    {
        if (indigo_property_match(CONF_PREV_PROPERTY, property))
            indigo_define_property(device, CONF_PREV_PROPERTY, NULL);
    }
    return result;
}

static void agent_connect_callback(indigo_device *device)
{
    CONNECTION_PROPERTY->state = INDIGO_OK_STATE;
    agent_habilita_conf_properties(device, CONNECTION_CONNECTED_ITEM->sw.value);
    if (CONNECTION_CONNECTED_ITEM->sw.value)
    {
        if (PRIVATE_DATA->current_disp > 0)
        {
            escribe_nueva_config(N_DISP_PROPERTY, prop_disp);
        }
        assert(read_agent_conf());
        indigo_usleep(ONE_SECOND_DELAY);

        INDIGO_DRIVER_LOG(DRIVER_NAME, "Conectado");

        if (connect_config(&server) == INDIGO_OK)
        {
            conecta_all_cameras(&agent_client);
            conecta_montura(&agent_client);
        }
        // -------------------------------------------------------------------------------- Define de las propiedades
        indigo_define_property(device, GANANCIA_PROPERTY, NULL);
        indigo_define_property(device, EXPOSICION_PROPERTY, NULL);
        indigo_define_property(device, REFRIG_STATUS_PROPERTY, NULL);
        indigo_define_property(device, REFRIG_POWER_PROPERTY, NULL);
        indigo_define_property(device, REFRIG_TEMP_PROPERTY, NULL);
        indigo_define_property(device, MONTURA_PARK_PROPERTY, NULL);
        indigo_define_property(device, MONTURA_COORDENADAS_PROPERTY, NULL);
        // -------------------------------------------------------------------------------- Inicio y define de las propiedades de las urls
        URL_PROPERTY = indigo_init_text_property(URL_PROPERTY, device->name, "URL", GRUPO_CONTROL_CAMARA, "URL de imagen", INDIGO_OK_STATE, INDIGO_RO_PERM, n_camaras);
        if (URL_PROPERTY == NULL)
            return INDIGO_FAILED;
        for (int i = 0; i < n_camaras; i++)
        {
            char label[6] = "URL X";
            label[4] = i + '0';
            indigo_init_text_item(URL_PROPERTY->items + i, label, label, NULL);
        }
        indigo_define_property(device, URL_PROPERTY, NULL);
        // -------------------------------------------------------------------------------- Cuando le des a conectar que compruebe el numero de dispositivos en el json
    }
    else
    {
        // -------------------------------------------------------------------------------- Reset de la exposicion porque al desconectar no actualiza
        PRIVATE_DATA->current_exposicion = PRIVATE_DATA->target_exposicion = 0;
        EXPOSICION_PROPERTY_ITEM->number.value = PRIVATE_DATA->current_exposicion;
        EXPOSICION_PROPERTY->state = INDIGO_OK_STATE;

        indigo_update_property(device, EXPOSICION_PROPERTY, NULL);

        CONF_PREV_PROPERTY_ITEM_OFF->sw.value = false;
        CONF_PREV_PROPERTY_ITEM_ON->sw.value = true;
        indigo_update_property(device, CONF_PREV_PROPERTY, NULL);

        indigo_delete_property(device, EXPOSICION_PROPERTY, NULL);
        indigo_delete_property(device, GANANCIA_PROPERTY, NULL);

        indigo_delete_property(device, REFRIG_STATUS_PROPERTY, NULL);
        indigo_delete_property(device, REFRIG_POWER_PROPERTY, NULL);
        indigo_delete_property(device, REFRIG_TEMP_PROPERTY, NULL);

        indigo_delete_property(device, URL_PROPERTY, NULL);
        indigo_delete_property(device, MONTURA_PARK_PROPERTY, NULL);
        indigo_delete_property(device, MONTURA_COORDENADAS_PROPERTY, NULL);

        agent_delete_conf_properties(device);
        desconecta_all_cameras(&agent_client);
        desconecta_montura(&agent_client);
        INDIGO_DRIVER_LOG(DRIVER_NAME, "Desconectado");
    }
    indigo_update_property(device, CONNECTION_PROPERTY, NULL);
}

static indigo_result agent_change_property(indigo_device *device, indigo_client *client, indigo_property *property)
{
    assert(device != NULL);
    assert(DEVICE_CONTEXT != NULL);
    assert(property != NULL);
    if (device == agent)
    {
        if (indigo_property_match(CONNECTION_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- CONNECTION
            if (indigo_ignore_connection_change(device, property))
                return INDIGO_OK;
            indigo_property_copy_values(CONNECTION_PROPERTY, property, false);
            // Necesario para poder conectar y desconectar varias veces
            indigo_usleep(0.5 * ONE_SECOND_DELAY);
            CONNECTION_PROPERTY->state = INDIGO_BUSY_STATE;
            indigo_update_property(device, CONNECTION_PROPERTY, NULL);
            indigo_set_timer(device, 0, agent_connect_callback, NULL);
            return INDIGO_OK;
        }
        else if (indigo_property_match(EXPOSICION_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            indigo_property_copy_values(EXPOSICION_PROPERTY, property, false);
            if (EXPOSICION_PROPERTY->state == INDIGO_OK_STATE)
            {
                PRIVATE_DATA->target_exposicion = EXPOSICION_PROPERTY_ITEM->number.value;
                set_exposure_all_cameras(&agent_client, PRIVATE_DATA->target_exposicion);
            }
            PRIVATE_DATA->current_exposicion = EXPOSICION_PROPERTY_ITEM->number.value;
            EXPOSICION_PROPERTY_ITEM->number.value = PRIVATE_DATA->current_exposicion;

            EXPOSICION_PROPERTY->state = INDIGO_BUSY_STATE;
            indigo_update_property(device, EXPOSICION_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (!strcmp(CCD_EXPOSURE_PROPERTY_NAME, property->name))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (act_exp == 0)
            {
                PRIVATE_DATA->current_exposicion = property->items->number.value;
                EXPOSICION_PROPERTY_ITEM->number.value = PRIVATE_DATA->current_exposicion;

                EXPOSICION_PROPERTY->state = property->state;
                indigo_update_property(device, EXPOSICION_PROPERTY, NULL);
            }
            act_exp += 1;
            act_exp %= n_camaras;
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(GANANCIA_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            indigo_property_copy_values(GANANCIA_PROPERTY, property, false);

            set_ganancia_all_cameras(&agent_client, property->items[0].number.value);

            indigo_update_property(device, GANANCIA_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (!strcmp(CCD_IMAGE_PROPERTY_NAME, property->name))
        {
            pthread_mutex_lock(&private_data->mutex);

            URL_PROPERTY->state = property->state;
            if (property->state == INDIGO_OK_STATE)
            {
                strcpy(URL_PROPERTY->items[posicion_url].text.value, property->items[0].blob.url);

                // Aprovecho que tengo un mutex y esto no se va a incrementar mas de lo debido
                posicion_url += 1;
                if (posicion_url == n_camaras)
                {
                    almacena_todas_las_fotos();
                    crea_prop_img_proc(device);
                }
                posicion_url %= n_camaras;
            }
            indigo_update_property(device, URL_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        // -------------------------------------------------------------------------------- REFRIGERACION
        else if (indigo_property_match(REFRIG_STATUS_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- Modificar el status
            pthread_mutex_lock(&private_data->mutex);
            indigo_property_copy_values(REFRIG_STATUS_PROPERTY, property, false);

            set_cooler_status_all_cameras(&agent_client, REFRIG_STATUS_PROPERTY_ITEM_ON->sw.value);

            indigo_update_property(device, REFRIG_STATUS_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(REFRIG_TEMP_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- Modificar la temperatura
            pthread_mutex_lock(&private_data->mutex);
            indigo_property_copy_values(REFRIG_TEMP_PROPERTY, property, false);
            if (REFRIG_TEMP_PROPERTY->state == INDIGO_OK_STATE)
            {
                PRIVATE_DATA->target_temp = REFRIG_TEMP_PROPERTY_ITEM->number.value;
                set_cooler_temp_all_cameras(&agent_client, PRIVATE_DATA->target_temp);
            }
            REFRIG_TEMP_PROPERTY_ITEM->number.value = PRIVATE_DATA->current_exposicion;

            REFRIG_TEMP_PROPERTY->state = INDIGO_BUSY_STATE;
            indigo_update_property(device, REFRIG_TEMP_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (!strcmp(CCD_TEMPERATURE_PROPERTY_NAME, property->name))
        {
            // -------------------------------------------------------------------------------- Recibir actualizacion de temperatura
            pthread_mutex_lock(&private_data->mutex);
            if (act_temp == 0)
            {
                PRIVATE_DATA->current_temp = property->items->number.value;
                REFRIG_TEMP_PROPERTY_ITEM->number.value = PRIVATE_DATA->current_temp;

                REFRIG_TEMP_PROPERTY->state = property->state;
                indigo_update_property(device, REFRIG_TEMP_PROPERTY, NULL);
            }
            act_temp += 1;
            act_temp %= n_camaras;
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (!strcmp(CCD_COOLER_POWER_PROPERTY_NAME, property->name))
        {
            // -------------------------------------------------------------------------------- Recibir actualizacion de power
            pthread_mutex_lock(&private_data->mutex);

            if (act_power == 0)
            {
                REFRIG_POWER_PROPERTY_ITEM->number.value = property->items->number.value;

                REFRIG_POWER_PROPERTY->state = property->state;
                indigo_update_property(device, REFRIG_POWER_PROPERTY, NULL);
            }

            act_power += 1;
            act_power %= n_camaras;
            pthread_mutex_unlock(&private_data->mutex);
        }
        // -------------------------------------------------------------------------------- MONTURA
        else if (indigo_property_match(MONTURA_COORDENADAS_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- Cambiar las coordenadas
            pthread_mutex_lock(&private_data->mutex);

            if (MONTURA_PARK_PARKED_ITEM->sw.value)
            {
                MONTURA_COORDENADAS_PROPERTY->state = INDIGO_ALERT_STATE;
            }
            else
            {
                if (MONTURA_COORDENADAS_PROPERTY->state == INDIGO_OK_STATE)
                {
                    indigo_property_copy_values(MONTURA_COORDENADAS_PROPERTY, property, false);

                    PRIVATE_DATA->target_ra = MONTURA_COORDENADAS_RA_ITEM->number.value;
                    PRIVATE_DATA->target_dec = MONTURA_COORDENADAS_DEC_ITEM->number.value;

                    MONTURA_COORDENADAS_RA_ITEM->number.value = PRIVATE_DATA->current_ra;
                    MONTURA_COORDENADAS_DEC_ITEM->number.value = PRIVATE_DATA->current_dec;

                    if (PRIVATE_DATA->current_ra != PRIVATE_DATA->target_ra)
                    {
                        set_RA_coord_montura(&agent_client, PRIVATE_DATA->target_ra);
                    }

                    if (PRIVATE_DATA->current_dec != PRIVATE_DATA->target_dec)
                    {
                        set_DEC_coord_montura(&agent_client, PRIVATE_DATA->target_dec);
                    }
                }

                MONTURA_COORDENADAS_PROPERTY->state = INDIGO_BUSY_STATE;
            }
            indigo_update_property(device, MONTURA_COORDENADAS_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(MONTURA_PARK_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- Modificar el parking
            pthread_mutex_lock(&private_data->mutex);

            indigo_property_copy_values(MONTURA_PARK_PROPERTY, property, false);

            if (MONTURA_COORDENADAS_PROPERTY->state == INDIGO_ALERT_STATE && MONTURA_PARK_UNPARKED_ITEM->sw.value)
            {
                MONTURA_COORDENADAS_PROPERTY->state = INDIGO_OK_STATE;
                indigo_update_property(device, MONTURA_COORDENADAS_PROPERTY, NULL);
            }

            set_parking_montura(&agent_client, MONTURA_PARK_PARKED_ITEM->sw.value);

            MONTURA_PARK_PROPERTY->state = INDIGO_BUSY_STATE;

            indigo_update_property(device, MONTURA_PARK_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (!strcmp(MOUNT_PARK_PROPERTY_NAME, property->name))
        {
            // -------------------------------------------------------------------------------- Recibir actualizacion de parking
            pthread_mutex_lock(&private_data->mutex);

            MONTURA_PARK_PROPERTY->state = property->state;

            indigo_update_property(device, MONTURA_PARK_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (!strcmp(MOUNT_EQUATORIAL_COORDINATES_PROPERTY_NAME, property->name))
        {
            // -------------------------------------------------------------------------------- Recibir actualizacion de coordenadas
            pthread_mutex_lock(&private_data->mutex);

            if (MONTURA_COORDENADAS_PROPERTY->state != INDIGO_ALERT_STATE)
                MONTURA_COORDENADAS_PROPERTY->state = property->state;

            MONTURA_COORDENADAS_RA_ITEM->number.value = property->items[0].number.value;
            MONTURA_COORDENADAS_DEC_ITEM->number.value = property->items[1].number.value;

            indigo_update_property(device, MONTURA_COORDENADAS_PROPERTY, NULL);

            pthread_mutex_unlock(&private_data->mutex);
        }
        // -------------------------------------------------------------------------------- CONFIGURACION
        else if (indigo_property_match(CONF_PREV_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- Cambiar de modo
            pthread_mutex_lock(&private_data->mutex);
            if (CONF_PREV_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(CONF_PREV_PROPERTY, property, false);

                if (CONF_PREV_PROPERTY_ITEM_OFF->sw.value)
                {
                    agent_define_conf_properties(device);
                }
                else
                    agent_delete_conf_properties(device);

                indigo_update_property(device, CONF_PREV_PROPERTY, NULL);
            }

            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(N_DISP_PROPERTY, property))
        {
            // -------------------------------------------------------------------------------- Cambiar n_disp
            pthread_mutex_lock(&private_data->mutex);
            if (N_DISP_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(N_DISP_PROPERTY, property, false);

                PRIVATE_DATA->target_disp = N_DISP_PROPERTY->items->number.value;

                if (PRIVATE_DATA->target_disp > 7)
                {
                    N_DISP_PROPERTY->items->number.value = 7;
                    PRIVATE_DATA->target_disp = 7;
                }

                if (PRIVATE_DATA->current_disp < PRIVATE_DATA->target_disp)
                {
                    for (int i = PRIVATE_DATA->current_disp; i < PRIVATE_DATA->target_disp; i++)
                        muestra_x_disp(device, i);
                }
                else if (PRIVATE_DATA->current_disp > PRIVATE_DATA->target_disp)
                {
                    for (int i = PRIVATE_DATA->current_disp; i >= PRIVATE_DATA->target_disp; i--)
                        oculta_x_disp(device, i);
                }

                PRIVATE_DATA->current_disp = PRIVATE_DATA->target_disp;
                indigo_update_property(device, N_DISP_PROPERTY, NULL);
            }

            pthread_mutex_unlock(&private_data->mutex);
        }
        // -------------------------------------------------------------------------------- Cambiar valores textos
        else if (indigo_property_match(DISP_0_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_0_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_0_PROPERTY, property, false);
                indigo_update_property(device, DISP_0_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_1_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_1_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_1_PROPERTY, property, false);
                indigo_update_property(device, DISP_1_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_2_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_2_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_2_PROPERTY, property, false);
                indigo_update_property(device, DISP_2_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_3_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_3_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_3_PROPERTY, property, false);
                indigo_update_property(device, DISP_3_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_4_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_4_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_4_PROPERTY, property, false);
                indigo_update_property(device, DISP_4_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_5_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_5_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_5_PROPERTY, property, false);
                indigo_update_property(device, DISP_5_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_6_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_6_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_6_PROPERTY, property, false);
                indigo_update_property(device, DISP_6_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
        else if (indigo_property_match(DISP_7_PROPERTY, property))
        {
            pthread_mutex_lock(&private_data->mutex);
            if (DISP_7_PROPERTY->state != INDIGO_IDLE_STATE)
            {
                indigo_property_copy_values(DISP_7_PROPERTY, property, false);
                indigo_update_property(device, DISP_7_PROPERTY, NULL);
            }
            pthread_mutex_unlock(&private_data->mutex);
        }
    }

    return indigo_device_change_property(device, client, property);
}

static indigo_result agent_detach(indigo_device *device)
{
    assert(device != NULL);
    if (IS_CONNECTED)
    {
        indigo_set_switch(CONNECTION_PROPERTY, CONNECTION_DISCONNECTED_ITEM, true);
        indigo_disconnect_server(server);
        indigo_detach_client(&agent_client);
    }
    indigo_release_property(EXPOSICION_PROPERTY);
    indigo_release_property(GANANCIA_PROPERTY);
    indigo_release_property(URL_PROPERTY);
    indigo_release_property(REFRIG_STATUS_PROPERTY);
    indigo_release_property(REFRIG_POWER_PROPERTY);
    indigo_release_property(REFRIG_TEMP_PROPERTY);
    indigo_release_property(MONTURA_COORDENADAS_PROPERTY);
    indigo_release_property(MONTURA_PARK_PROPERTY);
    indigo_release_property(CONF_PREV_PROPERTY);
    indigo_release_property(N_DISP_PROPERTY);
    indigo_release_property(DISP_0_PROPERTY);
    indigo_release_property(DISP_1_PROPERTY);
    indigo_release_property(DISP_2_PROPERTY);
    indigo_release_property(DISP_3_PROPERTY);
    indigo_release_property(DISP_4_PROPERTY);
    indigo_release_property(DISP_5_PROPERTY);
    indigo_release_property(DISP_6_PROPERTY);
    indigo_release_property(DISP_7_PROPERTY);
    INDIGO_DEVICE_DETACH_LOG(DRIVER_NAME, device->name);
    pthread_mutex_destroy(&PRIVATE_DATA->mutex);
    return indigo_device_detach(device);
}

static indigo_result agent_client_update_property(indigo_client *client,
                                                  indigo_device *device, indigo_property *property, const char *message)
{

    if (!monitored_device(property->device))
    {
        return INDIGO_OK;
    }

    Dispositivo *entrada = search(property->device);

    if (entrada == NULL)
    {
        insert(property->device);
        entrada = search(property->device);
    }

    if (property_match(property->name, entrada->type))
    {
        // indigo_log("%s", property->name);
        store_update_property(property, entrada);
        agent_change_property(agent, agent_client, property);
    }

    return INDIGO_OK;
}

indigo_result agent_function(indigo_driver_action action, indigo_driver_info *info)
{
    static indigo_device agent_template = INDIGO_DEVICE_INITIALIZER(
        "MARCOT",
        agent_attach,
        agent_enumerate_properties,
        agent_change_property,
        NULL,
        agent_detach);

    static indigo_client agent_client_template = {
        "agent_Client",         // client name
        false,                  // is this a remote client "no" - this is us
        NULL,                   // we do not have client specific data
        INDIGO_OK,              // result of last bus operation
                                // - we just initialize it with ok
        INDIGO_VERSION_CURRENT, // the client speaks current indigo version
        NULL,                   // BLOB mode records -> Set this to NULL
        agent_client_attach,
        agent_client_define_property,
        agent_client_update_property,
        NULL,
        NULL,
        agent_client_detach};

    static indigo_driver_action last_action = INDIGO_DRIVER_SHUTDOWN;

    SET_DRIVER_INFO(info, "BAD BUNNY", __FUNCTION__, DRIVER_VERSION, true, last_action);

    if (action == last_action)
        return INDIGO_OK;

    switch (action)
    {
    case INDIGO_DRIVER_INIT:
        last_action = action;
        private_data = indigo_safe_malloc(sizeof(marcot_private_data));
        agent = indigo_safe_malloc_copy(sizeof(indigo_device), &agent_template);
        agent->private_data = private_data;
        indigo_attach_device(agent);
        agent_client = indigo_safe_malloc_copy(sizeof(indigo_client), &agent_client_template);
        indigo_attach_client(agent_client);
        break;

    case INDIGO_DRIVER_SHUTDOWN:
        VERIFY_NOT_CONNECTED(agent);
        last_action = action;
        if (agent_client != NULL)
        {
            indigo_detach_client(agent_client);
            free(agent_client);
            agent_client = NULL;
        }
        if (agent != NULL)
        {
            indigo_detach_device(agent);
            free(agent);
            agent = NULL;
        }
        if (private_data != NULL)
        {
            free(private_data);
            private_data = NULL;
        }
        break;

    case INDIGO_DRIVER_INFO:
        break;
    }
    return INDIGO_OK;
}