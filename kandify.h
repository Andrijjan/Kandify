#ifndef KANDIFY_H
#define KANDIFY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_NAZIV 100
#define MAX_AUTOR 100
#define MAX_ZANR 50

#define CONFIG_FILE "config.txt"

extern char trenutna_datoteka[256];

typedef struct {
    int id;
    char naziv[MAX_NAZIV];
    char autor[MAX_AUTOR];
    char zanr[MAX_ZANR];
    int godina;
    float trajanje;
} Zapis;

typedef enum {
    IZLAZ = 0,
    MENI_PJESAMA = 1
} GlavniMeniOpcije;

typedef enum {
    POVRATAK = 0,
    DODAJ_ZAPIS = 1,
    PRIKAZI_ZAPISE = 2,
    UREDI_ZAPIS = 3,
    IZBRISI_ZAPIS = 4,
    SORTIRAJ_ZAPISE = 5,
    PRETRAZI_ZAPIS = 6,
    BRISI_DATOTEKU = 7,
    PREIMENUJ_DATOTEKU = 8,
    KOPIRAJ_DATOTEKU = 9
} MeniPjesamaOpcije;

void prikaziGlavniMeni();
void prikaziMeniPjesama();

void dodajZapis();
void prikaziZapise();
void urediZapis();
void izbrisiZapis();
void sortirajZapise();
void pretraziZapis();
void obrisiDatoteku();
void preimenujDatoteku();
void kopirajDatoteku();

int ucitajNazivDatoteke();
void sacuvajNazivDatoteke();

int ucitajZapise();
int sacuvajZapise();

int binarySearchRecursive(int left, int right, const char* key);
int cmpNaziv(const void* a, const void* b);

static inline void ocistiBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

#endif // KANDIFY_H
