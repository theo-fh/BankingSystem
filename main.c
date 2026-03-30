#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banking_funktionen.h"
#define DATEI ("../data/accounts")
#define BAUTO ("../backups/backup-auto")
#define BUSER ("../backups/backup-user")
#define BNEW ("../backups/backup-new")
#define INTMAX (2000000000) //Guthaben auf 20 Mio. Euro begrenzt

void clearInput();
int anzahlKonten(FILE*);
void auslesen(FILE*, struct Konto*, int);
int writeToFile(FILE*, struct Konto*, int);
int newAccount(struct Konto *, int*);
int withdraw(struct Konto *, int, int);
int deposit(struct Konto *,int, int);
int transfer(struct Konto *, int, int, int);

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
    long int betragLongTemp;
    int anzahl = anzahlKonten(accounts);
    int *anzahlPtr = &anzahl;
    struct Konto *konten = (struct Konto*) malloc(anzahl*sizeof(struct Konto)); //Gibt Warnung über Memory Leak, hab ich mich aber eigentlich drum gekümmert...

    //Datei accounts.txt auslesen
    auslesen(accounts, konten, anzahl);

    fclose(accounts);

    //Automatisches Backup erstellen
    FILE* backupAuto = fopen(BAUTO, "w");
    if (backupAuto == NULL) {
        printf("Datei für automatische Backups konnte nicht erstellt werden.");
    }
    else {
        writeToFile(backupAuto, konten, anzahl);
        printf("Backup erfolgreich erstellt. Pfad: %s\n", BAUTO);
    }
    fclose(backupAuto);

    //Eingabeaufforderung: Welche Operation (opLoad für Operation Backup laden)?
    char operation, opLoad;

    //Immer wieder Eingabe bis zum Programmende
    while (whileBedingung) {
        printf("Operation (? für Hilfe, e für Ende): ");
        scanf(" %c", &operation);

        //Anmerkung: Ein switch statement wäre hier evtl. schöner, mir reicht aber eine else if-Leiter aus.
        //neues Konto anlegen
        if (operation == 'n') {
            FILE* backupNew = fopen(BNEW, "w");
            if (backupNew == NULL) {
                printf("Datei für Backup konnte nicht erstellt werden.");
            }
            else {
                writeToFile(backupNew, konten, anzahl);
            }
            fclose(backupNew);
            tmpAos = (struct Konto*)realloc(konten, anzahl * sizeof(struct Konto) + sizeof(struct Konto));
            // Platz für weiteren Eintrag
            if (tmpAos == NULL) {
                printf("Fehler bei Speicherzuweisung. Empfehlung: backup-new laden.");
                free(tmpAos);
                exit(1);
            }

            konten = tmpAos;

            testVariable = newAccount(konten, anzahlPtr);

            if (testVariable == 0)
                printf("Abheben erfolgreich. Kontunummer: %d", anzahl - 1);
            else
                printf("Fehler beim Erstellen eines Kontos!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedinung nicht fälschlicherweise zutrifft

        }

        //Hilfe
        else if (operation == '?') {
            printf("Hilfe - Operationen\n");
            printf("n - Neues Konto\n");
            printf("p - Kontodaten drucken\n");
            printf("w - Abheben\n");
            printf("d - Einzahlen\n");
            printf("t - Überweisung\n");
            printf("s - Änderungen in Backup-Datei speichern\n");
            printf("l - Backup laden");
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
            printf("Kontostand: %.2f €", (float) konten[kontoNrTemp].guthaben / 100);
        }


        //abheben
        else if (operation == 'w') {
            //Eingabeaufforderung
            printf("ABHEBUNG \nKontonummer: ");
            scanf(" %d", &kontoNrTemp);
            clearInput();
            //Kontoinhaber anzeigen
            printf("Inhaber: %s\n", konten[kontoNrTemp].inhaber);

            printf("Betrag in ct: ");
            scanf(" %d", &betragTemp);
            clearInput();

            printf("Kontostand vorher: %.2f €", (float) konten[kontoNrTemp].guthaben / 100);
            testVariable = withdraw(konten, kontoNrTemp, betragTemp);
            if (testVariable == 0)
                printf("\nAbhebung erfolgreich. Neuer Kontostand: %.2f €", (float) konten[kontoNrTemp].guthaben / 100);
            else
                printf("Fehler beim Abheben!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedingung nicht fälschlicherweise zutrifft
        }

        //TODO: maximales Guthaben wirklich begrenzen
        //einzahlen
        else if (operation == 'd') {
            //Eingabeaufforderung
            printf("EINZAHLUNG \nKontonummer: ");
            scanf(" %d", &kontoNrTemp);
            clearInput();
            //Kontoinhaber anzeigen
            printf("Inhaber: %s\n", konten[kontoNrTemp].inhaber);

            printf("Betrag in ct: ");
            scanf(" %ld", &betragLongTemp);
            clearInput();

            if ((long int)betragLongTemp + konten[kontoNrTemp].guthaben > INTMAX) {
                printf("Guthaben nicht zugeschrieben. Zu hohes Guthaben. Herzlichen Glückwunsch.\n");
                continue;
            }

            betragTemp = (int) betragLongTemp;

            printf("Kontostand vorher: %.2f €", (float) konten[kontoNrTemp].guthaben / 100);
            testVariable = deposit(konten, kontoNrTemp, betragTemp);
            if (testVariable == 0)
                printf("\nEinzahlung erfolgreich. Neuer Kontostand: %.2f €", (float) konten[kontoNrTemp].guthaben / 100);
            else
                printf("Fehler beim Einzahlen!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedingung nicht fälschlicherweise zutrifft
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
            printf("Zahler: %s, Empfänger: %s\n", konten[kontoNrTemp].inhaber, konten[kontoNrTemp2].inhaber);

            printf("Betrag in ct: ");
            scanf(" %d", &betragTemp);
            clearInput();

            if ((long int)betragLongTemp + konten[kontoNrTemp2].guthaben > INTMAX) {
                printf("Guthaben nicht zugeschrieben. Zu hohes Guthaben. Herzlichen Glückwunsch.\n");
                continue;
            }

            betragTemp = (int) betragLongTemp;

            //Werte berechnen
            printf("Kontostände vorher: Zahler: %.2f €, Empfänger: %.2f €", (float) konten[kontoNrTemp].guthaben / 100, (float) konten[kontoNrTemp2].guthaben / 100);
            testVariable = transfer(konten, kontoNrTemp, kontoNrTemp2, betragTemp);

            //Überprüfung auf Fehler
            if (testVariable == 0)
                printf("\nÜberweisung erfolgreich. Neue Kontostände: Zahler: %.2f €, Empfänger: %.2f €", (float) konten[kontoNrTemp].guthaben / 100, (float) konten[kontoNrTemp2].guthaben / 100);
            else
                printf("Fehler beim Überweisen!");

            testVariable = 1;
            //zurücksetzen, damit bei anderer funktion die if-Bedinung nicht fälschlicherweise zutrifft
        }

        //Backup erstellen
        else if (operation == 's') {
            FILE* backupUser = fopen(BUSER, "w");
            if (backupUser == NULL) {
                printf("Datei für Backup konnte nicht erstellt werden.");
            }
            else {
                writeToFile(backupUser, konten, anzahl);
                printf("Backup erfolgreich erstellt. Pfad: %s\n", BUSER);
            }
            fclose(backupUser);

        }

        else if (operation == 'l') {
            printf("BACKUP LADEN - Optionen: \n");
            printf("a - backup-auto (bei Programmstart erstellt)\n");
            printf("u - backup-user (durch Operation 's' erstellt)\n");
            printf("n - backup-new (durch Operation 'n' erstellt)\n");
            printf("Welches Backup? ");
            scanf(" %c", &opLoad);

            int testLoad = 0;
            FILE* fptr;
            switch (opLoad) {
                case 'a':
                    fptr = fopen(BAUTO, "r");
                    auslesen(fptr, konten, anzahlKonten(fptr));
                    break;
                case 'u':
                    fptr = fopen(BUSER, "r");
                    auslesen(fptr, konten, anzahlKonten(fptr));
                    break;
                case 'n':
                    fptr = fopen(BNEW, "r");
                    auslesen(fptr, konten, anzahlKonten(fptr));
                    break;
                default:
                    printf("Eingabe unzulässig.");
                    testLoad = 1;
            }
            if (testLoad == 0) {
                printf("Backup geladen. Neustart wird empfohlen.");
            }

        }

        //falls etwas anderes eingegeben wird:
        else {
            printf("Eingabe unzulässig.\n");
        }

        printf("\n\n");


    }

    clearInput();

    printf("Speichert auf Datei...\n");
    accounts = fopen(DATEI, "w");
    writeToFile(accounts, konten, anzahl);

    fclose(accounts);

    free(konten);

    return 0;
}