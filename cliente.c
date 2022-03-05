#include "utilidades.h"

#define CCD_SIMULATOR "CCD Imager Simulator @ indigo_1"
#define CCD_SIMULATOR2 "CCD Imager Simulator @ indigo_0"

indigo_property *propiedadConnect = NULL;
indigo_property *propiedadExposicion = NULL;

static indigo_result my_attach(indigo_client *client)
{
    indigo_log("attached to INDIGO bus...");

    // Request property definitions
    indigo_enumerate_properties(client, &INDIGO_ALL_PROPERTIES);

    return INDIGO_OK;
}

static indigo_result my_define_property(indigo_client *client,
                                        indigo_device *device, indigo_property *property, const char *message)
{

    if (strcmp(property->device, CCD_SIMULATOR) && strcmp(property->device, CCD_SIMULATOR2))
    {
        return INDIGO_OK;
    }

    print_property_list(property, message);

    if (!strcmp(property->name, CONNECTION_PROPERTY_NAME))
    {
        propiedadConnect = property;
    }

    if (!strcmp(property->name, CCD_EXPOSURE_PROPERTY_NAME))
    {
        propiedadExposicion = property;
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
        indigo_change_switch_property(client, CCD_SIMULATOR, CCD_IMAGE_FORMAT_PROPERTY_NAME, 1, items, values);
    }

    return INDIGO_OK;
}

static indigo_result my_update_property(indigo_client *client,
                                        indigo_device *device, indigo_property *property, const char *message)
{

    if (strcmp(property->device, CCD_SIMULATOR) && strcmp(property->device, CCD_SIMULATOR2))
    {
        return INDIGO_OK;
    }

    print_property_list(property, message);

    if (!strcmp(property->name, CONNECTION_PROPERTY_NAME))
    {
        propiedadConnect = property;
    }

    if (!strcmp(property->name, CCD_EXPOSURE_PROPERTY_NAME))
    {
        propiedadExposicion = property;
    }

    if (!strcmp(property->name, CCD_IMAGE_PROPERTY_NAME))
    {
        /* URL blob transfer is available only in client - server setup.
           This will never be called in case of a client loading a driver. */
        if (*property->items[0].blob.url && indigo_populate_http_blob_item(&property->items[0]))
            indigo_log("image URL received (%s, %d bytes)...", property->items[0].blob.url, property->items[0].blob.size);

        if (property->items[0].blob.value)
        {
            char name[32];
            sprintf(name, "img_%02d.jpeg", count);
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

int main(int argc, const char *argv[])
{
    int n_server;
    if (argc < 2)
    {
        printf("Por favor vuelva a ejecutar \"./cliente <n_server>\"\n");
        return 0;
    }
    else
    {
        const char *aux = argv[1];
        if (aux[1] != '\0')
        {
            printf("Por favor vuelva a ejecutar \"./cliente <n_server>\", teniedo en cuenta que 0<n_server<10\n");
            return 0;
        }
        n_server = (int)aux[0] - 48;
    }

    indigo_start();

    // Para ver los mensajes de DEBUG en pantalla
    indigo_set_log_level(INDIGO_LOG_DEBUG);

    indigo_attach_client(&my_client);

    indigo_server_entry *server;
    conecta_n_disp(n_server, &server);

    printf("Esperando a propiedad connect\n");

    while (propiedadConnect == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
    }

    printf("Ya he conseguido la propiedad connect\n");
    indigo_usleep(ONE_SECOND_DELAY);

    if (indigo_get_switch(propiedadConnect, CONNECTION_CONNECTED_ITEM_NAME))
    {
        printf("Ya estaba conectado\n");
    }
    else
    {
        printf("No estaba conectado\n");

        indigo_device_connect(&my_client, CCD_SIMULATOR);

        printf("Me conecto yo\n");
    }

    printf("Esperando a propiedad tiempo de exposicion\n");
    while (propiedadExposicion == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
    }

    printf("Ya la he conseguido, voy a hacer una foto\n");

    static const char *items[] = {CCD_EXPOSURE_ITEM_NAME};
    static double values[] = {6.0};
    indigo_change_number_property(&my_client, CCD_SIMULATOR2, CCD_EXPOSURE_PROPERTY_NAME, 1, items, values);

    indigo_usleep(10 * ONE_SECOND_DELAY);

    indigo_disconnect_server(server);
    indigo_detach_client(&my_client);
    indigo_stop();

    return 0;
}