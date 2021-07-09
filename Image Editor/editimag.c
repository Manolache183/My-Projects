//Manolache Marius-Alexandru 313CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "header.h"

#define NCHAR 300

void taie(int x1, int y1, int x2, int y2, char type,
		  int ***imagine, int *m, int *n)
{
	int **copie; //pt a salva elementele selectate

	int newl = y2 - y1 + 1; //noul numar de linii
	int newc = x2 - x1 + 1; //noul numar de coloane

	//alocare copie, copiere elemente in copie
	//realocare matriece imagine, copiere elemente inapoi in matrice imagine
	if (type == '3' || type == '6') {
		alocarematrice(&copie, 3 * newc, newl);

		for (int i = y1; i <= y2; i++)
			for (int j = 3 * x1; j < (3 * (x2 + 1)); j++)
				copie[i - y1][j - 3 * x1] = (*imagine)[i][j];

		for (int i = 0; i < *n; i++)
			free((*imagine)[i]);

		free(*imagine);

		alocarematrice(imagine, 3 * newc, newl);

		for (int i = 0; i < newl; i++)
			for (int j = 0; j < 3 * newc; j++)
				(*imagine)[i][j] = copie[i][j];

		for (int i = 0; i < newl; i++)
			free(copie[i]);

		free(copie);
	} else {
		alocarematrice(&copie, newc, newl);

		for (int i = y1; i <= y2; i++)
			for (int j = x1; j <= x2; j++)
				copie[i - y1][j - x1] = (*imagine)[i][j];

		for (int i = 0; i < *n; i++)
			free((*imagine)[i]);

		free(*imagine);

		alocarematrice(imagine, newc, newl);

		for (int i = 0; i < newl; i++)
			for (int j = 0; j < newc; j++)
				(*imagine)[i][j] = copie[i][j];

		for (int i = 0; i < newl; i++)
			free(copie[i]);

		free(copie);
	}

	*n = newl;
	*m = newc;

	printf("Image cropped\n");
}

void selectare(int *x1, int *y1, int *x2, int *y2, int m, int n, char *cuv)
{
	int swapperini, a1, b1, a2, b2; //pt swap si noile coordonate

	char *command = cuv;
	command = strtok(NULL, "\n ");

	//se citeste fiecare cuvant de dupa "SAVE"
	//si se verifica daca respecta formatul
	if (!command) {
		printf("Invalid command\n");
		return;
	}

	if (strcmp(command, "ALL") == 0) {
		*x1 = 0;
		*y1 = 0;
		*x2 = m - 1;
		*y2 = n - 1;

		printf("Selected ALL\n");
	} else {
		if (validparams(&a1, command) == 'N') {
			printf("Invalid command\n");
			return;
		}

		command = strtok(NULL, "\n ");

		if (validparams(&b1, command) == 'N') {
			printf("Invalid command\n");
			return;
		}

		command = strtok(NULL, "\n ");

		if (validparams(&a2, command) == 'N') {
			printf("Invalid command\n");
			return;
		}

		command = strtok(NULL, "\n ");

		if (validparams(&b2, command) == 'N') {
			printf("Invalid command\n");
			return;
		}

		if (a1 >= 0 && a1 <= m && a2 >= 0 && a2 <= m && b1 >= 0 &&
			b1 <= n && b2 >= 0 && b2 <= n && a1 != a2 && b1 != b2) {
			*x1 = a1;
			*y1 = b1;
			*x2 = a2;
			*y2 = b2;

			if (*x1 > *x2) {
				swapperini = *x1;
				*x1 = *x2;
				*x2 = swapperini;
			}

			if (*y1 > *y2) {
				swapperini = *y1;
				*y1 = *y2;
				*y2 = swapperini;
			}

			printf("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);

			//fiind interval deschis se scade 1
			(*x2)--;
			(*y2)--;
		} else {
			printf("Invalid set of coordinates\n");
		}
	}
}

void gata(int ***imagine, int n)
{
	//inchiderea programului gracefull
	for (int i = 0; i < n; i++)
		free((*imagine)[i]);

	free(*imagine);

	exit(0);
}

void sepia(int x1, int y1, int x2, int y2,
		   char type, int ***imagine, int valmax)
{
	double newred, newgreen, newblue; //noile culori

	if (type == '3' || type == '6') {
		for (int i = y1; i <= y2; i++)
			for (int j = 3 * x1; j < (3 * (x2 + 1)); j += 3) {
				newred = 0.393 * (*imagine)[i][j] +
				0.769 * (*imagine)[i][j + 1] + 0.189 * (*imagine)[i][j + 2];

				newgreen = 0.349 * (*imagine)[i][j] +
				0.686 * (*imagine)[i][j + 1] + 0.168 * (*imagine)[i][j + 2];

				newblue = 0.272 * (*imagine)[i][j] +
				0.534 * (*imagine)[i][j + 1] + 0.131 * (*imagine)[i][j + 2];

				if (round(newred) < valmax)
					(*imagine)[i][j] = (int)round(newred);
				else
					(*imagine)[i][j] = valmax;

				if (round(newgreen) < valmax)
					(*imagine)[i][j + 1] = (int)round(newgreen);
				else
					(*imagine)[i][j + 1] = valmax;

				if (round(newblue) < valmax)
					(*imagine)[i][j + 2] = (int)round(newblue);
				else
					(*imagine)[i][j + 2] = valmax;
			}

		printf("Sepia filter applied\n");
	} else {
		printf("Sepia filter not available\n");
	}
}

void grayscale(int x1, int y1, int x2, int y2, char type, int ***imagine)
{
	double gray; //noua culoare

	if (type == '3' || type == '6') {
		for (int i = y1; i <= y2; i++)
			for (int j = 3 * x1; j < (3 * (x2 + 1)); j += 3) {
				gray = ((*imagine)[i][j] + (*imagine)[i][j + 1] +
				(*imagine)[i][j + 2]) / 3;

				(*imagine)[i][j] = (int)round(gray);

				(*imagine)[i][j + 1] = (int)round(gray);

				(*imagine)[i][j + 2] = (int)round(gray);
			}

		printf("Grayscale filter applied\n");
	} else {
		printf("Grayscale filter not available\n");
	}
}

void rotateselectie(int x1, int y1, int x2, int y2,
					char type, int ***imagine, char *cuv)
{
	short times; //de cate ori este unghiul mai mare decat 90
	int **copie;

	//dimensiunile copiei
	int dim1 = y2 - y1 + 1;
	int dim2 = x2 - x1 + 1;

	if (dim1 != dim2) {
		printf("The selection must be square\n");
		return;
	}

	cuv = strtok(NULL, "\n ");
	if (!cuv) {
		printf("Invalid command\n");
		return;
	}

	//stabilirea numarului de rotatii
	if (strcmp(cuv, "0") == 0 || strcmp(cuv, "360") == 0 ||
		strcmp(cuv, "-360") == 0) {
		printf("Rotated %s\n", cuv);
		return;
	} else if (strcmp(cuv, "90") == 0 || strcmp(cuv, "-270") == 0) {
		times = 1;
	} else if (strcmp(cuv, "180") == 0 || strcmp(cuv, "-180") == 0) {
		times = 2;
	} else if (strcmp(cuv, "270") == 0 || strcmp(cuv, "-90") == 0) {
		times = 3;
	} else {
		printf("Unsupported rotation angle\n");
		return;
	}

	//rotirea propriu-zisa
	for (int i = 0; i < times; i++) {
		if (type == '3' || type == '6') {
			alocarematrice(&copie, 3 * dim2, dim1);

			for (int i = y1; i <= y2; i++)
				for (int j = 3 * x1; j < (3 * (x2 + 1)); j++)
					copie[i - y1][j - 3 * x1] = (*imagine)[i][j];

			for (int i = 0; i < dim1; i++)
				for (int j = 0; j < 3 * dim2; j += 3) {
					(*imagine)[i + y1][j + 3 * x1] =
					copie[dim1 - 1 - (j / 3)][3 * i];

					(*imagine)[i + y1][j + 3 * x1 + 1] =
					copie[dim1 - 1 - (j / 3)][3 * i + 1];

					(*imagine)[i + y1][j + 3 * x1 + 2] =
					copie[dim1 - 1 - (j / 3)][3 * i + 2];
				}

			for (int i = 0; i < dim1; i++)
				free(copie[i]);
			free(copie);
		} else {
			alocarematrice(&copie, dim2, dim1);

			for (int i = y1; i <= y2; i++)
				for (int j = x1; j <= x2; j++)
					copie[i - y1][j - x1] = (*imagine)[i][j];

			for (int i = 0; i < dim1; i++)
				for (int j = 0; j < dim2; j++)
					(*imagine)[i + y1][j + x1] = copie[dim1 - 1 - j][i];

			for (int i = 0; i < dim1; i++)
				free(copie[i]);
			free(copie);
		}
	}

	printf("Rotated %s\n", cuv);
}

void rotatetot(int *m, int *n, int ***imagine, char type, char *cuv)
{
	short times; //de cate ori este unghiul mai mare decat 90
	int **copie;
	int newc, newl; //noile dimensiuni ale imaginii

	cuv = strtok(NULL, "\n ");
	if (!cuv) {
		printf("Invalid command\n");
		return;
	}

	//stabilirea numarului de rotatii
	if (strcmp(cuv, "0") == 0 || strcmp(cuv, "360") == 0 ||
		strcmp(cuv, "-360") == 0) {
		printf("Rotated %s\n", cuv);
		return;
	} else if (strcmp(cuv, "90") == 0 || strcmp(cuv, "-270") == 0) {
		times = 1;
	} else if (strcmp(cuv, "180") == 0 || strcmp(cuv, "-180") == 0) {
		times = 2;
	} else if (strcmp(cuv, "270") == 0 || strcmp(cuv, "-90") == 0) {
		times = 3;
	} else {
		printf("Unsupported rotation angle\n");
		return;
	}

	//rotirea propriuzisa
	for (int i = 0; i < times; i++) {
		newc = *n;
		newl = *m;
		if (type == '3' || type == '6') {
			alocarematrice(&copie, 3 * (*m), *n);

			for (int i = 0; i < *n; i++)
				for (int j = 0; j < 3 * (*m); j++)
					copie[i][j] = (*imagine)[i][j];

			for (int i = 0; i < *n; i++)
				free((*imagine)[i]);
			free(*imagine);

			alocarematrice(imagine, 3 * newc, newl);

			for (int i = 0; i < newl; i++)
				for (int j = 0; j < 3 * newc; j += 3) {
					(*imagine)[i][j] = copie[(*n) - 1 - (j / 3)][3 * i];
					(*imagine)[i][j + 1] = copie[(*n) - 1 - (j / 3)][3 * i + 1];
					(*imagine)[i][j + 2] = copie[(*n) - 1 - (j / 3)][3 * i + 2];
				}

			for (int i = 0; i < (*n); i++)
				free(copie[i]);
			free(copie);
		} else {
			alocarematrice(&copie, (*m), *n);

			for (int i = 0; i < *n; i++)
				for (int j = 0; j < *m; j++)
					copie[i][j] = (*imagine)[i][j];

			for (int i = 0; i < *n; i++)
				free((*imagine)[i]);
			free(*imagine);

			alocarematrice(imagine, newc, newl);

			for (int i = 0; i < newl; i++)
				for (int j = 0; j < newc; j++)
					(*imagine)[i][j] = copie[(*n) - 1 - j][i];

			for (int i = 0; i < (*n); i++)
				free(copie[i]);

			free(copie);
		}
		*n = newl;
		*m = newc;
	}
	printf("Rotated %s\n", cuv);
}

char validparams(int *x, char *command)
{
	//functie pentru validarea parametrilor functiei select
	if (!command) //daca nu exita parametru
		return 'N';

	//verifica daca elementele stringului incepand cu al doilea sunt cifre
	short ok = 1;
	for (unsigned long i = 1; i < strlen(command); i++)
		if (!isdigit(command[i]))
			ok = 0;

	//toate elementele trebuie sa fie cifre, sau '-' in cazul primului element
	if ((isdigit(command[0]) || command[0] == '-') && ok == 1)
		*x = atoi(command);
	else
		return 'N';

	return 'Y';
}

void alocarematrice(int ***imagine, int m, int n)
{
	//alocare dinamica si verificare pentru matrice, nimic special
	(*imagine) = (int **)calloc(n, sizeof(int *));

	if (!(*imagine)) {
		printf("Nu e memorie men\n");
		exit(-1);
	}

	for (int i = 0; i < n; i++) {
		(*imagine)[i] = (int *)calloc(m, sizeof(int));
		if (!(*imagine)[i]) {
			printf("Nu e memorie men\n");
			exit(-1);
		}
	}
}

void citiredinfisier(int ***imagine, FILE *in, char p, int m, int n)
{
	//citire din fisier, nimic special
	//p poate fi "b" de la binar sau 't' de la "text"
	if (p == 't')
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				fscanf(in, "%d", &((*imagine)[i][j]));
	else if (p == 'b')
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				fread(&((*imagine)[i][j]), sizeof(char), 1, in);
}

void scriereinfisier(int **imagine, FILE *out, char p, int m, int n)
{
	//scriere in fisier, nimic special
	//p poate fi "b" de la binar sau 't' de la "text"
	if (p == 't')
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				fprintf(out, "%d ", imagine[i][j]);
			fprintf(out, "\n");
		} else if (p == 'b') {
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					fwrite(&imagine[i][j], sizeof(char), 1, out);
		}
}
