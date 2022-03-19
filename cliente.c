#include "utilidades.h"
#include "st_disp.h"

#define CCD_SIMULATOR "CCD Imager Simulator @ indigo_4"

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

    if (!monitored_device(property->device))
    {
        return INDIGO_OK;
    }

    struct Dispositivo *entrada = search(property->device);

    if (entrada == NULL)
    {
        insert(property->device);
        entrada = search(property->device);
    }

    print_property_list(property, message);

    if (!strcmp(property->name, CONNECTION_PROPERTY_NAME))
    {
        entrada->propiedadConnect = property;
    }

    if (!strcmp(property->name, CCD_EXPOSURE_PROPERTY_NAME))
    {
        entrada->propiedadExposicion = property;
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

    struct Dispositivo *entrada = search(property->device);

    if (entrada == NULL)
    {
        insert(property->device);
        entrada = search(property->device);
    }

    print_property_list(property, message);

    if (!strcmp(property->name, CONNECTION_PROPERTY_NAME))
    {
        entrada->propiedadConnect = property;
    }

    if (!strcmp(property->name, CCD_EXPOSURE_PROPERTY_NAME))
    {
        entrada->propiedadExposicion = property;
    }

    if (!strcmp(property->name, CCD_IMAGE_PROPERTY_NAME))
    {
        entrada->imagen = property;
        almacena_foto(entrada);
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
    read_json();

    indigo_start();

    // Para ver los mensajes de DEBUG en pantalla
    indigo_set_log_level(INDIGO_LOG_DEBUG);

    indigo_attach_client(&my_client);

    indigo_server_entry *server;
    connect_all_dev(&server);

    printf("Esperando a propiedad connect\n");

    struct Dispositivo *entrada = search(CCD_SIMULATOR);

    while(entrada == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
        entrada = search(CCD_SIMULATOR);
    }

    while (entrada->propiedadConnect == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
    }

    printf("Ya he conseguido la propiedad connect\n");
    indigo_usleep(ONE_SECOND_DELAY);

    if (indigo_get_switch(entrada->propiedadConnect, CONNECTION_CONNECTED_ITEM_NAME))
    {
        printf("Ya estaba conectado\n");
    }
    else
    {
        printf("No estaba conectado\n");

        indigo_device_connect(&my_client, entrada->nombre);

        printf("Me conecto yo\n");
    }

    printf("Esperando a propiedad tiempo de exposicion\n");
    while (entrada->propiedadExposicion == NULL)
    {
        indigo_usleep(ONE_SECOND_DELAY);
        entrada = search(CCD_SIMULATOR);
    }

    printf("Ya la he conseguido, voy a hacer una foto\n");

    static const char *items[] = {CCD_EXPOSURE_ITEM_NAME};
    static double values[] = {2.0};
    indigo_change_number_property(&my_client, entrada->nombre, CCD_EXPOSURE_PROPERTY_NAME, 1, items, values);

    indigo_usleep(5 * ONE_SECOND_DELAY);
    display();
    indigo_disconnect_server(server);
    indigo_detach_client(&my_client);
    indigo_stop();

    return 0;

}