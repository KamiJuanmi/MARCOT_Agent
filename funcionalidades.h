#include "st_disp.h"

void conecta_dispositivo(indigo_client *my_client, char* nom_disp);

void conecta_dispositivo_disp(indigo_client *my_client, Dispositivo *disp);

void conecta_all_cameras(indigo_client *my_client);

void set_exposure_camera(indigo_client *my_client, char* camara);

void set_exposure_camera_disp(indigo_client *my_client, Dispositivo *disp);

void set_exposure_all_cameras(indigo_client *my_client);