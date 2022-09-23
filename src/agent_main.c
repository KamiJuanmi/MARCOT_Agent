#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <indigo/indigo_driver_xml.h>
#include "../include/agent.h"

int main(int argc, const char * argv[]) {
	indigo_main_argc = argc;
	indigo_main_argv = argv;
    indigo_use_blob_urls = false;
	indigo_client *protocol_adapter = indigo_xml_device_adapter(0, 1);
    indigo_enable_blob_mode_record *record = (indigo_enable_blob_mode_record *)malloc(sizeof(indigo_enable_blob_mode_record));
	memset(record, 0, sizeof(indigo_enable_blob_mode_record));
	record->mode = INDIGO_ENABLE_BLOB_ALSO;
	protocol_adapter->enable_blob_mode_records = record;
	indigo_start();
	agent_function(INDIGO_DRIVER_INIT, NULL);
    indigo_attach_client(protocol_adapter);
	indigo_xml_parse(NULL, protocol_adapter);
	agent_function(INDIGO_DRIVER_SHUTDOWN, NULL);
	indigo_stop();
    

	return 0;
}