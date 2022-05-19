#include "st_disp.h"

void conecta_dispositivo(indigo_client *my_client, char* nom_disp);

void conecta_dispositivo_disp(indigo_client *my_client, Dispositivo *disp);

void conecta_all_cameras(indigo_client *my_client);

void desconecta_all_cameras(indigo_client *my_client);

void set_exposure_camera(indigo_client *my_client, char* camara, const double sec);

void set_exposure_camera_disp(indigo_client *my_client, Dispositivo *disp, const double sec);

void set_exposure_all_cameras(indigo_client *my_client,const double sec);

void set_wait_exposure_cameras(indigo_client *my_client, const double sec);