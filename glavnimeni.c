#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

void prikaziGlavniMeni() {
    GlavniMeniOpcije izbor;
    do {
        printf("\n===================================\n");
        printf("         >>> Glavni Meni <<<        \n");
        printf("===================================\n");
        printf("1. Meni pjesama\n");
        printf("0. Izlaz\n");
        printf("-----------------------------------\n");
        printf("Unesite izbor: ");
        scanf("%d", (int*)&izbor);

        switch (izbor) {
        case MENI_PJESAMA:
            prikaziMeniPjesama();
            break;
        case IZLAZ:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Pogresan unos.\n");
        }
    } while (izbor != IZLAZ);
}
