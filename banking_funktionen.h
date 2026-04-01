//
// Created by theo on 30.03.26.
//

#ifndef ERSTES_PROJEKT_BANKING_FUNKTIONEN_H
#define ERSTES_PROJEKT_BANKING_FUNKTIONEN_H

#define DATEI "../data/accounts"
#define BAUTO "../backups/backup-auto"
#define BUSER "../backups/backup-user"
#define BNEW "../backups/backup-new"
#include <limits.h>

struct Konto {
    int guthaben; //in ct
    char inhaber[40];
};

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
int eingabeInt(int*);                           //ersetzt scanf, nur für positive zahlen

inline void clearInput() {
    /*
    Leert den Input-Buffer.
    Kann dazu führen, dass Enter ein zweites Mal gedrückt werden muss, um den nächsten Output zu erreichen.
    */

    int c;
    while ((c = getchar()) != EOF && c != '\n'){}
}

inline int anzahlKonten(FILE* datei) {
    /*
     Zählt die Länge der gegebenen Datei und berechnet daraus die Anzahl an Zeilen bei fester zeilenlänge inkl. newline.
     Funktionalität beruht auf Tabellenform der Datei.

     datei -
     */
    //Menge der char in der File lesen (bis EOF)
    int c;
    int i = 0;
    while ((c = fgetc(datei)) != EOF) {
        i++;
    }

    //daraus Anzahl der Konten (→ feste Zeilenlänge)
    int zeilenlänge = 62;
    int anzahlKonten = i/zeilenlänge;

    return anzahlKonten;
}

inline int auslesen(char pfad[25], struct Konto **aos,int* anzahlPtr) {
    /*
    Liest Einträge aus Datei (Tabellenform) und gibt sie in das geg. Array aus

    pfad[25] - dateipfad, aus dem gelesen wird (lässt nur Platz für relative Pfade)
    **aos - Pointer zum Array (zu Pointer)
    *anzahlPtr - Pointer zur Anzahl der Konten, wird für andere Funktionen bestimmt
     */

    FILE* datei = fopen(pfad, "r");
    if (datei == NULL) {
        printf("Fehler beim lesen der Datei.");
        return 1;
    }
    *anzahlPtr = anzahlKonten(datei);
    *aos = (struct Konto*) malloc(*anzahlPtr*sizeof(struct Konto));
    int zeilenlaenge = 62;
    fseek(datei, 0, SEEK_SET);
    for (int kontoNr = 0; kontoNr < *anzahlPtr; kontoNr++) {
        fseek(datei, zeilenlaenge * kontoNr, SEEK_SET);
        char guthabenStr[30] = {0};
        char inhaberStr[40] = {0};
        int n = 0;
        int c;
        while ((c = fgetc(datei)) != ' ') {
            guthabenStr[n] = (char)c;
            n++;
        } //Guthaben des Kontos lesen
        guthabenStr[n] = '\0'; //String kürzen, damit nur "Integer" und '\0' enthalten sind

        int guthaben = 0;
        char* endptr;
        long int guthabenLong = strtol(guthabenStr, &endptr, 10);
        if (guthabenLong > INT_MAX) {
            printf("Fehler beim Auslesen. Kontonummer: %d", kontoNr);
            return 1;
        }
        guthaben = (int)guthabenLong;
        (*(aos) + kontoNr)->guthaben = guthaben;

        fseek(datei, (long int) zeilenlaenge * kontoNr + 20, SEEK_SET);
        n = 0;
        while ((c = fgetc(datei)) != '/') {
            inhaberStr[n] = (char)c;
            n++;
        } //Inhaber des Kontos lesen

        for (int i = 39; i >= 0; i--) {
            if (inhaberStr[i] != ' ') {
                inhaberStr[i+1] = '\0';
                break;
            }
        }

        strcpy((*(aos) + kontoNr)->inhaber, inhaberStr);
    }
    fclose(datei);
    return 0;
}

inline int writeToFile(char pfad[25], struct Konto* aos, int anzahl) {
    /*
    Quasi das Gegenteil von auslesen()
    Schreibt Kontodaten eines Kontos in einen String, der dann in einer neuen Zeile an die Datei angefügt wird

    pfad[25] - Pfad zur Datei, in der die Daten stehen
    *aos - Array mit Daten
    anzahl - Anzahl von Konten, sagt der Funktion, wan sie aufhören soll zu schreiben, um keine Müllwerte einzuschreiben
     */
    FILE* datei = fopen(pfad, "w");
    if (datei == NULL) {
        printf("Datei konnte nicht erstellt werden.");
        return 1;
    }

    for (int kontoNr = 0; kontoNr<anzahl; kontoNr++) {
        //String mit Kontodaten neu initiieren
        char info[63] = {0};
        int stellen;

        //guthaben drucken
        if (aos[kontoNr].guthaben > 0)
            stellen = (int) log10(aos[kontoNr].guthaben) + 1;
        else if (aos[kontoNr].guthaben == 0)
            stellen = 1;
        else
            stellen = (int) log10(-1 * aos[kontoNr].guthaben) + 2;

        sprintf(info, "%d", aos[kontoNr].guthaben);

        //Rest des Platzes mit Leerzeichen füllen
        for (int i=stellen; i < 20; i++) {
            info[i] = ' ';
        }

        //Inhaber drucken
        strcat(info, aos[kontoNr].inhaber);


        //Rest mit Leerzeichen füllen
        int stringLaenge = (int)strlen(info); //länge des Strings ohne '\0'

        // info[20 + stringLaenge] = '\0';
        for (int i = stringLaenge; i < 60; i++) {
            info[i] = ' ';
        }

        //Das hier war die Lösung:
        info[60] = '\0';
        //Ende mit "/\n" besetzen
        strcat(info, "/\n");

        fprintf(datei, "%s", info);
    }
    printf("Kontodaten gesichert. Pfad: %s\n", pfad);
    fclose(datei);
    return 0;
}

inline int newAccount(struct Konto **aos, int* anzahlPtr) {
    /*
    Fügt neues Konto hinzu, inkl. Abfrage für Inhaber und Startguthaben
    Druckt dann die neue Kontonummer.

    **aos - Array mit Daten. Pointer zum Pointer, um realloc verwenden zu können
    *anzahlPtr - um die Anzahl der Konten in main um 1 zu erhöhen
     */
    writeToFile(BNEW, *aos, *anzahlPtr);
    struct Konto *tmpAos = (struct Konto*)realloc(*aos, *anzahlPtr * sizeof(struct Konto) + sizeof(struct Konto));
    // Platz für weiteren Eintrag
    if (tmpAos == NULL) {
        printf("Fehler bei Speicherzuweisung. Empfehlung: Backup laden.");
        free(tmpAos);
        exit(1);
    }

    *aos = tmpAos;

    //Anzahl der Konten intern erhöhen
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
    if (eingabeInt(&startguthaben) != 0) {
        return 1;
    }

    //Angaben in AOS übernehmen
    strcpy((*aos + anzahl - 1)->inhaber, inhaberNeu);
    (*aos + anzahl - 1)->guthaben = startguthaben;

    //Anzahl d. Konten global erhöhen
    *anzahlPtr += 1;

    printf("Konto angelegt. Kontonummer: %d", *anzahlPtr-1);

    return 0;
}

inline int withdraw(struct Konto *konten,int anzahl) {
    /*
    Abheben - nimmt Kontonummer und Betrag entgegen und schreibt Änderung des Guthabens in das Array

    *konten - Array mit Daten
    anzahl - Anzahl der Konten im Array
     */

    int kontoNrTemp, betragTemp;

    //Eingabeaufforderung
    printf("ABHEBUNG \nKontonummer: ");
    if (eingabeInt(&kontoNrTemp) != 0) {
        return 1;
    }

    //Fehlerbehandlung von der Kontonummer aus
    if (kontoNrTemp > (anzahl -1)) {
        printf("Kein Konto zu dieser Kontonummer.");
        return 1;
    }
    //Kontoinhaber anzeigen
    printf("Inhaber: %s\n", konten[kontoNrTemp].inhaber);

    printf("Betrag in ct: ");

    if (eingabeInt(&betragTemp) != 0) {
        return 1;
    }


    printf("Kontostand vorher: %.2f €\n", (float) konten[kontoNrTemp].guthaben / 100);

    (konten + kontoNrTemp)->guthaben -= betragTemp;

    if ((konten + kontoNrTemp)->guthaben < 0) {
        printf("\nACHTUNG: Guthaben ausgenutzt. Kontostand negativ.\n");
    }

    printf("Kontostand nachher: %.2f €\n", (float) konten[kontoNrTemp].guthaben / 100);

    return 0;
}

inline int deposit(struct Konto *konten,int anzahl) {
    /*
    Einzahlen - nimmt Kontonummer und Betrag entgegen und schreibt Änderung des Guthabens in Array

    *konten - Array mit Kontodaten
    anzahl - Anzahl der Konten im Array
     */
    int kontoNrTemp, betragTemp;
    printf("EINZAHLUNG \nKontonummer: ");
    if (eingabeInt(&kontoNrTemp) != 0) {
        return 1;
    }
    if (kontoNrTemp > (anzahl -1)) {
        printf("Kein Konto zu dieser Kontonummer.");
        return 1;
    }
    //Kontoinhaber anzeigen
    printf("Inhaber: %s\n", konten[kontoNrTemp].inhaber);

    printf("Betrag in ct: ");
    if (eingabeInt(&betragTemp) != 0) {
        return 1;
    }

    if ((long int)betragTemp + konten[kontoNrTemp].guthaben > INT_MAX) {
        printf("Guthaben nicht zugeschrieben. Zu hohes Guthaben. Herzlichen Glückwunsch.\n");
        return 1;
    }

    printf("Kontostand vorher: %.2f €\n", (float) konten[kontoNrTemp].guthaben / 100);
    (konten + kontoNrTemp)->guthaben += betragTemp;

    printf("Kontostand nachher: %.2f €", (float) konten[kontoNrTemp].guthaben / 100);

    return 0;
}

inline int transfer(struct Konto *konten, int anzahl) {
    /*
    Überweisung - nimmt Kontonummer von Zahler u. Empfänger sowie den Betrag entgegen, schreibt Änderungen der Guthaben in Array

    *konten - Array mit Kontodaten
    anzahl - Anzahl der Kontos im Array
     */
    int kontoNrTemp, kontoNrTemp2, betragTemp;

    //Eingabeaufforderung
    printf("ÜBERWEISUNG \nKontonummer d. Zahlers: ");
    if (eingabeInt(&kontoNrTemp) != 0) {
        return 1;
    }

    if (kontoNrTemp > (anzahl -1)) {
        printf("Kein Konto zu dieser Kontonummer.");
        return 1;
    }

    printf("Kontonummer d. Empfängers: ");
    if (eingabeInt(&kontoNrTemp2) != 0) {
        return 1;
    }
    if (kontoNrTemp2 > (anzahl -1)) {
        printf("Kein Konto zu dieser Kontonummer.");
        return 1;
    }

    printf("Zahler: %s, Empfänger: %s\n", konten[kontoNrTemp].inhaber, konten[kontoNrTemp2].inhaber);

    printf("Betrag in ct: ");
    if (eingabeInt(&betragTemp) != 0) {
        return 1;
    }
    if ((long int)betragTemp + konten[kontoNrTemp2].guthaben > INT_MAX) {
        printf("Guthaben nicht zugeschrieben. Zu hohes Guthaben. Herzlichen Glückwunsch.\n");
        return 1;
    }

    printf("Kontostände vorher: Zahler: %.2f €, Empfänger: %.2f €", (float) konten[kontoNrTemp].guthaben / 100, (float) konten[kontoNrTemp2].guthaben / 100);
    //Betrag von den Konten abrechnen
    konten[kontoNrTemp].guthaben -= betragTemp;
    konten[kontoNrTemp2].guthaben += betragTemp;

    printf("\nKontostände nachher: Zahler: %.2f €, Empfänger: %.2f €", (float) konten[kontoNrTemp].guthaben / 100, (float) konten[kontoNrTemp2].guthaben / 100);


    return 0;
}

inline void hilfe() {
    /*
    Druckt alle Befehle, die der Benutzer ausführen kann → alles andere gibt Error aus
     */
    printf("Hilfe - Operationen\n");
    printf("n - Neues Konto\n");
    printf("p - Kontodaten drucken\n");
    printf("w - Abheben\n");
    printf("d - Einzahlen\n");
    printf("t - Überweisung\n");
    printf("s - Änderungen in Backup-Datei speichern\n");
    printf("l - Backup laden\n");
    printf("e - Speichern und beenden");
}

inline int drucken(struct Konto* konten, int anzahl) {
    /*
    Druckt Inhaber und Guthaben eines Kontos aus

    *konten - Array mit Kontodaten
    anzahl - Anzahl der Konten im Array
     */
    int kontoNr;
    printf("KONTO DRUCKEN \nKontonummer: ");
    if (eingabeInt(&kontoNr) != 0) {
        return 1;
    }
    if (kontoNr > (anzahl -1)) {
        printf("Kein Konto zu dieser Kontonummer.");
        return 1;
    }
    printf("Inhaber: ");
    puts(konten[kontoNr].inhaber);
    printf("Kontostand: %.2f €", (float) konten[kontoNr].guthaben / 100);


    return 0;
}

inline int chooseBackup(char *pfadPtr) {
    /*
    Interaktive Auswahl, welches Backup geladen werden soll. Für Verwendung mit writeToFile() vorgesehen.

    *pfadPtr - Pointer zum pfad-String (muss genügend Platz haben → das wird schon im main() gewährleistet)
     */
    char opLoad;

    printf("BACKUP LADEN - Optionen: \n");
    printf("a - backup-auto (bei Programmstart erstellt)\n");
    printf("u - backup-user (durch Operation 's' erstellt)\n");
    printf("n - backup-new (durch Operation 'n' erstellt)\n");
    printf("Welches Backup? ");
    scanf(" %c", &opLoad);

    switch (opLoad) {
        case 'a':
            sprintf(pfadPtr, BAUTO);
            break;
        case 'u':
            sprintf(pfadPtr, BUSER);
            break;
        case 'n':
            sprintf(pfadPtr, BNEW);
            break;
        default:
            printf("Eingabe unzulässig.");
            return 1;
    }

    return 0;
}

inline int eingabeInt(int* eingelesenerInt) {
    /*
    Nimmt Pointer zu einer Ganzzahl entgegen, macht Eingabeaufforderung
    und schreibt in den Wert des Pointers die eingegebene Ganzzahl.

    Überprüft vorher, ob der Wert in einen Integer passt und ob der String auch nur aus einer Nummer besteht
     */
    char string[11]; //10 Stellen, alles Höhere: größer als INT_MAX
    char* endptr;
    fgets(string, 11, stdin);
    string[strcspn(string, "\n")] = '\0';
    long int parsed = strtol(string, &endptr, 10);
    //'entptr == string' braucht man evtl. gar nicht
    if (parsed > INT_MAX || parsed < 0 || endptr == string || *endptr != '\0') {
        printf("Eingabe unzulässig.");
        return 1;
    }

    *eingelesenerInt = (int) parsed;

    return 0;
}

#endif //ERSTES_PROJEKT_BANKING_FUNKTIONEN_H