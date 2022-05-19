#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <indigo/indigo_driver_xml.h>
#include "../include/driver.h"

int main(int argc, const char * argv[]) {
	indigo_main_argc = argc;
	indigo_main_argv = argv;
	indigo_client *protocol_adapter = indigo_xml_device_adapter(0, 1);
	indigo_start();
	indigo_simple_driver(INDIGO_DRIVER_INIT, NULL);
	indigo_attach_client(protocol_adapter);
	indigo_xml_parse(NULL, protocol_adapter);
	indigo_simple_driver(INDIGO_DRIVER_SHUTDOWN, NULL);
	indigo_stop();
	return 0;
}