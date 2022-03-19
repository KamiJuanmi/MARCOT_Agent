#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <indigo/indigo_bus.h>
#include <indigo/indigo_client.h>
#include <indigo/indigo_xml.h>

// https://github.com/jehiah/json-c
// https://askubuntu.com/questions/1295203/where-to-install-json-c
#include<json-c/json.h>

#define TEXT_LEN_TO_PRINT 80

static bool save_blobs = false;
static bool print_verbose = false;

static bool reducir_print = true;

extern int num_devices;

void print_property_list(indigo_property *property, const char *message);

void connect_all_dev(indigo_server_entry**);

bool monitored_device(const char *device_name);

void read_json();
