#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"
#include <stdlib.h>
#include <string.h>

static int ucitajZapise(Zapis** zapisi) {
    FILE* fp = fopen(DATOTEKA, "rb");
    if (!fp) return 0;

    *zapisi = (Zapis*)malloc(MAX_ZAPISA * sizeof(Zapis));
    if (!*zapisi) {
        perror("malloc");
        fclose(fp);
        return 0;
    }

    int count = 0;
    while (fread(&(*zapisi)[count], sizeof(Zapis), 1, fp) == 1 && count < MAX_ZAPISA) {
        count++;
    }

    fclose(fp);
    return count;
}

void dodajZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);

    Zapis novi;
    novi.id = count + 1;
    ocistiBuffer();

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
        perror("Greska pri otvaranju datoteke");
        free(zapisi);
        return;
    }

    fwrite(&novi, sizeof(Zapis), 1, fp);
    fclose(fp);
    free(zapisi);

    printf("Zapis uspjesno dodan.\n");
}

void prikaziZapise() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);

    for (int i = 0; i < count; i++) {
        Zapis z = zapisi[i];
        printf("\nID: %d | Naziv: %s | Autor: %s | Zanr: %s | Godina: %d | Trajanje: %.2f min\n",
            z.id, z.naziv, z.autor, z.zanr, z.godina, z.trajanje);
    }

    free(zapisi);
}

void urediZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);

    int id;
    printf("Unesite ID zapisa za uredjivanje: ");
    scanf("%d", &id);
    ocistiBuffer();

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (zapisi[i].id == id) {
            found = 1;
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
            ocistiBuffer();
            break;
        }
    }

    if (!found) {
        printf("Zapis s ID %d nije pronadjen.\n", id);
        free(zapisi);
        return;
    }

    FILE* fp = fopen(DATOTEKA, "wb");
    for (int i = 0; i < count; i++) {
        fwrite(&zapisi[i], sizeof(Zapis), 1, fp);
    }
    fclose(fp);
    free(zapisi);

    printf("Zapis uspjesno azuriran.\n");
}

void izbrisiZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);

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
    free(zapisi);

    printf("Zapis izbrisan i ID-evi azurirani.\n");
}
