#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <indigo/indigo_bus.h>
#include <indigo/indigo_client.h>
#include <indigo/indigo_xml.h>

#define TEXT_LEN_TO_PRINT 80


static bool save_blobs = false;
static bool print_verbose = false;

static int count = 5;

static bool reducir_print = true;

void print_property_list(indigo_property *property, const char *message);

void conecta_n_disp(int, indigo_server_entry**);