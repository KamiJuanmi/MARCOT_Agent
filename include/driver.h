
/** Prueba de desarollo de un driver
 \file indigo_simple_driver.h
 */

#ifndef simple_driver_h
#define simple_driver_h

#include <indigo/indigo_driver.h>
#include "client.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SIMPLE_DRIVER_NAME				"El driver del Juanmi"

/** Create driver device instance
 */

extern indigo_result indigo_simple_driver(indigo_driver_action action, indigo_driver_info *info);

#ifdef __cplusplus
}
#endif

#endif /* indigo_simple_driver_h */