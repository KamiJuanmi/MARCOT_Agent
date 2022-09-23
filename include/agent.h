
#ifndef simple_driver_h
#define simple_driver_h

#include <indigo/indigo_driver.h>
#include "agent_client.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SIMPLE_DRIVER_NAME				"El driver del Juanmi"

// No se si esto lo utilizare
#define PRIVATE_DATA ((marcot_private_data *)device->private_data)

#define GRUPO_CONTROL_CAMARA "Control de camaras"
#define GRUPO_REFRIGERACION "Refrigeracion"
#define GRUPO_MONTURA "Control de montura"

#define EXPOSICION_PROPERTY (PRIVATE_DATA->exposicion_property)
#define EXPOSICION_PROPERTY_NAME "Exposicion"
#define EXPOSICION_PROPERTY_ITEM (EXPOSICION_PROPERTY->items + 0)

#define GANANCIA_PROPERTY (PRIVATE_DATA->gain_property)
#define GANANCIA_PROPERTY_NAME "Ganancia"
#define GANANCIA_PROPERTY_ITEM (GANANCIA_PROPERTY->items + 0)

#define URL_PROPERTY (PRIVATE_DATA->url_property)

#define IMG_PROC_PROPERTY (PRIVATE_DATA->img_proc)
#define IMG_PROC_PROPERTY_ITEM (IMG_PROC_PROPERTY->items + 0)


#define REFRIG_POWER_PROPERTY (PRIVATE_DATA->refrig_power)
#define REFRIG_POWER_PROPERTY_ITEM (REFRIG_POWER_PROPERTY->items + 0)

#define REFRIG_STATUS_PROPERTY (PRIVATE_DATA->refrig_status)
#define REFRIG_STATUS_PROPERTY_ITEM_ON (REFRIG_STATUS_PROPERTY->items + 0)
#define REFRIG_STATUS_PROPERTY_ITEM_OFF (REFRIG_STATUS_PROPERTY->items + 1)

#define REFRIG_TEMP_PROPERTY (PRIVATE_DATA->refrig_temperatura)
#define REFRIG_TEMP_PROPERTY_ITEM (REFRIG_TEMP_PROPERTY->items + 0)


/** MOUNT_EQUATORIAL_COORDINATES property pointer, property is mandatory, property change request should be fully handled by device driver.
 */
#define MONTURA_COORDENADAS_PROPERTY (PRIVATE_DATA->montura_equatorial_coord_property)
#define MONTURA_COORDENADAS_RA_ITEM (MONTURA_COORDENADAS_PROPERTY->items+0)
#define MONTURA_COORDENADAS_DEC_ITEM (MONTURA_COORDENADAS_PROPERTY->items+1)

/** MOUNT_PARK property pointer, property is optional, property change request should be fully handled by device driver.
*/
#define MONTURA_PARK_PROPERTY (PRIVATE_DATA->montura_park_property)
#define MONTURA_PARK_PARKED_ITEM (MONTURA_PARK_PROPERTY->items+0)
#define MONTURA_PARK_UNPARKED_ITEM (MONTURA_PARK_PROPERTY->items+1)

#define CONF_PREV_PROPERTY (PRIVATE_DATA->conf_prev_property)
#define CONF_PREV_PROPERTY_ITEM_ON (CONF_PREV_PROPERTY->items + 0)
#define CONF_PREV_PROPERTY_ITEM_OFF (CONF_PREV_PROPERTY->items + 1)
#define N_DISP_PROPERTY (PRIVATE_DATA->n_dispositivos_property)

#define DISP_0_PROPERTY (PRIVATE_DATA->disp_0)
#define DISP_1_PROPERTY (PRIVATE_DATA->disp_1)
#define DISP_2_PROPERTY (PRIVATE_DATA->disp_2)
#define DISP_3_PROPERTY (PRIVATE_DATA->disp_3)
#define DISP_4_PROPERTY (PRIVATE_DATA->disp_4)
#define DISP_5_PROPERTY (PRIVATE_DATA->disp_5)
#define DISP_6_PROPERTY (PRIVATE_DATA->disp_6)
#define DISP_7_PROPERTY (PRIVATE_DATA->disp_7)

typedef struct
{
    double target_exposicion, current_exposicion;
    indigo_property *exposicion_property;
    indigo_property *gain_property;
    indigo_property *url_property;
    indigo_property *img_proc;
    void *image_buffer;

    double current_disp, target_disp;
    indigo_property *conf_prev_property;
    indigo_property *n_dispositivos_property;
    indigo_property *disp_0;
    indigo_property *disp_1;
    indigo_property *disp_2;
    indigo_property *disp_3;
    indigo_property *disp_4;
    indigo_property *disp_5;
    indigo_property *disp_6;
    indigo_property *disp_7;

    indigo_property *refrig_power;
    indigo_property *refrig_status;
    double target_temp, current_temp;
    indigo_property *refrig_temperatura;

    double target_ra, current_ra, target_dec, current_dec;
	indigo_property *montura_equatorial_coord_property;	///< MOUNT_EQUATORIAL_COORDINATES property pointer
	indigo_property *montura_park_property; ///< MOUNT_PARK property pointer

    pthread_mutex_t mutex;
} marcot_private_data;

extern indigo_result indigo_simple_driver(indigo_driver_action action, indigo_driver_info *info);


#ifdef __cplusplus
}
#endif

#endif /* indigo_simple_driver_h */