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
        printf("0. Povratak na glavni meni\n");
        printf("-----------------------------------\n");
        printf("\nUnesite izbor: ");
        scanf("%d", (int*)&izbor);

        switch (izbor) {
        case DODAJ_ZAPIS:
            dodajZapis();
            break;
        case PRIKAZI_ZAPISE:
            printf("\n|+==============================+|\n");
            printf("|--> Trenutni zapis pjesama:  <--|\n");
            printf("|+==============================+|\n");
            prikaziZapise();
            break;
        case UREDI_ZAPIS:
            urediZapis();
            break;
        case IZBRISI_ZAPIS:
            izbrisiZapis();
            break;
        case POVRATAK:
            break;
        default:
            printf("Pogresan unos.\n");
        }
    } while (izbor != POVRATAK);
}
