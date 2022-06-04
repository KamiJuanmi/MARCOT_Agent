#include "st_disp.h"

extern int n_camaras;

indigo_result connect_all_dev(indigo_server_entry**);

indigo_result connect_config(indigo_server_entry**);

void conecta_dispositivo(indigo_client *my_client, char* nom_disp);

// Juanmi del pasado te amo por hacer esta funcion
void conecta_dispositivo_disp(indigo_client *my_client, Dispositivo *disp);

void conecta_all_cameras(indigo_client *my_client);

void desconecta_all_cameras(indigo_client *my_client);

void conecta_montura(indigo_client *my_client);

void desconecta_montura(indigo_client *my_client);

void set_exposure_camera(indigo_client *my_client, char* camara, const double sec);

void set_exposure_camera_disp(indigo_client *my_client, Dispositivo *disp, const double sec);

void set_exposure_all_cameras(indigo_client *my_client,const double sec);

void set_wait_exposure_cameras(indigo_client *my_client, const double sec);

void set_ganancia_camera_disp(indigo_client *my_client, Dispositivo *disp, const double ganancia);

void set_ganancia_all_cameras(indigo_client *my_client, const double ganancia);

void set_cooler_status_camera_disp(indigo_client *my_client, Dispositivo *disp, const bool value);

void set_cooler_status_all_cameras(indigo_client *my_client, const bool value);

void set_cooler_temp_camera_disp(indigo_client *my_client, Dispositivo *disp, const double temp);

void set_cooler_temp_all_cameras(indigo_client *my_client, const double temp);

void set_RA_coord_montura(indigo_client *my_client, const double coord);

void set_DEC_coord_montura(indigo_client *my_client, const double coord);

void set_parking_montura(indigo_client *my_client, const bool value);

void almacena_foto_disp(Dispositivo *disp, int n_disp);

void almacena_todas_las_fotos();

void procesa_fotos();
// magick montage -label %f -frame 5 -background '#336699' -geometry +4+4 rose.jpg red-ball.png frame.jpg

// montage -label %f -frame 7 -background '#451080' -geometry +2+2 -resize 10% img/img_disp_0.jpeg img/img_disp_1.jpeg img/img_disp_0.jpeg frame.jpg