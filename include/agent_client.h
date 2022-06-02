#include "../include/funcionalidades.h"

static indigo_result agent_client_attach(indigo_client *client)
{
    indigo_enumerate_properties(client, &INDIGO_ALL_PROPERTIES);
    return INDIGO_OK;
}

static indigo_result agent_client_define_property(indigo_client *client,
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

    if(property_match(property->name, entrada->type))
    {
        store_update_property(property, entrada);
    }

    if (!strcmp(property->name, CCD_IMAGE_PROPERTY_NAME))
    {
        if (device->version >= INDIGO_VERSION_2_0)
            indigo_enable_blob(client, property, INDIGO_ENABLE_BLOB_URL);
        else
            indigo_enable_blob(client, property, INDIGO_ENABLE_BLOB_ALSO);
    }

    if (!strcmp(property->name, CCD_IMAGE_FORMAT_PROPERTY_NAME))
    {
        static const char *items[] = {CCD_IMAGE_FORMAT_JPEG_ITEM_NAME};
        static bool values[] = {true};
        indigo_change_switch_property(client, entrada->nombre, CCD_IMAGE_FORMAT_PROPERTY_NAME, 1, items, values);
    }

    return INDIGO_OK;
}

static indigo_result agent_client_detach(indigo_client *client)
{
    indigo_log("detached from INDIGO bus...");
    return INDIGO_OK;
}

static indigo_server_entry *server = NULL;