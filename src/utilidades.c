#include "../include/utilidades.h"

struct json_object *parsed_json;
Array nombre_dispositivos;
Array host_puertos;
HashTable *propiedades_tipo;
int num_devices;

void print_property_list(indigo_property *property, const char *message)
{
    indigo_item *item;
    int i;
    if (print_verbose)
    {
        char perm_str[3] = "";
        switch (property->perm)
        {
        case INDIGO_RW_PERM:
            strcpy(perm_str, "RW");
            break;
        case INDIGO_RO_PERM:
            strcpy(perm_str, "RO");
            break;
        case INDIGO_WO_PERM:
            strcpy(perm_str, "WO");
            break;
        }

        char type_str[20] = "";
        switch (property->type)
        {
        case INDIGO_TEXT_VECTOR:
            strcpy(type_str, "TEXT_VECTOR");
            break;
        case INDIGO_NUMBER_VECTOR:
            strcpy(type_str, "NUMBER_VECTOR");
            break;
        case INDIGO_SWITCH_VECTOR:
            strcpy(type_str, "SWITCH_VECTOR");
            break;
        case INDIGO_LIGHT_VECTOR:
            strcpy(type_str, "LIGHT_VECTOR");
            break;
        case INDIGO_BLOB_VECTOR:
            strcpy(type_str, "BLOB_VECTOR");
            break;
        }

        char state_str[20] = "";
        switch (property->state)
        {
        case INDIGO_IDLE_STATE:
            strcpy(state_str, "IDLE");
            break;
        case INDIGO_ALERT_STATE:
            strcpy(state_str, "ALERT");
            break;
        case INDIGO_OK_STATE:
            strcpy(state_str, "OK");
            break;
        case INDIGO_BUSY_STATE:
            strcpy(state_str, "BUSY");
            break;
        }

        printf("Name : %s.%s (%s, %s)\nState: %s\nGroup: %s\nLabel: %s\n", property->device, property->name, perm_str, type_str, state_str, property->group, property->label);
        if (message)
        {
            printf("Message:\"%s\"\n", message);
        }
        printf("Items:\n");
    }

    for (i = 0; i < property->count; i++)
    {
        if (reducir_print)
        {
            if (strcmp(property->name, CCD_EXPOSURE_PROPERTY_NAME))
            {
                continue;
            }
        }

        item = &(property->items[i]);
        switch (property->type)
        {
        case INDIGO_TEXT_VECTOR:
            if (item->text.length > TEXT_LEN_TO_PRINT)
            {
                printf("%s.%s.%s = \"%.*s\" + %ld characters\n", property->device, property->name, item->name, TEXT_LEN_TO_PRINT, item->text.value, item->text.length - TEXT_LEN_TO_PRINT - 1);
            }
            else
            {
                printf("%s.%s.%s = \"%s\"\n", property->device, property->name, item->name, item->text.value);
            }
            break;
        case INDIGO_NUMBER_VECTOR:
            printf("%s.%s.%s = %f\n", property->device, property->name, item->name, item->number.value);
            break;
        case INDIGO_SWITCH_VECTOR:
            printf("%s.%s.%s = %s\n", property->device, property->name, item->name, item->sw.value ? "ON" : "OFF");
            break;
        case INDIGO_LIGHT_VECTOR:
            printf("%s.%s.%s = %s\n", property->device, property->name, item->name, item->light.value ? "ON" : "OFF");
            break;
        case INDIGO_BLOB_VECTOR:
            if ((save_blobs) && (!indigo_use_blob_urls) && (item->blob.size > 0) && (property->state == INDIGO_OK_STATE))
            {
                // Eliminado a proposito, en diciembre
            }
            else if ((save_blobs) && (indigo_use_blob_urls) && (item->blob.url[0] != '\0') && (property->state == INDIGO_OK_STATE))
            {
                // Eliminado a proposito, en diciembre
            }
            else if ((!save_blobs) && (indigo_use_blob_urls) && (item->blob.url[0] != '\0') && (property->state == INDIGO_OK_STATE))
            {
                printf("%s.%s.%s = <%s>\n", property->device, property->name, item->name, item->blob.url);
            }
            else if ((!save_blobs) && (!indigo_use_blob_urls) && (item->blob.size > 0) && (property->state == INDIGO_OK_STATE))
            {
                printf("%s.%s.%s = <BLOB NOT SHOWN>\n", property->device, property->name, item->name);
            }
            else
            {
                // printf("save_blobs = %d, indigo_use_blob_urls = %d, item->blob.size = %d, item->blob.value = %p, property->state = %d\n", save_blobs, indigo_use_blob_urls, item->blob.size, item->blob.value, property->state);
                printf("%s.%s.%s = <NO BLOB DATA>\n", property->device, property->name, item->name);
            }
            break;
        }
    }

    if (print_verbose)
        printf("\n");
}

bool monitored_device(const char *device_name)
{
    if (estaAlmacenadoNombre(nombre_dispositivos, device_name))
    {
        return true;
    }
    return false;
}

bool property_match(const char *property_name, const int type)
{
    HashTable propiedades = propiedades_tipo[type];

    return checkeaKey(&propiedades, property_name);
}

int get_tipo_device(const char *device_name)
{
    int i = getPosicionNombre(nombre_dispositivos, device_name);
    for (i; i < nombre_dispositivos.used; i++)
    {
        if (nombre_dispositivos.array[i].uniontype == 1)
            return nombre_dispositivos.array[i].cont.num_array;
    }
    return -1;
}

int read_json_client(void)
{
    FILE *fp;
    char buffer[1024];

    fp = fopen("conf/client_conf.json", "r");
    if (fp == NULL)
    {
        printf("Error al abrir el archivo json.");
        return 1;
    }
    if (fread(buffer, 1024, 1, fp))
    {
        printf("Error al leer el archivo json.");
        return 1;
    }
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    struct json_object *number_devices;

    if (!json_object_object_get_ex(parsed_json, "number_devices", &number_devices))
    {
        printf("Error al extraer el numero de dispositivos del fichero JSON\n");
    }
    else
    {
        num_devices = json_object_get_int(number_devices);
    }

    struct json_object *devices;
    struct array_list *dispositivos;
    // https://json-c.github.io/json-c/json-c-0.10/doc/html/arraylist_8h.html
    struct json_object *disp;

    if (!json_object_object_get_ex(parsed_json, "devices", &devices))
    {
        printf("Error al extraer los dispositivos del fichero JSON\n");
        return 1;
    }

    dispositivos = json_object_get_array(devices);
    int n_distintos_dispositivos = array_list_length(dispositivos);

    propiedades_tipo = malloc(n_distintos_dispositivos * sizeof(HashTable));

    initArray(&nombre_dispositivos, 2);

    for (int i = 0; i < n_distintos_dispositivos; i++)
    {
        disp = array_list_get_idx(dispositivos, i);

        struct json_object *nombres_etiq;

        if (!json_object_object_get_ex(disp, "names", &nombres_etiq))
        {
            printf("Error al extraer los nombres de los dispositivos del fichero JSON\n");
            return 1;
        }

        // https://stackoverflow.com/questions/65914522/using-the-json-c-library-to-parse-a-json-array
        int n_nombres = json_object_array_length(nombres_etiq);
        for (int j = 0; j < n_nombres; j++)
        {
            struct json_object *nombres_reales = json_object_array_get_idx(nombres_etiq, j);
            char *nombre = json_object_get_string(nombres_reales);
            insertNombre(&nombre_dispositivos, nombre);
        }
        insertInt(&nombre_dispositivos, i);

        struct json_object *prop_etiq;

        if (!json_object_object_get_ex(disp, "properties", &prop_etiq))
        {
            printf("Error al extraer las propiedades de los dispositivos del fichero JSON\n");
            return 1;
        }

        int n_prop = json_object_array_length(prop_etiq);
        HashTable prop_disp;
        initHashTable(&prop_disp, n_prop);
        for (int j = 0; j < n_prop; j++)
        {
            struct json_object *prop_reales = json_object_array_get_idx(prop_etiq, j);
            char *nombre = json_object_get_string(prop_reales);
            hashInt(&prop_disp, nombre, 0);
        }

        propiedades_tipo[i] = prop_disp;
    }

    return 0;
}

int read_agent_conf()
{
    FILE *fp = fopen("conf/agent.conf", "r");
    bool conf = true;
    bool line_ze = true;
    int port;
    initArray(&host_puertos, 2);
    if (fp != NULL)
    {
        char line[20];
        while (fgets(line, sizeof line, fp) != NULL)
        {
            line[strcspn(line, "\n")] = 0;
            if (line_ze)
            {
                num_devices = strtoll(line, NULL, 10);

                line_ze = false;
            }
            else
            {
                if (conf)
                {
                    insertNombre(&host_puertos, line);
                }
                else
                {
                    port = strtoll(line, NULL, 10);
                    insertInt(&host_puertos, port);
                }
                conf = !conf;
            }
        }
        fclose(fp);
        printArray(host_puertos);
        return true;
    }
    else
        return false;
}

FILE *abrir_conf_n_disp(int n_disp)
{
    FILE *fp = fopen("conf/agent.conf", "w");
    fprintf(fp, "%i", n_disp);
    return fp;
}

void escribe_host_port(FILE *fp, char *host, int port)
{
    fprintf(fp, "\n%s\n", host);
    fprintf(fp, "%i", port);
}

void escribe_nueva_config(indigo_property *n_disp_prop, Array prop_disp)
{
    int n_disp = n_disp_prop->items->number.value;

    FILE *fp;
    fp = abrir_conf_n_disp(n_disp_prop->items->number.value);
    for (int i = 0; i < n_disp; i++)
    {
        char *host = malloc(128 * sizeof(char));
        char *port = malloc(128 * sizeof(char));
        int port_nb;

        strcpy(host, prop_disp.array[i].cont.propiedad->items[0].text.value);
        strcpy(port, prop_disp.array[i].cont.propiedad->items[1].text.value);
        port_nb = strtoll(port, NULL, 10);
        escribe_host_port(fp, host, port_nb);
    }
    fclose(fp);
}
