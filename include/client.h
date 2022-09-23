#include "../include/funcionalidades.h"

static indigo_result my_attach(indigo_client *client)
{
    indigo_enumerate_properties(client, &INDIGO_ALL_PROPERTIES);
    return INDIGO_OK;
}

static indigo_result my_define_property(indigo_client *client,
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

static indigo_result my_update_property(indigo_client *client,
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

    if(property_match(property->name, entrada->type) && entrada->type==0)
    {
        store_update_property(property, entrada);
    }

    if (!strcmp(property->name, CCD_IMAGE_PROPERTY_NAME))
    {
        int n_disp = hashCode(entrada->nombre, SIZE);
        almacena_foto_disp(entrada, n_disp);
    }

    return INDIGO_OK;
}

static indigo_result my_detach(indigo_client *client)
{
    indigo_log("detached from INDIGO bus...");
    return INDIGO_OK;
}

static indigo_client my_client = {
    "MyClient",             // client name
    false,                  // is this a remote client "no" - this is us
    NULL,                   // we do not have client specific data
    INDIGO_OK,              // result of last bus operation
                            // - we just initialize it with ok
    INDIGO_VERSION_CURRENT, // the client speaks current indigo version
    NULL,                   // BLOB mode records -> Set this to NULL
    my_attach,
    my_define_property,
    my_update_property,
    NULL,
    NULL,
    my_detach};
