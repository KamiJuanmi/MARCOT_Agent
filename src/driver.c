/** Prueba de desarollo de un driver
 \file indigo_simple_driver.c
 */

#define DRIVER_VERSION 0x0000
#define DRIVER_NAME "Junamis"

#define CCD_PRUEBA "CCD Imager Simulator @ indigo_0"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>

#include <indigo/indigo_driver_xml.h>

#include "../include/driver.h"

// No se si esto lo utilizare
#define PRIVATE_DATA ((marcot_private_data *)device->private_data)

#define EXPOSICION_PROPERTY (PRIVATE_DATA->exposicion_property)
#define EXPOSICION_PROPERTY_ITEM (EXPOSICION_PROPERTY->items + 0)

typedef struct
{
    double target_exposicion, current_exposicion;
    indigo_property *exposicion_property;
    Array propiedades_polla;
} marcot_private_data;

static indigo_result hucha_enumerate_properties(indigo_device *device, indigo_client *client, indigo_property *property)
{
    if (IS_CONNECTED)
    {
        if (indigo_property_match(EXPOSICION_PROPERTY, property))
            indigo_define_property(device, EXPOSICION_PROPERTY, NULL);
    }
    return indigo_device_enumerate_properties(device, NULL, NULL);
}

static indigo_result hucha_attach(indigo_device *device)
{
    assert(device != NULL);
    assert(PRIVATE_DATA != NULL);
    if (indigo_device_attach(device, DRIVER_NAME, DRIVER_VERSION, 0) == INDIGO_OK)
    {

        indigo_attach_client(&my_client);
        indigo_set_log_level(INDIGO_LOG_DEBUG);

        // -------------------------------------------------------------------------------- Inicio la propiedad inventada
        EXPOSICION_PROPERTY = indigo_init_number_property(EXPOSICION_PROPERTY, device->name, "Exposicion", "Mi grupo", "Hola", INDIGO_OK_STATE, INDIGO_RW_PERM, 1);
        if (EXPOSICION_PROPERTY == NULL)
            return INDIGO_FAILED;
        indigo_init_number_item(EXPOSICION_PROPERTY_ITEM, "Exposicion", "Exposicion", 0, 10000, 1, 0);
        strcpy(EXPOSICION_PROPERTY_ITEM->number.format, "%g");
        // --------------------------------------------------------------------------------

        INDIGO_DEVICE_ATTACH_LOG(CCD_PRUEBA, device->name);
    }
    return 0;
}

static void hucha_connect_callback(indigo_device *device)
{
    CONNECTION_PROPERTY->state = INDIGO_OK_STATE;
    INDIGO_DRIVER_LOG(DRIVER_NAME, "he tocado");
    if (CONNECTION_CONNECTED_ITEM->sw.value)
    {
        INDIGO_DRIVER_LOG(DRIVER_NAME, "Conectado");
        if (!read_json_client())
        {
            if (connect_all_dev(&server) == INDIGO_OK)
            {
                conecta_all_cameras(&my_client);
            }
        }
        indigo_define_property(device, EXPOSICION_PROPERTY, NULL);
    }
    else
    {
        indigo_delete_property(device, EXPOSICION_PROPERTY, NULL);
        desconecta_all_cameras(&my_client);
        INDIGO_DRIVER_LOG(DRIVER_NAME, "Desconectado");
    }
    indigo_update_property(device, CONNECTION_PROPERTY, NULL);
}

static indigo_result hucha_change_property(indigo_device *device, indigo_client *client, indigo_property *property)
{
    assert(device != NULL);
    assert(DEVICE_CONTEXT != NULL);
    assert(property != NULL);
    if (indigo_property_match(CONNECTION_PROPERTY, property))
    {
        // -------------------------------------------------------------------------------- CONNECTION
        if (indigo_ignore_connection_change(device, property))
            return INDIGO_OK;
        indigo_property_copy_values(CONNECTION_PROPERTY, property, false);
        CONNECTION_PROPERTY->state = INDIGO_BUSY_STATE;
        indigo_update_property(device, CONNECTION_PROPERTY, NULL);
        indigo_set_timer(device, 0, hucha_connect_callback, NULL);
        return INDIGO_OK;
    }
    else if (indigo_property_match(EXPOSICION_PROPERTY, property))
    {
        indigo_property_copy_values(EXPOSICION_PROPERTY, property, false);
        EXPOSICION_PROPERTY->state = INDIGO_BUSY_STATE;
        PRIVATE_DATA->target_exposicion = EXPOSICION_PROPERTY_ITEM->number.value;
        EXPOSICION_PROPERTY_ITEM->number.value = PRIVATE_DATA->current_exposicion;
        indigo_update_property(device, EXPOSICION_PROPERTY, "Target exposicion %g", PRIVATE_DATA->target_exposicion);
        set_exposure_all_cameras(&my_client, PRIVATE_DATA->target_exposicion);
    }
    return indigo_device_change_property(device, client, property);
}

static indigo_result hucha_detach(indigo_device *device)
{
    assert(device != NULL);
    if (IS_CONNECTED)
    {
        indigo_set_switch(CONNECTION_PROPERTY, CONNECTION_DISCONNECTED_ITEM, true);
        hucha_connect_callback(device);
        indigo_disconnect_server(server);
        indigo_detach_client(&my_client);
    }
    indigo_release_property(EXPOSICION_PROPERTY);
    INDIGO_DEVICE_DETACH_LOG(DRIVER_NAME, device->name);
    return indigo_device_detach(device);
}

static marcot_private_data *private_data = NULL;
static indigo_device *hucha = NULL;

indigo_result indigo_simple_driver(indigo_driver_action action, indigo_driver_info *info)
{
    static indigo_device hucha_template = INDIGO_DEVICE_INITIALIZER(
        "MARCOT",
        hucha_attach,
        hucha_enumerate_properties,
        hucha_change_property,
        NULL,
        hucha_detach);

    static indigo_driver_action last_action = INDIGO_DRIVER_SHUTDOWN;

    SET_DRIVER_INFO(info, "BAD BUNNY", __FUNCTION__, DRIVER_VERSION, true, last_action);

    if (action == last_action)
        return INDIGO_OK;

    switch (action)
    {
    case INDIGO_DRIVER_INIT:
        last_action = action;
        private_data = indigo_safe_malloc(sizeof(marcot_private_data));
        hucha = indigo_safe_malloc_copy(sizeof(indigo_device), &hucha_template);
        hucha->private_data = private_data;
        indigo_attach_device(hucha);
        break;

    case INDIGO_DRIVER_SHUTDOWN:
        VERIFY_NOT_CONNECTED(hucha);
        last_action = action;
        if (hucha != NULL)
        {
            indigo_detach_device(hucha);
            free(hucha);
            hucha = NULL;
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
