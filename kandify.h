#ifndef KANDIFY_H
#define KANDIFY_H

#include <stdio.h>

#define MAX_NAZIV 100
#define MAX_AUTOR 100
#define MAX_ZANR 50
#define MAX_ZAPISA 100
#define DATOTEKA "datoteka.bin"

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
    IZBRISI_ZAPIS = 4
} MeniPjesamaOpcije;

void prikaziGlavniMeni();
void prikaziMeniPjesama();
void dodajZapis();
void prikaziZapise();
void urediZapis();
void izbrisiZapis();

static inline void ocistiBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

#endif
