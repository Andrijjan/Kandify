#define _CRT_SECURE_NO_WARNINGS
#include "kandify.h"

static Zapis* zapisi = NULL;
static size_t brojZapisa = 0;
static size_t kapacitetZapisa = 0;

static void freeZapisi() {
    if (zapisi) {
        free(zapisi);
        zapisi = NULL;
    }
    brojZapisa = kapacitetZapisa = 0;
}

static int ensureCapacity(size_t minCap) {
    if (kapacitetZapisa >= minCap) return 1;
    size_t newCap = kapacitetZapisa ? kapacitetZapisa * 2 : 1;
    while (newCap < minCap) newCap *= 2;
    Zapis* tmp = realloc(zapisi, newCap * sizeof(Zapis));
    if (!tmp) {
        perror("Greska pri realokaciji memorije");
        return 0;
    }
    zapisi = tmp;
    kapacitetZapisa = newCap;
    return 1;
}

int ucitajZapise() {
    freeZapisi();
    FILE* fp = fopen(trenutna_datoteka, "rb");
    if (!fp) {
        if (errno != ENOENT) perror("Greska pri otvaranju datoteke");
        return 0;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("Greska pri pozicioniranju datoteke");
        fclose(fp);
        return 0;
    }
    long fileSize = ftell(fp);
    if (fileSize < 0) {
        perror("Greska pri odredjivanju velicine datoteke");
        fclose(fp);
        return 0;
    }
    size_t count = fileSize / sizeof(Zapis);
    rewind(fp);
    if (count == 0) {
        fclose(fp);
        return 0;
    }
    zapisi = malloc(count * sizeof(Zapis));
    if (!zapisi) {
        perror("Greska pri alokaciji memorije");
        fclose(fp);
        return 0;
    }
    size_t read = fread(zapisi, sizeof(Zapis), count, fp);
    if (read != count && ferror(fp)) {
        perror("Greska pri citanju zapisa");
        free(zapisi);
        fclose(fp);
        return 0;
    }
    fclose(fp);
    brojZapisa = read;
    kapacitetZapisa = count;
    return (int)brojZapisa;
}

int sacuvajZapise() {
    FILE* fp = fopen(trenutna_datoteka, "wb");
    if (!fp) {
        perror("Greska pri spremanju zapisa");
        return 0;
    }
    size_t napisano = fwrite(zapisi, sizeof(Zapis), brojZapisa, fp);
    if (napisano != brojZapisa) {
        perror("Greska pri pisanju zapisa");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

void dodajZapis() {
    ucitajZapise();
    if (!ensureCapacity(brojZapisa + 1)) return;

    Zapis novi;
    novi.id = (int)brojZapisa + 1;
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
    if (scanf("%d", &novi.godina) != 1) {
        printf("Neispravan unos godine.\n");
        ocistiBuffer();
        return;
    }
    printf("Unesite trajanje (u minutama): ");
    if (scanf("%f", &novi.trajanje) != 1) {
        printf("Neispravan unos trajanja.\n");
        ocistiBuffer();
        return;
    }

    zapisi[brojZapisa++] = novi;
    if (sacuvajZapise())
        printf("Zapis uspjesno dodan.\n");
    else
        printf("Greska pri spremanju zapisa.\n");
}

void prikaziZapise() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za prikaz.\n");
        return;
    }
    printf("\n--- Popis zapisa ---\n");
    for (size_t i = 0; i < brojZapisa; i++) {
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
    if (scanf("%d", &id) != 1) {
        printf("Neispravan unos.\n");
        ocistiBuffer();
        return;
    }

    if (id < 1 || id >(int)brojZapisa) {
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
    if (scanf("%d", &p->godina) != 1) {
        printf("Neispravan unos.\n");
        ocistiBuffer();
        return;
    }

    printf("Trenutno trajanje: %.2f\nNovo trajanje: ", p->trajanje);
    if (scanf("%f", &p->trajanje) != 1) {
        printf("Neispravan unos.\n");
        ocistiBuffer();
        return;
    }

    if (sacuvajZapise())
        printf("Zapis uspjesno uređen.\n");
    else
        printf("Greska pri spremanju zapisa.\n");
}

int binarySearchRecursive(int left, int right, const char* key) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    int cmp = strcmp(zapisi[mid].naziv, key);
    if (cmp == 0) return mid;
    else if (cmp > 0) return binarySearchRecursive(left, mid - 1, key);
    else return binarySearchRecursive(mid + 1, right, key);
}

int cmpNaziv(const void* a, const void* b) {
    const Zapis* pa = a;
    const Zapis* pb = b;
    return strcmp(pa->naziv, pb->naziv);
}

void sortirajZapise() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za sortiranje.\n");
        return;
    }
    qsort(zapisi, brojZapisa, sizeof(Zapis), cmpNaziv);
    for (size_t i = 0; i < brojZapisa; i++)
        zapisi[i].id = (int)i + 1;
    if (sacuvajZapise())
        printf("Zapisi su sortirani po nazivu.\n");
    else
        printf("Greska pri spremanju zapisa.\n");
}

void pretraziZapis() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za pretragu.\n");
        return;
    }
    // Prvo sortiramo zapis po nazivu
    qsort(zapisi, brojZapisa, sizeof(Zapis), cmpNaziv);

    char trazeniNaziv[MAX_NAZIV];
    ocistiBuffer();
    printf("Unesite naziv pjesme za pretragu: ");
    fgets(trazeniNaziv, MAX_NAZIV, stdin);
    trazeniNaziv[strcspn(trazeniNaziv, "\n")] = '\0';

    // Jedan rezultat binarne pretrage
    int idx = binarySearchRecursive(0, (int)brojZapisa - 1, trazeniNaziv);
    if (idx < 0) {
        printf("Nema pjesama sa tim nazivom.\n");
        return;
    }

    // Proširujemo na sve susjedne s istim nazivom
    int lijevo = idx, desno = idx;
    // skeniraj ulijevo
    while (lijevo - 1 >= 0 && strcmp(zapisi[lijevo - 1].naziv, trazeniNaziv) == 0)
        lijevo--;
    // skeniraj udesno
    while (desno + 1 < (int)brojZapisa && strcmp(zapisi[desno + 1].naziv, trazeniNaziv) == 0)
        desno++;

    printf("\n--- Rezultati pretrage ---\n");
    for (int i = lijevo; i <= desno; i++) {
        printf("ID: %d\n", zapisi[i].id);
        printf("Naziv: %s\n", zapisi[i].naziv);
        printf("Autor: %s\n", zapisi[i].autor);
        printf("Zanr: %s\n", zapisi[i].zanr);
        printf("Godina: %d\n", zapisi[i].godina);
        printf("Trajanje: %.2f min\n", zapisi[i].trajanje);
        printf("-----------------------\n");
    }
}


void izbrisiZapis() {
    if (ucitajZapise() == 0) {
        printf("Nema zapisa za brisanje.\n");
        return;
    }
    int id;
    printf("Unesite ID zapisa koji zelite izbrisati: ");
    if (scanf("%d", &id) != 1) {
        printf("Neispravan unos.\n");
        ocistiBuffer();
        return;
    }
    if (id < 1 || id >(int)brojZapisa) {
        printf("Ne postoji zapis sa tim ID.\n");
        return;
    }
    for (size_t i = id - 1; i < brojZapisa - 1; i++) {
        zapisi[i] = zapisi[i + 1];
        zapisi[i].id = (int)i + 1;
    }
    brojZapisa--;
    if (sacuvajZapise())
        printf("Zapis uspjesno obrisan.\n");
    else
        printf("Greska pri spremanju zapisa.\n");
}

void obrisiDatoteku() {
    printf("Jeste li sigurni da zelite obrisati datoteku '%s'? (d/n): ",
        trenutna_datoteka);
    ocistiBuffer();
    char odgovor;
    (void)scanf("%c", &odgovor);
    if (odgovor == 'd' || odgovor == 'D') {
        if (remove(trenutna_datoteka) == 0)
            printf("Datoteka je obrisana.\n");
        else
            perror("Greska pri brisanju datoteke");
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
        perror("Greska pri spremanju konfiguracije");
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
        printf("Datoteka je preimenovana iz '%s' u '%s'.\n",
            trenutna_datoteka, novi_naziv);
        strncpy(trenutna_datoteka, novi_naziv, sizeof(trenutna_datoteka));
        sacuvajNazivDatoteke();
    }
    else {
        perror("Greska pri preimenovanju datoteke");
    }
}

void kopirajDatoteku() {
    char dest[256];
    ocistiBuffer();
    printf("Unesite naziv nove kopije datoteke: ");
    fgets(dest, sizeof(dest), stdin);
    dest[strcspn(dest, "\n")] = '\0';

    FILE* src = fopen(trenutna_datoteka, "rb");
    if (!src) {
        perror("Greska pri otvaranju izvorne datoteke");
        return;
    }
    FILE* dst = fopen(dest, "wb");
    if (!dst) {
        perror("Greska pri otvaranju odredisne datoteke");
        fclose(src);
        return;
    }

    if (fseek(src, 0, SEEK_END) != 0) {
        perror("Greska pri pozicioniranju izvora");
        fclose(src);
        fclose(dst);
        return;
    }
    long size = ftell(src);
    if (size < 0) {
        perror("Greska pri ftell");
        fclose(src);
        fclose(dst);
        return;
    }
    rewind(src);

    char* buffer = malloc((size_t)size);
    if (!buffer) {
        perror("Greska pri alokaciji buffera");
        fclose(src);
        fclose(dst);
        return;
    }
    size_t r = fread(buffer, 1, (size_t)size, src);
    if (r != (size_t)size) {
        perror("Greska pri citanju izvora");
        free(buffer);
        fclose(src);
        fclose(dst);
        return;
    }
    size_t w = fwrite(buffer, 1, r, dst);
    if (w != r) perror("Greska pri pisanju odredisne datoteke");

    free(buffer);
    fclose(src);
    fclose(dst);
}
