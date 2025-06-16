#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

char trenutna_datoteka[256]; 

int main() {
    if (!ucitajNazivDatoteke()) {
        strcpy(trenutna_datoteka, "datoteka.bin");
    }
    prikaziGlavniMeni();
    return 0;
}
