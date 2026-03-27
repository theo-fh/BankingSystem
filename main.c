#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATEI ("../accounts")
#define ZEILENLAENGE (int zeilenlaenge = 62;)

struct Konto {
    int guthaben; //in ct
    char inhaber[40];
};

void clearInput();
int anzahlKonten(FILE*); //Nicht mehr richtig, sobald newAccount() benutzt wurde!!!
void auslesen(FILE*, struct Konto*, int);
int writeToFile();
void newAccount(struct Konto*, int*);
int withdraw();
int deposit();
int transfer();

int main() {

    FILE* accounts = fopen(DATEI, "r+");
    if (accounts == NULL) {
        printf("Fehler beim lesen der Datei.");
        exit(1);
    }

    int anzahl = anzahlKonten(accounts);
    int *anzahlPtr = &anzahl;
    struct Konto *konten = (struct Konto*) malloc(anzahl*sizeof(struct Konto)); //Gibt Warnung über Memory Leak, hab ich mich aber eigentlich drum gekümmert...

    auslesen(accounts, konten, anzahl);

    char operation = 'n';
    if (operation == 'n') {


        struct Konto *tmpAos = realloc(konten, anzahl*sizeof(struct Konto) + sizeof(struct Konto)); // Platz für weiteren Eintrag
        if (konten == NULL) {
            printf("Fehler bei Speicherzuweisung.");
            free(tmpAos);
            exit(1);
        }
        else {

            konten = tmpAos;

            newAccount(konten, anzahlPtr);

            printf("\nKonto erfolgreich erstellt.");
        }
    } //neues Konto anlegen

    free(konten);
    fclose(accounts);

    return 0;
}



//FUNKTIONEN

void clearInput() {
    int c;
    while ((c = getchar()) != EOF && c != '\n') {}
}

int anzahlKonten(FILE* datei) {

    //Menge der char in der File lesen (bis EOF)
    char c;
    int i = 0;
    while ((c = fgetc(datei)) != EOF) {
        i++;
    }

    //daraus Anzahl der Konten (--> feste Zeilenlänge)
    int zeilenlänge = 62;
    int anzahlKonten = i/zeilenlänge;

    return anzahlKonten + 1;
}

void auslesen(FILE* datei, struct Konto *aos, int anzahl) {
    int zeilenlaenge = 62;
    fseek(datei, 0, SEEK_SET);
    for (int kontoNr = 0; kontoNr < anzahl; kontoNr++) {
        fseek(datei, zeilenlaenge * kontoNr, SEEK_SET);
        char guthabenStr[30] = {0};
        char inhaberStr[40] = {0};
        int n = 0;
        char c;
        while ((c = fgetc(datei)) != ' ') {
            guthabenStr[n] = c;
            n++;
        } //Guthaben des Kontos lesen
        guthabenStr[n] = '\0'; //String kürzen, damit nur "Integer" und '\0' enthalten sind
        int stellen = n-1;

        int guthaben = 0;
        for (int i = stellen; i >= 0; i--) {
            guthaben += ((int) guthabenStr[i] - 48)*pow(10, stellen - i);
        }
        (aos + kontoNr)->guthaben = guthaben;

        fseek(datei, (long int) zeilenlaenge * kontoNr + 20, SEEK_SET);
        n = 0;
        while ((c = fgetc(datei)) != '/') {
            inhaberStr[n] = c;
            n++;
        } //Inhaber des Kontos lesen

        for (int i = 39; i >= 0; i--) {
            if (inhaberStr[i] != ' ') {
                inhaberStr[i+1] = '\0';
                break;
            }
        }

        strcpy((aos + kontoNr)->inhaber, inhaberStr);
    }
}

int writeToFile() {
    FILE* accounts = fopen(DATEI, "w");

    //an dem Skript unten orientieren

    /*int stellen = log10(startguthaben)+1;
    char info[61];


    sprintf(info, "%d", startguthaben);
    for (int i = stellen; i<20; i++) {
        info[i] = ' ';
    }
    sprintf("%s", inhaber);

    info[59] = '\n';
    int i = 20;
    while (info[i] != '\n') {
        if (info[i] == 0)
            info[i] = ' ';
        i++;
    }

    fprintf(datei, info);
    --> funktioniert noch nicht ganz, aber hilfreich für das schreiben von AOS in Datei
    */


    fclose(accounts);
}

void newAccount(struct Konto *aos, int* anzahlPtr) {
    //Anzahl der Konten intern erhöhen
    int anzahl = *(anzahlPtr) + 1;

    char inhaberNeu[40];
    int startguthaben;
    printf("NEUES KONTO \nInhaber:");
    fgets(inhaberNeu, 40, stdin);


    // Newline aus dem Input entfernen
    for (int i = 0; i < 40; i++) {
        if (inhaberNeu[i] == '\n') {
            inhaberNeu[i] = '\0';
            break;
        }
    }


    printf("Startguthaben in Cent: ");
    scanf(" %d", &startguthaben);
    clearInput();

    //Angaben in AOS übernehmen
    strcpy((aos + anzahl - 1)->inhaber, inhaberNeu);
    (aos + anzahl - 1)->guthaben = startguthaben;

    //Anzahl d. Konten global erhöhen
    *anzahlPtr += 1;
}

int withdraw() {

}

int deposit() {

}

int transfer() {

} //Kombination aus withdraw und deposit
