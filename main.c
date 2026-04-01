#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banking_funktionen.h"


int main() {

    //Datei accounts.txt auslesen
    int anzahl;
    struct Konto *konten = (struct Konto*)malloc(sizeof(struct Konto));
    auslesen(DATEI, &konten, &anzahl);

    //Automatisches Backup erstellen
    writeToFile(BAUTO, konten, anzahl);

    //Eingabeaufforderung: Welche Operation (opLoad für Operation Backup laden)?
    char operation;

    //Variablen:
    char pfadLoad[25]; //kann nicht direkt nach "case 'l':" im switch statement stehen
    int whileBedingung = 1;

    //Immer wieder Eingabe bis zum Programmende
    while (whileBedingung) {
        printf("Operation (? für Hilfe): ");
        scanf(" %c", &operation);
        clearInput();
        switch (operation) {
            case '?':
                hilfe();
                break;
            case 'n':
                newAccount(&konten, &anzahl);
                break;
            case 'e':
                whileBedingung = 0;
                break;
            case 'p':
                drucken(konten, anzahl);
                break;
            case 'w':
                withdraw(konten, anzahl);
                break;
            case 'd':
                deposit(konten, anzahl);
                break;
            case 't':
                transfer(konten, anzahl);
                break;
            case 's':
                writeToFile(BUSER, konten, anzahl);
                break;
            case 'l':
                if (chooseBackup(pfadLoad) == 0) {
                    if (auslesen(pfadLoad, &konten, &anzahl) == 0) {
                        printf("Backup erfolgreich geladen. Ein Neustart wird empfohlen.");
                    }
                }
                break;
            default:
                printf("Eingabe unzulässig.");
        }

        printf("\n\n");
    }

    writeToFile(DATEI, konten, anzahl);

    free(konten);

    return 0;
}
