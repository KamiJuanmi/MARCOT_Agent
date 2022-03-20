#include <indigo/indigo_bus.h>
#include <indigo/indigo_client.h>

#include "array.h"

// https://github.com/jehiah/json-c
// https://askubuntu.com/questions/1295203/where-to-install-json-c
#include<json-c/json.h>

#define TEXT_LEN_TO_PRINT 80

static bool save_blobs = false;
static bool print_verbose = false;

static bool reducir_print = true;

extern struct json_object *parsed_json;

void print_property_list(indigo_property *property, const char *message);

void connect_all_dev(indigo_server_entry**);

bool monitored_device(const char *device_name);

void read_json();
