//Manolache Marius-Alexandru 313CA
#pragma once

void load(int ***imagine, char *type, int *m, int *n,
		  int *valmax, short *loaded, char *cuv);

void salvare(int **imagine, int m, int n, int valmax, char type, char *cuv);

void taie(int x1, int y1, int x2, int y2,
		  char type, int ***imagine, int *m, int *n);

void selectare(int *x1, int *y1, int *x2, int *y2, int m, int n, char *cuv);

void gata(int ***imagine, int n);

void sepia(int x1, int y1, int x2, int y2,
		   char type, int ***imagine, int valmax);

void grayscale(int x1, int y1, int x2, int y2, char type, int ***imagine);

void rotateselectie(int x1, int y1, int x2, int y2,
					char type, int ***imagine, char *cuv);

void rotatetot(int *m, int *n, int ***imagine, char type, char *cuv);

char validparams(int *x, char *command);

void alocarematrice(int ***imagine, int m, int n);

void citiredinfisier(int ***imagine, FILE *in, char p, int m, int n);

void scriereinfisier(int **imagine, FILE *out, char p, int m, int n);
