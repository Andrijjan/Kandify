#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"
#include <stdio.h>

void prikaziGlavniMeni() {
    int izbor;
    do {
        printf("\n===================================\n");
        printf("         >>> Glavni Meni <<<        \n");
        printf("===================================\n");
        printf("1. Meni pjesama\n");
        printf("0. Izlaz\n");
        printf("-----------------------------------\n");
        printf("Unesite izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            prikaziMeniPjesama();
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Pogresan unos.\n");
        }
    } while (izbor != 0);
}
