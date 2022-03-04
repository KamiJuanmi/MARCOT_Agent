#include "utilidades.h"

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

            /*
            Puesto que a esta funcion solo se le llama cuando es el CCD en el update, esto no hace falta, pero hay que tener cuidado si se cambia el orden
            if (strcmp(property->device, CCD_SIMULATOR))
            {
                continue;
            }
            */
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