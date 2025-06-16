#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

void prikaziMeniPjesama() {
    MeniPjesamaOpcije izbor;
    do {
        printf("\n===================================\n");
        printf("      [>>> Meni Pjesama <<<]       \n");
        printf("===================================\n");
        printf("1. Dodaj novi zapis\n");
        printf("2. Prikazi sve zapise\n");
        printf("3. Uredi zapis\n");
        printf("4. Izbrisi zapis\n");
        printf("5. Sortiraj zapise\n");
        printf("6. Pretrazi zapis\n");
        printf("7. Obrisi datoteku\n");
        printf("8. Preimenuj datoteku\n");
        printf("9. Kopiraj datoteku\n");
        printf("0. Povratak na glavni meni\n");
        printf("-----------------------------------\n");
        printf("Unesite izbor: ");

        if (scanf("%d", (int*)&izbor) != 1) {
            printf("Pogresan unos.\n");
            ocistiBuffer();
            continue;
        }

        switch (izbor) {
        case DODAJ_ZAPIS:        dodajZapis();        break;
        case PRIKAZI_ZAPISE:     prikaziZapise();     break;
        case UREDI_ZAPIS:        urediZapis();        break;
        case IZBRISI_ZAPIS:      izbrisiZapis();      break;
        case SORTIRAJ_ZAPISE:    sortirajZapise();    break;
        case PRETRAZI_ZAPIS:     pretraziZapis();     break;
        case BRISI_DATOTEKU:     obrisiDatoteku();    break;
        case PREIMENUJ_DATOTEKU: preimenujDatoteku(); break;
        case KOPIRAJ_DATOTEKU:   kopirajDatoteku();   break;
        case POVRATAK:           break;
        default:                 printf("Pogresan unos.\n"); break;
        }
    } while (izbor != POVRATAK);
}
