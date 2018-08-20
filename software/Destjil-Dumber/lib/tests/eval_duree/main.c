/* 
 * File:    main.cpp
 * Author:  Piro
 *
 * Created on 6 décembre 2011, 07: 47
 */


#include <stdlib.h> 
#include "../headers/dincludes.h"

#define MAX_NBE 1000


double moyenne(long long mesures[MAX_NBE], int nbe);
double ecart_type(long long mesures[MAX_NBE], double moyenne, int nbe);
double min(long long mesures[MAX_NBE], int nbe);
double max(long long mesures[MAX_NBE], int nbe);

double moyenne(long long mesures[MAX_NBE], int nbe) {
    int i;
    long long somme = 0;
    for (i = 0; i < nbe; i++) {
        somme += mesures[i];
    }
    return (double) somme / (double) nbe;
}

double ecart_type(long long mesures[MAX_NBE], double moyenne, int nbe) {
    long long somme = 0;
    int i;
    for (i=0; i<nbe; i++){
        somme += pow((double)mesures[i] - moyenne,2.0);
    }
    return (double) somme / (double) nbe;
}

double min(long long mesures[MAX_NBE], int nbe) {
	long long m = 1000000000000000000.0;
    int i;
	for (i=0; i< nbe; i++){
        if (mesures[i] < m) m = mesures[i]; 
    }
    return (double) m;
}

double max(long long mesures[MAX_NBE], int nbe) {
	long long m = 0;
    int i;
	for (i=0; i<nbe; i++){
        if (mesures[i] > m) m = mesures[i]; 
    }
    return (double) m;
}

int main(int argc, char** argv) {
	
    long long t1;
    long long t2;
    long long mesures[MAX_NBE];
    double moy, e, mi, ma;
	
	int real = 0;
	
	DCamera* camera;
    DImage* img = d_new_image();
    int i = 0;
	int nbe = MAX_NBE;
	
    DArena* arena = NULL;
	
	while ( i < argc) {
		if (!strcmp(argv[i], "-n")) {
			if (i+1 >= argc) {
				printf("Valeur manquante après -n\n");
				exit(-1);
			}
			nbe = atoi(argv[i+1]);
			printf("Nombre d'échantillons: %d\n", nbe);
		}
		i++;
	}
	
	if (!real) { //Utilisation d'une image fixe
		printf("Chargement image\n");
		IplImage* imgSrc = cvLoadImage("Photo1c.jpg", CV_LOAD_IMAGE_COLOR);
		d_image_set_ipl(img, imgSrc);
		cvReleaseImage(&imgSrc);
	} else {
		printf("ouverture camera\n");
		camera = d_new_camera();
		d_camera_open(camera);
	}
	
	
	/*****************************
	 Temps d'acquistion d'une image
	 d_camera_get_frame
 	*****************************/
	if (real) {
		printf("Evalution du temps pour d_camera_get_frame\n");
		for (i = 0; i < nbe; i++) {
			t1 = d_tools_get_time_in_millisec();
			d_camera_get_frame(camera, img);
			t2 = d_tools_get_time_in_millisec();
			mesures[i] = t2 - t1;
		}
		moy = moyenne(mesures, nbe);
		e = ecart_type(mesures, moy, nbe);
		mi = min(mesures, nbe);
		ma = max(mesures, nbe);
		printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
	}    
	
	/*****************************
	 Temps pour le calcul de la position du robot avec arene
	 d_image_compute_arena_position
	 *****************************/
	printf("Evalution du temps pour d_image_compute_arena_position\n");
    for (i = 0; i < nbe; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_image_compute_arena_position(img);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
	moy = moyenne(mesures, nbe);
	e = ecart_type(mesures, moy, nbe);
	mi = min(mesures, nbe);
	ma = max(mesures, nbe);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
	
	
	/*****************************
	 Temps pour le calcul de la position du robot sans arene
	 d_image_compute_robot_position
	 *****************************/
	printf("Evalution du temps pour d_image_compute_robot_position sans arene\n");
	arena = NULL;
    for (i = 0; i < nbe; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_image_compute_robot_position(img, arena);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
	moy = moyenne(mesures, nbe);
	e = ecart_type(mesures, moy, nbe);
	mi = min(mesures, nbe);
	ma = max(mesures, nbe);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);

	/*****************************
	 Temps pour le calcul de la position du robot avec arene
	 d_image_compute_robot_position
	 *****************************/
	printf("Evalution du temps pour d_image_compute_robot_position avec arene\n");
	if (real) d_camera_get_frame(camera, img);
	i = 0;
	while (arena == NULL) {
		arena = d_image_compute_arena_position(img);
		i++;
		if (i > 10) exit(EXIT_FAILURE);
	}
	
    for (i = 0; i < nbe; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_image_compute_robot_position(img, arena);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
	moy = moyenne(mesures, nbe);
	e = ecart_type(mesures, moy, nbe);
	mi = min(mesures, nbe);
	ma = max(mesures, nbe);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
    
	
	/*****************************
	 Temps pour compresser une image
	 d_jpegimage_compress
	 *****************************/
	printf("Evalution du temps pour d_jpegimage_compress\n");
    DJpegimage* jpeg = d_new_jpegimage();
    for (i = 0; i < nbe; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_jpegimage_compress(jpeg, img);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
	moy = moyenne(mesures, nbe);
	e = ecart_type(mesures, moy, nbe);
	mi = min(mesures, nbe);
	ma = max(mesures, nbe);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
	    
    return 0;
}

