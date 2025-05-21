#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"
#include <stdio.h>

void prikaziMeniPjesama() {
    int izbor;
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
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            dodajZapis();
            break;
        case 2:
            printf("\n|+==============================+|\n");
            printf("|--> Trenutni zapis pjesama:  <--|\n");
            printf("|+==============================+|\n");
            prikaziZapise();
            break;
        case 3:
            urediZapis();
            break;
        case 4:
            izbrisiZapis();
            break;
        case 0:
            break;
        default:
            printf("Pogresan unos.\n");
        }
    } while (izbor != 0);
}
