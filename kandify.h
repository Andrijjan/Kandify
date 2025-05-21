#ifndef KANDIFY_H
#define KANDIFY_H

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

void prikaziGlavniMeni();
void prikaziMeniPjesama();
void dodajZapis();
void prikaziZapise();
void urediZapis();
void izbrisiZapis();
int ucitajZapise(Zapis zapisi[], int max);

#endif
