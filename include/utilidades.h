#include <indigo/indigo_bus.h>
#include <indigo/indigo_client.h>

#include "hashtable.h"

// https://github.com/jehiah/json-c
// https://askubuntu.com/questions/1295203/where-to-install-json-c
#include<json-c/json.h>

#define TEXT_LEN_TO_PRINT 80

static bool save_blobs = false;
static bool print_verbose = false;

static bool reducir_print = true;

extern struct json_object *parsed_json;

extern Array nombre_num_array; // nombre ... nombre tipo nombre ... nombre tipo

extern int num_devices;

extern HashTable *propiedades_tipo;

void print_property_list(indigo_property *property, const char *message);

indigo_result connect_all_dev(indigo_server_entry**);

bool monitored_device(const char *device_name);

bool property_match(const char *property_name, const int type);

int get_tipo_device(const char* device_name);

int read_json_client();

int read_agent_conf();

FILE *abrir_conf_n_disp(int n_disp);

void escribe_host_port(FILE *fp, char* host, int port);