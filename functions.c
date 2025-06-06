#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

char trenutna_datoteka[256] = "datoteka.bin";

// Usporedba po nazivu za sortiranje i pretragu
int usporediNazive(const void* a, const void* b) {
    const Zapis* za = (const Zapis*)a;
    const Zapis* zb = (const Zapis*)b;
    return strcmp(za->naziv, zb->naziv);
}

// Usporedba po ID-u za sortiranje
int usporediID(const void* a, const void* b) {
    const Zapis* za = (const Zapis*)a;
    const Zapis* zb = (const Zapis*)b;
    return za->id - zb->id;
}

// Učitaj sve zapise iz datoteke u dinamički alocirani niz
static int ucitajZapise(Zapis** zapisi) {
    FILE* fp = fopen(trenutna_datoteka, "rb");
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

// Sačuvaj sve zapise u datoteku (prepiši postojeću)
static int sacuvajZapise(Zapis* zapisi, int count) {
    FILE* fp = fopen(trenutna_datoteka, "wb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke za pisanje");
        return 0;
    }

    int napisano = fwrite(zapisi, sizeof(Zapis), count, fp);
    fclose(fp);
    if (napisano != count) {
        perror("Greska pri pisanju u datoteku");
        return 0;
    }
    return 1;
}

void dodajZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);
    if (count >= MAX_ZAPISA) {
        printf("Dostignut maksimalni broj zapisa.\n");
        free(zapisi);
        return;
    }

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

    // Dodaj novi zapis u niz
    zapisi = (Zapis*)realloc(zapisi, (count + 1) * sizeof(Zapis));
    if (!zapisi) {
        perror("realloc");
        return;
    }
    zapisi[count] = novi;
    count++;

    if (sacuvajZapise(zapisi, count)) {
        printf("Zapis uspjesno dodan.\n");
    }

    free(zapisi);
}

void prikaziZapise() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);
    if (count == 0) {
        printf("Nema zapisa za prikaz.\n");
        return;
    }

    printf("\n--- Svi zapisi ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d\nNaziv: %s\nAutor: %s\nZanr: %s\nGodina: %d\nTrajanje: %.2f min\n",
            zapisi[i].id, zapisi[i].naziv, zapisi[i].autor, zapisi[i].zanr, zapisi[i].godina, zapisi[i].trajanje);
        printf("-------------------------\n");
    }
    free(zapisi);
}

void urediZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);
    if (count == 0) {
        printf("Nema zapisa za uređivanje.\n");
        return;
    }

    int idZaUredivanje;
    printf("Unesite ID zapisa za uređivanje: ");
    scanf("%d", &idZaUredivanje);

    if (idZaUredivanje < 1 || idZaUredivanje > count) {
        printf("Ne postoji zapis s tim ID-em.\n");
        free(zapisi);
        return;
    }

    int index = idZaUredivanje - 1;

    printf("Trenutni naziv: %s\n", zapisi[index].naziv);
    printf("Unesite novi naziv (ENTER za ostaviti isti): ");
    ocistiBuffer();
    char unos[MAX_NAZIV];
    fgets(unos, MAX_NAZIV, stdin);
    if (unos[0] != '\n') {
        unos[strcspn(unos, "\n")] = '\0';
        strncpy(zapisi[index].naziv, unos, MAX_NAZIV);
    }

    printf("Trenutni autor: %s\n", zapisi[index].autor);
    printf("Unesite novog autora (ENTER za ostaviti isti): ");
    fgets(unos, MAX_AUTOR, stdin);
    if (unos[0] != '\n') {
        unos[strcspn(unos, "\n")] = '\0';
        strncpy(zapisi[index].autor, unos, MAX_AUTOR);
    }

    printf("Trenutni zanr: %s\n", zapisi[index].zanr);
    printf("Unesite novi zanr (ENTER za ostaviti isti): ");
    fgets(unos, MAX_ZANR, stdin);
    if (unos[0] != '\n') {
        unos[strcspn(unos, "\n")] = '\0';
        strncpy(zapisi[index].zanr, unos, MAX_ZANR);
    }

    printf("Trenutna godina: %d\n", zapisi[index].godina);
    printf("Unesite novu godinu (0 za ostaviti istu): ");
    int god;
    scanf("%d", &god);
    if (god != 0) {
        zapisi[index].godina = god;
    }

    printf("Trenutno trajanje: %.2f\n", zapisi[index].trajanje);
    printf("Unesite novo trajanje (0 za ostaviti isto): ");
    float traj;
    scanf("%f", &traj);
    if (traj != 0.0f) {
        zapisi[index].trajanje = traj;
    }

    if (sacuvajZapise(zapisi, count)) {
        printf("Zapis uspjesno uređen.\n");
    }

    free(zapisi);
}

void izbrisiZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);
    if (count == 0) {
        printf("Nema zapisa za brisanje.\n");
        return;
    }

    int idZaBrisanje;
    printf("Unesite ID zapisa za brisanje: ");
    scanf("%d", &idZaBrisanje);

    if (idZaBrisanje < 1 || idZaBrisanje > count) {
        printf("Ne postoji zapis s tim ID-em.\n");
        free(zapisi);
        return;
    }

    // Pomaknuti sve zapise nakon izbrisanog za jedno mjesto
    int index = idZaBrisanje - 1;
    for (int i = index; i < count - 1; i++) {
        zapisi[i] = zapisi[i + 1];
        zapisi[i].id = i + 1;  // prilagodba ID-a
    }
    count--;

    if (sacuvajZapise(zapisi, count)) {
        printf("Zapis uspjesno izbrisan.\n");
    }

    free(zapisi);
}

void sortirajZapise() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);
    if (count == 0) {
        printf("Nema zapisa za sortiranje.\n");
        return;
    }

    int izbor;
    printf("Sortiraj po:\n1. Nazivu\n2. ID-u\nUnesite izbor: ");
    scanf("%d", &izbor);

    if (izbor == 1) {
        qsort(zapisi, count, sizeof(Zapis), usporediNazive);
    }
    else if (izbor == 2) {
        qsort(zapisi, count, sizeof(Zapis), usporediID);
    }
    else {
        printf("Pogresan izbor.\n");
        free(zapisi);
        return;
    }

    if (sacuvajZapise(zapisi, count)) {
        printf("Zapisi su uspjesno sortirani.\n");
    }

    free(zapisi);
}

void pretraziZapis() {
    Zapis* zapisi = NULL;
    int count = ucitajZapise(&zapisi);
    if (count == 0) {
        printf("Nema zapisa za pretragu.\n");
        return;
    }

    char trazeni[MAX_NAZIV];
    ocistiBuffer();
    printf("Unesite naziv za pretragu: ");
    fgets(trazeni, MAX_NAZIV, stdin);
    trazeni[strcspn(trazeni, "\n")] = '\0';

    // Sortiraj prije pretrage (bsearch zahtijeva sortirano polje)
    qsort(zapisi, count, sizeof(Zapis), usporediNazive);

    Zapis kljuc;
    memset(&kljuc, 0, sizeof(Zapis));
    strncpy(kljuc.naziv, trazeni, MAX_NAZIV);

    Zapis* rezultat = (Zapis*)bsearch(&kljuc, zapisi, count, sizeof(Zapis), usporediNazive);

    if (!rezultat) {
        printf("Nema pjesama s nazivom '%s'.\n", trazeni);
        free(zapisi);
        return;
    }

    int index = (int)(rezultat - zapisi);

    // Ispis svih zapisa s traženim nazivom
    int i = index;
    while (i >= 0 && strcmp(zapisi[i].naziv, trazeni) == 0) i--;
    i++; // vrati se na prvi koji ima isti naziv

    printf("\n--- Pjesme s nazivom '%s' ---\n", trazeni);

    while (i < count && strcmp(zapisi[i].naziv, trazeni) == 0) {
        printf("ID: %d\nNaziv: %s\nAutor: %s\nZanr: %s\nGodina: %d\nTrajanje: %.2f min\n",
            zapisi[i].id, zapisi[i].naziv, zapisi[i].autor, zapisi[i].zanr, zapisi[i].godina, zapisi[i].trajanje);
        printf("-------------------------\n");
        i++;
    }

    free(zapisi);
}

void obrisiDatoteku() {
    if (remove(trenutna_datoteka) == 0) {
        printf("Datoteka '%s' uspjesno obrisana.\n", trenutna_datoteka);
    }
    else {
        perror("Greska pri brisanju datoteke");
    }
}

void preimenujDatoteku() {
    char novi_naziv[256];
    ocistiBuffer();

    printf("Unesite novi naziv datoteke (npr. nova_datoteka.bin): ");
    fgets(novi_naziv, sizeof(novi_naziv), stdin);
    novi_naziv[strcspn(novi_naziv, "\n")] = '\0';

    if (strlen(novi_naziv) == 0) {
        printf("Neispravan naziv datoteke.\n");
        return;
    }

    if (rename(trenutna_datoteka, novi_naziv) == 0) {
        printf("Datoteka je preimenovana iz '%s' u '%s'.\n", trenutna_datoteka, novi_naziv);
        strncpy(trenutna_datoteka, novi_naziv, sizeof(trenutna_datoteka));
    }
    else {
        perror("Greska pri preimenovanju datoteke");
    }
}

