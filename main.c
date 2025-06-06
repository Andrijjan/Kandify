#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

char trenutna_datoteka[256];  // Definicija globalne varijable

int main() {
    // Učitaj naziv datoteke iz konfiguracije, ako nema postavi default
    if (!ucitajNazivDatoteke()) {
        strcpy(trenutna_datoteka, "datoteka.bin");
    }

    prikaziGlavniMeni();
    return 0;
}
