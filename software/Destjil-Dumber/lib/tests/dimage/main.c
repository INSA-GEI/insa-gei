/* 
 * File:   main.c
 * Author: P.-E. Hladik
 * Created on 31 mars 2013
 *
 * Ce programme teste les méthodes de la bibliothèque dimage
 */

#include <stdio.h>
#include <stdlib.h>
#include "../headers/dimage.h"
#include "../headers/dimageshop.h"


int main(int argc, char** argv) {
	if (argc <= 1) {
		printf("Entrer le nom du fichier de la photo\n");
	} else {
		DImage* p0 = d_new_image();
		IplImage* imgSrc = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
		d_image_set_ipl(p0, imgSrc);
		cvReleaseImage(&imgSrc);
		p0->print(p0);
		showImage(p0, "Img");
		
		DArena* p1 = d_image_compute_arena_position(p0);
		if (p1 != NULL) {
			p1->print(p1);
			d_imageshop_draw_arena(p0, p1);
			showImage(p0, "arene");
		} else {
			printf("Pas d'arene\n");
		}
		
		DPosition* p2 = d_image_compute_robot_position(p0, p1);
		if (p2 != NULL) {
			p2->print(p2);
			d_imageshop_draw_position(p0, p2);
			showImage(p0, "Position");
		} else {
			printf("Pas de position trouvee\n");
		}
	}
    return 0;
}

