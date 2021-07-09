//Manolache Marius-Alexandru 313CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "header.h"

#define NCHAR 300

int main(void)
{
	//imaginea, primul cuvant din comanda, tipul imaginii, tipul imaginii
	//comanda, nr de coloane, nr de linii, intensitatea pixelilor imaginii
	//coordonatele selectate, short pt a vedea daca exista o imagine incarcata
	int **imagine;
	char *command, type, fraza[NCHAR];
	int m = 2, n = 2, valmax = -1, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	short loaded = 0;

	while (1) {
		fgets(fraza, NCHAR, stdin); //fraza
		command = strtok(fraza, "\n "); //primul cuvant

		if (strcmp(command, "LOAD") == 0) {
			load(&imagine, &type, &m, &n,
				 &valmax, &loaded, command);

			x1 = 0; //update la coordonate
			y1 = 0;
			x2 = m - 1;
			y2 = n - 1;
		} else if (strcmp(command, "CROP") == 0) {
			if (loaded) {
				taie(x1, y1, x2, y2, type, &imagine, &m, &n);
				x1 = 0; //update la coordonate
				y1 = 0;
				x2 = m - 1;
				y2 = n - 1;
			} else {
				printf("No image loaded\n");
			}
		} else if (strcmp(command, "SEPIA") == 0) {
			if (loaded)
				sepia(x1, y1, x2, y2, type, &imagine, valmax);
			else
				printf("No image loaded\n");
		} else if (strcmp(command, "GRAYSCALE") == 0) {
			if (loaded)
				grayscale(x1, y1, x2, y2, type, &imagine);
			else
				printf("No image loaded\n");
		} else if (strcmp(command, "EXIT") == 0) {
			if (loaded) {
				gata(&imagine, n);
			} else {
				printf("No image loaded\n");
				exit(0);
			}
		} else if (strcmp(command, "SELECT") == 0) {
			if (loaded)
				selectare(&x1, &y1, &x2, &y2, m, n, command);
			else
				printf("No image loaded\n");
		} else if (strcmp(command, "ROTATE") == 0) {
			if (loaded == 1 && x1 == 0 && y1 == 0 &&
				x2 == m - 1 && y2 == n - 1 && x1 != x2) {
				rotatetot(&m, &n, &imagine, type, command);
				x1 = 0; //update la coordonate
				y1 = 0;
				x2 = m - 1;
				y2 = n - 1;
			} else if (loaded) {
				rotateselectie(x1, y1, x2, y2, type, &imagine, command);
			} else {
				printf("No image loaded\n");
			}
		} else if (strcmp(command, "SAVE") == 0) {
			if (loaded)
				salvare(imagine, m, n, valmax, type, command);
			else
				printf("No image loaded\n");
		} else {
			printf("Invalid command\n");
		}
	}
	return 0;
}
