#include "../include/client.h"

static indigo_client my_client = {
    "MyClient", false, NULL, INDIGO_OK, INDIGO_VERSION_CURRENT, NULL,                   
    my_attach,
    my_define_property,
    my_update_property,
    NULL,
    NULL,
    my_detach};

int main(int argc, const char *argv[])
{
    if(read_json_client())
        return 1;

    indigo_start();

    // Para ver los mensajes de DEBUG en pantalla
    indigo_set_log_level(INDIGO_LOG_DEBUG);

    indigo_attach_client(&my_client);

    connect_all_dev(&server);
    
    conecta_all_cameras(&my_client);
    set_wait_exposure_cameras(&my_client, 1.0);
    indigo_usleep(5 * ONE_SECOND_DELAY);
    indigo_disconnect_server(server);
    indigo_detach_client(&my_client);
    indigo_usleep(0.1 * ONE_SECOND_DELAY);
    indigo_stop();
    return 0;

}