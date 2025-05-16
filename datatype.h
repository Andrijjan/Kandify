#ifndef DATATYPE_H
#define DATATYPE_H

typedef enum genre {
	RAP,
	POP,
	PUNK,
	HIPHOP,
	COUNTRY,
	JAZZ,
	ELECTRONIC,
	DISCO,
	REGGAE,
	METAL,
	ROCK,
	RB
}GENRE;

typedef struct pjesma {
	int id;
	char name[51];
	GENRE genre;
	int duration;
	int year;
}PJESMA;

typedef struct user {
	int id;
	char name[21];
	char lastname[21];
	short noPJESMA;
	int* PJESMA;
} USER;
#endif
#pragma once
