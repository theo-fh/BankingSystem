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

//Funktionen
void clearInput();                              //löscht input, falls user mehr als ein char + \n eingegeben hat
int anzahlKonten(FILE*);                        //rechnet Anzahl der Konten aus Anzahl der Zeichen in einer datei, Anzahl ist wichtige Variable für fast alle funktionen
int auslesen(char*, struct Konto**, int*);      //liest von Datei mit gegebenem Pfad die Kontodaten aller Einträge in ein Array
int writeToFile(char*, struct Konto*, int);     //schreibt die (ggf. geänderten) Daten wieder in eine Datei mit gegebenem Pfad ein
int newAccount(struct Konto **, int*);          //fügt neues Konto hinzu, nachdem es das Array um einen Eintrag erweitert hat
int withdraw(struct Konto *, int);              //Abhebung mit Abfrage von Kontonr. und Betrag
int deposit(struct Konto *, int);               //Einzahlung, analog zu Abhebung
int transfer(struct Konto *, int);              //Überweisung mit Abfrage, quasi Kombination aus Abhebung und Einzahlung
void hilfe();                                   //druckt alle möglichen Operationen
int drucken(struct Konto*, int);                //druckt die Daten eines Kontos aus (Inhaber, Guthaben)
int chooseBackup(char *);                       //Abfrage, welches Backup geladen werden soll, schreibt in einen String, der dann auslesen() überreicht wird

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
