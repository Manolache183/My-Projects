//Manolache Marius-Alexandru 313CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "header.h"

#define NCHAR 300

void load(int ***imagine, char *type,
		  int *m, int *n, int *valmax, short *loaded, char *cuv)
{
	if (*loaded) { //daca este imagine loaded elibereaza memoria
		for (int i = 0; i < *n; i++)
			free((*imagine)[i]);
		free(*imagine);
	}

	char *input_filename = cuv;
	input_filename = strtok(NULL, "\n ");
	if (!input_filename) {
		printf("Invalid command\n");
		return;
	}
	FILE * in = fopen(input_filename, "rt");
	if (!in) {
		printf("Failed to load %s\n", input_filename);
		*loaded = 0;
		return;
	}
	printf("Loaded %s\n", input_filename);
	*loaded = 1;
	short b = 0; //=1 dupa ce citim P, =2 dupa ce citim valmax
	long position; //pozitia la care ne intoarcem cand trebuie stocat ceva
	char thrash[NCHAR]; //string in care se stocheaza tot ce se citeste

	while (fgets(thrash, NCHAR, in)) {
		if (thrash[0] == '#') { //sarim comentariile
			position = ftell(in);
			continue;
		} else if (b == 1) { //am trecut de magic-word
			if ((*type) == '1' || (*type) == '4')
				b = 2; //daca imaginea este alb-negru nu ne intereseaza valmax
			else
				b = 3; //urmeaza valmax
			fseek(in, position, SEEK_SET);
			fscanf(in, "%d%d", m, n);
			fgets(thrash, NCHAR, in); //pt a trece la urmatoarea linie
		} else if (thrash[0] == 'P') {
			(*type) = thrash[1];
			position = ftell(in);
			b = 1; //urmeaza sa citim dimensiunile
		} else if (b == 2) { //am ajuns la matrice si retinem pozitia
			fseek(in, position, SEEK_SET);
			break;
		} else if (b == 3) { //am ajuns la valmax
			b = 2;
			*valmax = atoi(thrash);
			position = ftell(in);
		}
	}

	if ((*type) == '3' || (*type) == '6')
		alocarematrice(imagine, 3 * (*m), *n);
	else
		alocarematrice(imagine, *m, *n);

	if ((*type) == '4' || (*type) == '5') {
		fclose(in); //redeschidem binar
		in = fopen(input_filename, "rb");
		if (!in)
			printf("Failed to load %s\n", input_filename);

		fseek(in, position, SEEK_SET);
		citiredinfisier(imagine, in, 'b', *m, *n);
	} else if ((*type) == '6') {
		fclose(in); //redeschidem binar
		in = fopen(input_filename, "rb");
		if (!in)
			printf("Failed to load %s\n", input_filename);

		fseek(in, position, SEEK_SET);
		citiredinfisier(imagine, in, 'b', 3 * (*m), *n);
	} else if ((*type) == '1' || (*type) == '2') {
		citiredinfisier(imagine, in, 't', *m, *n);
	} else if ((*type) == '3') {
		citiredinfisier(imagine, in, 't', 3 * (*m), *n);
	}
	fclose(in);
}

void salvare(int **imagine, int m, int n, int valmax, char type, char *cuv)
{
	//verificam daca al treilea cuvant exista si e ascii
	//daca nu, pur si simplu scriem in output ce trebuie scris
	FILE *out;
	char *output_filename;

	output_filename = cuv;
	output_filename = strtok(NULL, "\n ");

	cuv = output_filename;
	cuv = strtok(NULL, "\n ");

	if (cuv && strcmp(cuv, "ascii") == 0) {
		if (type == '4' || type == '5' || type == '6')
			type -= 3;

		out = fopen(output_filename, "wt");

		fputc('P', out);
		fprintf(out, "%c", type);
		fputc(10, out);
		fprintf(out, "%d", m);
		fputc(32, out);
		fprintf(out, "%d", n);
		fputc(10, out);

		if (type == '2' || type == '3') {
			fprintf(out, "%d", valmax);
			fputc(10, out);
		}

		if (type == '3')
			scriereinfisier(imagine, out, 't', 3 * m, n);
		else
			scriereinfisier(imagine, out, 't', m, n);

		printf("Saved %s\n", output_filename);
	} else {
		if (type == '1' || type == '2' || type == '3')
			type += 3;

		out = fopen(output_filename, "wb");

		fputc('P', out);
		fprintf(out, "%c", type);
		fputc(10, out);
		fprintf(out, "%d", m);
		fputc(32, out);
		fprintf(out, "%d", n);
		fputc(10, out);

		if (type == '5' || type == '6') {
			fprintf(out, "%d", valmax);
			fputc(10, out);
		}

		if (type == '6')
			scriereinfisier(imagine, out, 'b', 3 * m, n);
		else
			scriereinfisier(imagine, out, 'b', m, n);

		printf("Saved %s\n", output_filename);
	}

	fclose(out);
}
