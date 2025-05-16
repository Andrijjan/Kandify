#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DATATYPE_H.h"

void clearConsole() {

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void createFile() {
	FILE* pF = fopen("pjesma.bin", "wb");
	if (pF == NULL) {
		printf("\nGreška pri kreiranju datoteke pjesma.bin!\n");
		exit(EXIT_FAILURE);
	}

	int noPJESMA = 0;
	fwrite(&noPJESMA, sizeof(int), 1, pF);

	fclose(pF);
	printf("\nDatoteka je kreirana!\n");
}

void readPJESMA(int* noPJESMA, FILM** arrayPJESMA) {
	FILE* pF = fopen("pjesma.bin", "rb");
	if (pF == NULL) {
		createFile();
		pF = fopen("pjesma.bin", "rb");
	}
	fread(noPJESMA, sizeof(int), 1, pF);
	*arrayPJESMA = (FILM*)calloc(*noPJESMA, sizeof(FILM));
	if (*arrayPJESMA == NULL) {
		printf("\nNedovoljno memorije za ucitavanje pjesama!\n");
		fclose(pF);
		return;
	}
	fseek(pF, sizeof(int), SEEK_SET);
	fread(*arrayPJESMA, sizeof(PJESMA), *noPJESMA, pF);
	fclose(pF);
}

void addPJESMA() {

	clearConsole();

	FILM tempPJESMA = { 0 };
	int noPJESMA = 0;

	FILE* pF = fopen("pjesma.bin", "rb+");
	if (pF == NULL) {
		createFile();
		pF = fopen("pjesma.bin", "rb+");
	}

	fread(&noPJESMA, sizeof(int), 1, pF);
	tempPJESMA.id = noPJESMA;

	printf("\n");
	printf("*********************************\n");
	printf("*    DODAVANJE PJESME   *\n");

	printf("* 1. IME: ");
	fgets(tempPJESMA.name, sizeof(tempPJESMA.name), stdin);
	tempPJESMA.name[strcspn(tempPJESMA.name, "\n")] = 0;

	do
	{
		printf("* 2. ZANR: ");
		printf("\n0) RAP     1) POP");
		printf("\n2) PUNK    3) HIPHOP");
		printf("\n4) COUNTRY 5) JAZZ");
		printf("\n6) ELECTRONIC    7) DISCO");
		printf("\n8) REGGAE    9) METAL");
		printf("\n9) ROCK 10) RB\n");
		scanf(" %d", &tempPJESMA.genre);
	} while (tempPJESMA.genre < 0 || tempPJESMA.genre > 9);


	printf("* 3. TRAJANJE: ");
	scanf(" %d", &tempPJESMA.duration);

	printf("* 4. GODINA IZDANJA: ");
	scanf(" %d", &tempPJESMA.year);

	printf("* 5. BROJ PRIMJERAKA: ");
	scanf(" %d", &tempPJESMA.copies);
	printf("*********************************\n");

	fseek(pF, sizeof(int) + noPJESMA * sizeof(FILM), SEEK_SET);
	fwrite(&tempPJESMA, sizeof(FILM), 1, pF);

	noPJESMA++;
	rewind(pF);
	fwrite(&noPJESMA, sizeof(int), 1, pF);

	fclose(pF);
}

void showAllFilms(short unsigned enter) {

	clearConsole();

	FILE* pF = fopen("films.bin", "rb");
	int noPJESMA = 0;
	FILM* arrayFilms = NULL;

	if (pF == NULL) {
		createFile();
		pF = fopen("films.bin", "rb");
	}

	readFilms(&noPJESMA, &arrayFilms);
	if (arrayFilms == NULL) {
		printf("\nNema pjesama za ispis!\n");
		fclose(pF);
		getchar();
		return;
	}

	printf("** ISPIS SVIH PJESAMA **\n");
	printf("%-5s %-30s %-10s %-15s %-20s %-15s\n", "ID", "Ime pjesme", "Zanr", "Trajanje", "Godina Izdanja");

	printf("--------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < noPJESMA; i++) {
		printf("%-5d %-30s %-10d %-15d %-20d %-15d\n", (arrayFilms + i)->id, (arrayFilms + i)->name, (arrayFilms + i)->genre, (arrayFilms + i)->duration, (arrayFilms + i)->year, (arrayFilms + i)->copies);
	}

	arrayFilms = NULL;
	free(arrayFilms);
	fclose(pF);

	if (enter) {
		printf("\n'ENTER' za povratak u meni......\n");
		getchar();
	}

}

void deleteFilm() {

	clearConsole();

	int noPJESMA = 0;
	int id = -1;
	FILM* arrayFilms = NULL;

	readFilms(&noPJESMA, &arrayFilms);

	if (noPJESMA == 0) {
		printf("\nNema pjesama za brisanje!\n");
		getchar();
		return;
	}

	if (arrayFilms == NULL) {
		printf("\nNedovoljno memorije za učitavanje pjesama!\n");
		getchar();
		return;
	}

	showAllFilms(0);

	do {
		printf("\nUnesite ID pjesme koju želite obrisati: ");
		scanf(" %d", &id);
	} while (id < 0 || id >= noPJESMA);

	FILE* pF = fopen("films.bin", "wb+");
	if (pF == NULL) {
		printf("\nGreška pri otvaranju datoteke pjesme.bin!\n");
		return;
	}

	fseek(pF, sizeof(int), SEEK_SET);

	for (int i = 0; i < noPJESMA; i++) {
		if ((arrayFilms + i)->id != id) {
			if ((arrayFilms + i)->id > id) {
				(arrayFilms + i)->id--;
			}
			fwrite((arrayFilms + i), sizeof(FILM), 1, pF);
		}
	}

	noPJESMA--;
	rewind(pF);
	fwrite(&noPJESMA, sizeof(int), 1, pF);

	arrayFilms = NULL;
	free(arrayFilms);
	fclose(pF);
}

void updateFilm() {
	clearConsole();
	int noPJESMA = 0;
	int id = -1;
	FILM* arrayFilms = NULL;
	readFilms(&noPJESMA, &arrayFilms);
	if (noPJESMA == 0) {
		printf("\nNema pjesama za uređivanje!\n");
		getchar();
		return;
	}
	if (arrayFilms == NULL) {
		printf("\nNedovoljno memorije za učitavanje pjesama!\n");
		getchar();
		return;
	}
	showAllFilms(0);
	do {
		printf("\nUnesite ID pjesme koji zelite urediti: ");
		scanf(" %d", &id);
		getchar();
	} while (id < 0 || id >= noPJESMA);
	FILM tempPJESMA = { 0 };
	tempPJESMA.id = id;
	printf("\n");
	printf("*********************************\n");
	printf("*    UREĐIVANJE PJESME   *\n");
	printf("* 1. IME: ");
	fgets(tempPJESMA.name, sizeof(tempPJESMA.name), stdin);
	tempPJESMA.name[strcspn(tempPJESMA.name, "\n")] = 0;
	do
	{
		printf("* 2. ZANR: ");
		printf("\n0) RAP     1) POP");
		printf("\n2) PUNK    3) HIPHOP");
		printf("\n4) COUNTRY 5) JAZZ");
		printf("\n6) ELECTRONIC    7) DISCO");
		printf("\n8) REGGAE    9) METAL");
		printf("\n9) ROCK 10) RB\n");
		scanf(" %d", &tempPJESMA.genre);
	} while (tempPJESMA.genre < 0 || tempPJESMA.genre > 9);
	printf("* 3. TRAJANJE: ");
	scanf(" %d", &tempPJESMA.duration);
	printf("* 4. GODINA IZDANJA: ");
	scanf(" %d", &tempPJESMA.year);
	printf("*********************************\n");
	FILE* pF = fopen("films.bin", "rb+");
	if (pF == NULL) {
		printf("\nGreška pri otvaranju datoteke films.bin!\n");
		return;
	}
	fseek(pF, sizeof(int) + id * sizeof(FILM), SEEK_SET);
	fwrite(&tempPJESMA, sizeof(FILM), 1, pF);

	fclose(pF);
	arrayFilms = NULL;
	free(arrayFilms);
}
