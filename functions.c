#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ucitajZapise(Zapis zapisi[], int max) {
    FILE* fp = fopen(DATOTEKA, "rb");
    if (!fp) return 0;

    int count = 0;
    while (fread(&zapisi[count], sizeof(Zapis), 1, fp) == 1 && count < max) {
        count++;
    }

    fclose(fp);
    return count;
}

void dodajZapis() {
    Zapis zapisi[MAX_ZAPISA];
    int count = ucitajZapise(zapisi, MAX_ZAPISA);

    Zapis novi;
    novi.id = count + 1;
    getchar();

    printf("Unesite naziv: ");
    fgets(novi.naziv, MAX_NAZIV, stdin);
    novi.naziv[strcspn(novi.naziv, "\n")] = '\0';

    printf("Unesite autora: ");
    fgets(novi.autor, MAX_AUTOR, stdin);
    novi.autor[strcspn(novi.autor, "\n")] = '\0';

    printf("Unesite zanr: ");
    fgets(novi.zanr, MAX_ZANR, stdin);
    novi.zanr[strcspn(novi.zanr, "\n")] = '\0';

    printf("Unesite godinu: ");
    scanf("%d", &novi.godina);

    printf("Unesite trajanje (u minutama): ");
    scanf("%f", &novi.trajanje);

    FILE* fp = fopen(DATOTEKA, "ab");
    if (!fp) {
        printf("Greska pri otvaranju datoteke.\n");
        return;
    }

    fwrite(&novi, sizeof(Zapis), 1, fp);
    fclose(fp);

    printf("Zapis uspjesno dodan.\n");
}

void prikaziZapise() {
    Zapis z;
    FILE* fp = fopen(DATOTEKA, "rb");
    if (!fp) {
        printf("Greska pri otvaranju datoteke.\n");
        return;
    }

    while (fread(&z, sizeof(Zapis), 1, fp) == 1) {
        printf("\nID: %d | Naziv: %s | Autor: %s | Zanr: %s | Godina: %d | Trajanje: %.2f min\n",
            z.id, z.naziv, z.autor, z.zanr, z.godina, z.trajanje);
    }

    fclose(fp);
}

void urediZapis() {
    Zapis zapisi[MAX_ZAPISA];
    int count = ucitajZapise(zapisi, MAX_ZAPISA);

    int id;
    printf("Unesite ID zapisa za uredjivanje: ");
    scanf("%d", &id);
    getchar();

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (zapisi[i].id == id) {
            found = 1;
            printf("Uredjivanje zapisa ID %d:\n", id);

            printf("Trenutni naziv: %s\nNovi naziv: ", zapisi[i].naziv);
            fgets(zapisi[i].naziv, MAX_NAZIV, stdin);
            zapisi[i].naziv[strcspn(zapisi[i].naziv, "\n")] = '\0';

            printf("Trenutni autor: %s\nNovi autor: ", zapisi[i].autor);
            fgets(zapisi[i].autor, MAX_AUTOR, stdin);
            zapisi[i].autor[strcspn(zapisi[i].autor, "\n")] = '\0';

            printf("Trenutni zanr: %s\nNovi zanr: ", zapisi[i].zanr);
            fgets(zapisi[i].zanr, MAX_ZANR, stdin);
            zapisi[i].zanr[strcspn(zapisi[i].zanr, "\n")] = '\0';

            printf("Trenutna godina: %d\nNova godina: ", zapisi[i].godina);
            scanf("%d", &zapisi[i].godina);

            printf("Trenutno trajanje: %.2f\nNovo trajanje: ", zapisi[i].trajanje);
            scanf("%f", &zapisi[i].trajanje);
            getchar(); // za Enter nakon float
            break;
        }
    }

    if (!found) {
        printf("Zapis s ID %d nije pronadjen.\n", id);
        return;
    }

    FILE* fp = fopen(DATOTEKA, "wb");
    for (int i = 0; i < count; i++) {
        fwrite(&zapisi[i], sizeof(Zapis), 1, fp);
    }
    fclose(fp);

    printf("Zapis uspjesno azuriran.\n");
}

void izbrisiZapis() {
    Zapis zapisi[MAX_ZAPISA];
    int count = ucitajZapise(zapisi, MAX_ZAPISA);

    int id;
    printf("Unesite ID zapisa za brisanje: ");
    scanf("%d", &id);

    int noviIndex = 0;
    for (int i = 0; i < count; i++) {
        if (zapisi[i].id != id) {
            zapisi[noviIndex++] = zapisi[i];
        }
    }

    for (int i = 0; i < noviIndex; i++) {
        zapisi[i].id = i + 1;
    }

    FILE* fp = fopen(DATOTEKA, "wb");
    for (int i = 0; i < noviIndex; i++) {
        fwrite(&zapisi[i], sizeof(Zapis), 1, fp);
    }
    fclose(fp);

    printf("Zapis izbrisan i ID-evi azurirani.\n");
}
