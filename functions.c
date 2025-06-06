#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

static Zapis zapisi[MAX_ZAPISA];
static int brojZapisa = 0;

int ucitajZapise() {
    FILE* fp = fopen(trenutna_datoteka, "rb");
    if (!fp) {
        brojZapisa = 0;
        return 0;
    }
    brojZapisa = 0;
    while (fread(&zapisi[brojZapisa], sizeof(Zapis), 1, fp) == 1 && brojZapisa < MAX_ZAPISA) {
        brojZapisa++;
    }
    fclose(fp);
    return brojZapisa;
}

int sacuvajZapise() {
    FILE* fp = fopen(trenutna_datoteka, "wb");
    if (!fp) {
        perror("Greska pri spremanju zapisa");
        return 0;
    }
    size_t napisano = fwrite(zapisi, sizeof(Zapis), brojZapisa, fp);
    fclose(fp);
    return (napisano == brojZapisa);
}

void dodajZapis() {
    ucitajZapise();

    if (brojZapisa >= MAX_ZAPISA) {
        printf("Nije moguce dodati vise zapisa, dostignut maksimum.\n");
        return;
    }

    Zapis novi;
    novi.id = brojZapisa + 1;

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

    zapisi[brojZapisa++] = novi;

    if (sacuvajZapise()) {
        printf("Zapis uspjesno dodan.\n");
    }
    else {
        printf("Greska pri spremanju zapisa.\n");
    }
}

void prikaziZapise() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za prikaz.\n");
        return;
    }

    printf("\n--- Popis zapisa ---\n");
    for (int i = 0; i < brojZapisa; i++) {
        printf("ID: %d\n", zapisi[i].id);
        printf("Naziv: %s\n", zapisi[i].naziv);
        printf("Autor: %s\n", zapisi[i].autor);
        printf("Zanr: %s\n", zapisi[i].zanr);
        printf("Godina: %d\n", zapisi[i].godina);
        printf("Trajanje: %.2f min\n", zapisi[i].trajanje);
        printf("-----------------------\n");
    }
}

void urediZapis() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za uredjivanje.\n");
        return;
    }

    int id;
    printf("Unesite ID zapisa koji zelite urediti: ");
    scanf("%d", &id);

    if (id < 1 || id > brojZapisa) {
        printf("Ne postoji zapis sa tim ID.\n");
        return;
    }

    Zapis* p = &zapisi[id - 1];

    ocistiBuffer();

    printf("Trenutni naziv: %s\nNovi naziv: ", p->naziv);
    fgets(p->naziv, MAX_NAZIV, stdin);
    p->naziv[strcspn(p->naziv, "\n")] = '\0';

    printf("Trenutni autor: %s\nNovi autor: ", p->autor);
    fgets(p->autor, MAX_AUTOR, stdin);
    p->autor[strcspn(p->autor, "\n")] = '\0';

    printf("Trenutni zanr: %s\nNovi zanr: ", p->zanr);
    fgets(p->zanr, MAX_ZANR, stdin);
    p->zanr[strcspn(p->zanr, "\n")] = '\0';

    printf("Trenutna godina: %d\nNova godina: ", p->godina);
    scanf("%d", &p->godina);

    printf("Trenutno trajanje: %.2f\nNovo trajanje: ", p->trajanje);
    scanf("%f", &p->trajanje);

    if (sacuvajZapise()) {
        printf("Zapis uspjesno uređen.\n");
    }
    else {
        printf("Greska pri spremanju zapisa.\n");
    }
}

void izbrisiZapis() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za brisanje.\n");
        return;
    }

    int id;
    printf("Unesite ID zapisa koji zelite izbrisati: ");
    scanf("%d", &id);

    if (id < 1 || id > brojZapisa) {
        printf("Ne postoji zapis sa tim ID.\n");
        return;
    }

    // Pomjeri sve zapise iza obrisanog za jedno mjesto unaprijed
    for (int i = id - 1; i < brojZapisa - 1; i++) {
        zapisi[i] = zapisi[i + 1];
        zapisi[i].id = i + 1;  // prilagodi ID
    }
    brojZapisa--;

    if (sacuvajZapise()) {
        printf("Zapis uspjesno obrisan.\n");
    }
    else {
        printf("Greska pri spremanju zapisa.\n");
    }
}

int cmpNaziv(const void* a, const void* b) {
    const Zapis* pa = (const Zapis*)a;
    const Zapis* pb = (const Zapis*)b;
    return strcmp(pa->naziv, pb->naziv);
}

void sortirajZapise() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za sortiranje.\n");
        return;
    }

    qsort(zapisi, brojZapisa, sizeof(Zapis), cmpNaziv);

    // Nakon sortiranja prilagodi ID-eve
    for (int i = 0; i < brojZapisa; i++) {
        zapisi[i].id = i + 1;
    }

    if (sacuvajZapise()) {
        printf("Zapisi su sortirani po nazivu.\n");
    }
    else {
        printf("Greska pri spremanju zapisa.\n");
    }
}

void pretraziZapis() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za pretragu.\n");
        return;
    }

    char trazeniNaziv[MAX_NAZIV];
    ocistiBuffer();

    printf("Unesite naziv pjesme za pretragu: ");
    fgets(trazeniNaziv, MAX_NAZIV, stdin);
    trazeniNaziv[strcspn(trazeniNaziv, "\n")] = '\0';

    int pronadeno = 0;
    printf("\n--- Rezultati pretrage ---\n");
    for (int i = 0; i < brojZapisa; i++) {
        if (strcmp(zapisi[i].naziv, trazeniNaziv) == 0) {
            printf("ID: %d\n", zapisi[i].id);
            printf("Naziv: %s\n", zapisi[i].naziv);
            printf("Autor: %s\n", zapisi[i].autor);
            printf("Zanr: %s\n", zapisi[i].zanr);
            printf("Godina: %d\n", zapisi[i].godina);
            printf("Trajanje: %.2f min\n", zapisi[i].trajanje);
            printf("-----------------------\n");
            pronadeno++;
        }
    }

    if (!pronadeno) {
        printf("Nema pjesama sa tim nazivom.\n");
    }
}

void obrisiDatoteku() {
    printf("Jeste li sigurni da zelite obrisati datoteku '%s'? (d/n): ", trenutna_datoteka);
    ocistiBuffer();
    char odgovor;
    scanf("%c", &odgovor);

    if (odgovor == 'd' || odgovor == 'D') {
        if (remove(trenutna_datoteka) == 0) {
            printf("Datoteka je obrisana.\n");
            brojZapisa = 0;
        }
        else {
            perror("Greska pri brisanju datoteke");
        }
    }
    else {
        printf("Brisanje datoteke prekinuto.\n");
    }
}

int ucitajNazivDatoteke() {
    FILE* fp = fopen(CONFIG_FILE, "r");
    if (!fp) return 0;

    if (fgets(trenutna_datoteka, sizeof(trenutna_datoteka), fp)) {
        trenutna_datoteka[strcspn(trenutna_datoteka, "\n")] = '\0';
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

void sacuvajNazivDatoteke() {
    FILE* fp = fopen(CONFIG_FILE, "w");
    if (!fp) {
        perror("Greska pri spremanju konfiguracijske datoteke");
        return;
    }
    fprintf(fp, "%s\n", trenutna_datoteka);
    fclose(fp);
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
        sacuvajNazivDatoteke();
    }
    else {
        perror("Greska pri preimenovanju datoteke");
    }
}
