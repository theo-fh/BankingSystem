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
int newAccount(struct Konto*, int*);
int withdraw(struct Konto*, int, int);
int deposit(struct Konto*, int, int);
int transfer(struct Konto*, int, int, int);

int main() {

    //Dateipfad bestimmen
    FILE* accounts = fopen(DATEI, "r+");
    if (accounts == NULL) {
        printf("Fehler beim lesen der Datei.");
        exit(1);
    }

    //VARIABLEN
    struct Konto *tmpAos;
    int whileBedingung = 1;
    int testVariable; //um z.B. auf return 0 zu prüfen
    int kontoNrTemp, kontoNrTemp2;
    int betragTemp; //damit bei erneutem Ausführen von z.B. withdraw nicht eine bereits existierende betrag-Variable intitiiert wird
    int anzahl = anzahlKonten(accounts);
    int *anzahlPtr = &anzahl;
    struct Konto *konten = (struct Konto*) malloc(anzahl*sizeof(struct Konto)); //Gibt Warnung über Memory Leak, hab ich mich aber eigentlich drum gekümmert...

    //Datei accounts.txt auslesen
    auslesen(accounts, konten, anzahl);

    //Eingabeaufforderung: Welche Operation?
    char operation;

    //Immer wieder Eingabe bis zum Programmende
    while (whileBedingung) {
        printf("Operation (? für Hilfe, e für Ende): ");
        scanf(" %c", &operation);

        //was noch fehlt: nach jeder Operation die entsprechenden Konten anzeigen

        //neues Konto anlegen
        if (operation == 'n') {
            tmpAos = realloc(konten, anzahl * sizeof(struct Konto) + sizeof(struct Konto));
            // Platz für weiteren Eintrag
            if (konten == NULL) {
                printf("Fehler bei Speicherzuweisung.");
                free(tmpAos);
                exit(1);
            } else {
                konten = tmpAos;
                free(tmpAos);

                testVariable = newAccount(konten, anzahlPtr);

                if (testVariable == 0)
                    printf("Abheben erfolgreich. Kontunummer: %d", anzahl - 1);
                else
                    printf("Fehler beim Erstellen eines Kontos!");

                testVariable = 1;
                //zurücksetzen, damit bei anderer funktion die if-Bedinung nicht fälschlicherweise zutrifft
            }
        }

        //Hilfe
        else if (operation == '?') {
            printf("Hilfe - Operationen\n");
            printf("n - Neues Konto\n");
            printf("p - Kontodaten drucken\n");
            printf("w - Abheben\n");
            printf("d - Einzahlen\n");
            printf("t - Überweisung\n");
            printf("s - Änderungen in Backup-Datei speichern"); //noch nicht umgesetzt
        }

        //Ende - Programm beenden
        else if (operation == 'e') {
            whileBedingung = 0;
        }

        //Kontodaten drucken
        else if (operation == 'p') {
            printf("KONTO DRUCKEN \nKontonummer: ");
            scanf(" %d", &kontoNrTemp);
            printf("Inhaber: ");
            puts(konten[kontoNrTemp].inhaber);
            printf("Kontostand: %.2f", (float) konten[kontoNrTemp].guthaben / 100);
        }

        //abheben
        else if (operation == 'w') {
            //Eingabeaufforderung
            printf("ABHEBUNG \nKontonummer: ");
            scanf(" %d", &kontoNrTemp);
            clearInput();
            printf("Betrag in ct: ");
            scanf(" %d", &betragTemp);
            clearInput();

            printf("Kontostand vorher: %.2f", (float) konten[kontoNrTemp].guthaben / 100);
            testVariable = withdraw(konten, kontoNrTemp, betragTemp);
            if (testVariable == 0)
                printf("\nAbhebung erfolgreich. Neuer Kontostand: %.2f", (float) konten[kontoNrTemp].guthaben / 100);
            else
                printf("Fehler beim Abheben!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedinung nicht fälschlicherweise zutrifft
        }

        //einzahlen
        else if (operation == 'd') {
            //Eingabeaufforderung
            printf("EINZAHLUNG \nKontonummer: ");
            scanf(" %d", &kontoNrTemp);
            clearInput();
            printf("Betrag in ct: ");
            scanf(" %d", &betragTemp);
            clearInput();

            printf("Kontostand vorher: %.2f", (float) konten[kontoNrTemp].guthaben / 100);
            testVariable = deposit(konten, kontoNrTemp, betragTemp);
            if (testVariable == 0)
                printf("\nEinzahlung erfolgreich. Neuer Kontostand: %.2f", (float) konten[kontoNrTemp].guthaben / 100);
            else
                printf("Fehler beim Einzahlen!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedinung nicht fälschlicherweise zutrifft
        }

        //Überweisung
        else if (operation == 't') {
            //Eingabeaufforderung
            printf("ÜBERWEISUNG \nKontonummer d. Zahlers: ");
            scanf(" %d", &kontoNrTemp);
            clearInput();

            printf("Kontonummer d. Empfängers: ");
            scanf(" %d", &kontoNrTemp2);
            clearInput();

            printf("Betrag in ct: ");
            scanf(" %d", &betragTemp);
            clearInput();

            //Werte eintragen
            printf("Kontostände vorher: Zahler: %.2f, Empfänger: %.2f", (float) konten[kontoNrTemp].guthaben / 100, (float) konten[kontoNrTemp2].guthaben / 100);
            testVariable = transfer(konten, kontoNrTemp, kontoNrTemp2, betragTemp);

            //Überprüfung auf Fehler
            if (testVariable == 0)
                printf("\nÜberweisung erfolgreich. Neue Kontostände: Zahler: %.2f, Empfänger: %.2f", (float) konten[kontoNrTemp].guthaben / 100, (float) konten[kontoNrTemp2].guthaben / 100);
            else
                printf("Fehler beim Überweisen!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedinung nicht fälschlicherweise zutrifft
        }

        //falls etwas anderes eingegeben wird:
        else {
            printf("Eingabe unzulässig.\n");
        }

        printf("\n\n");

    }

    clearInput();

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

int newAccount(struct Konto *aos, int* anzahlPtr) {
    //Anzahl der Konten intern erhöhen
    clearInput();
    int anzahl = *(anzahlPtr) + 1;

    char inhaberNeu[40];
    int startguthaben;
    printf("NEUES KONTO \nInhaber: ");
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

    return 0;
}

int withdraw(struct Konto *aos,int kontoNr, int betrag) {
    (aos + kontoNr)->guthaben -= betrag;

    return 0; //lasse ich noch offen, vielleicht return neues guthaben
} // Ohne Abfrage in der Funktion, um es mit transfer() kompatibel zu machen

int deposit(struct Konto *aos,int kontoNr, int betrag) {
    (aos + kontoNr)->guthaben += betrag;

    return 0;
}

int transfer(struct Konto *aos, int zahlerNr, int empfangerNr, int betrag) {
    withdraw(aos, zahlerNr, betrag);
    deposit(aos, empfangerNr, betrag);

    return 0;
}
