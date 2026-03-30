//
// Created by theo on 30.03.26.
//
#define DATEI ("../data/accounts")
#define BAUTO ("../backups/backup-auto")
#define BUSER ("../backups/backup-user")
#define BNEW ("../backups/backup-new")
#define INTMAX (2000000000)
#ifndef ERSTES_PROJEKT_BANKING_FUNKTIONEN_H
#define ERSTES_PROJEKT_BANKING_FUNKTIONEN_H

struct Konto {
    int guthaben; //in ct
    char inhaber[40];
};

inline void clearInput() {
    int c;
    while ((c = getchar()) != EOF && c != '\n') {}
}

inline int anzahlKonten(FILE* datei) {

    //Menge der char in der File lesen (bis EOF)
    char c;
    int i = 0;
    while ((c = fgetc(datei)) != EOF) {
        i++;
    }

    //daraus Anzahl der Konten (--> feste Zeilenlänge)
    int zeilenlänge = 62;
    int anzahlKonten = i/zeilenlänge;

    return anzahlKonten;
}

inline void auslesen(FILE* datei, struct Konto *aos, int anzahl) {
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
        if (guthabenStr[0] == '-') {
            for (int i = stellen; i >= 1; i--) {
                guthaben += ((int) guthabenStr[i] - 48)*pow(10, stellen - i);
            }
            guthaben *= -1;
        }
        else {
            for (int i = stellen; i >= 0; i--) {
                guthaben += ((int) guthabenStr[i] - 48)*pow(10, stellen - i);
            }
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

inline int writeToFile(FILE* fptr, struct Konto* aos, int anzahl) {

    for (int kontoNr = 0; kontoNr<anzahl; kontoNr++) {
        //String mit Kontodaten neu initiieren
        char info[63] = {0};
        int stellen;

        //guthaben drucken
        if (aos[kontoNr].guthaben > 0) {
            stellen = (int) log10(aos[kontoNr].guthaben) + 1;
        }
        else {
            stellen = (int) log10(-1 * aos[kontoNr].guthaben) + 2;
        }
        sprintf(info, "%d", aos[kontoNr].guthaben);

        //Rest des Platzes mit Leerzeichen füllen
        for (int i=stellen; i < 20; i++) {
            info[i] = ' ';
        }

        //Inhaber drucken
        strcat(info, aos[kontoNr].inhaber);


        //Rest mit Leerzeichen füllen
        int stringLaenge = strlen(info); //länge des Strings ohne '\0'

        // info[20 + stringLaenge] = '\0';
        for (int i = stringLaenge; i < 60; i++) {
            info[i] = ' ';
        }

        //Das hier war die Lösung:
        info[60] = '\0';
        //Ende mit "/\n" besetzen
        strcat(info, "/\n");

        fprintf(fptr, "%s", info);
    }

    return 0;
}

inline int newAccount(struct Konto *aos, int* anzahlPtr) {
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

    //Guthaben begrenzen auf 2 Mia. Cent (20 Mio. Euro) --> bis dahin gehen Integer ungefähr
    if ( startguthaben > INTMAX) {
        printf("Kontostand zu groß, um in Integer zu passen. Herzlichen Glückwunsch.\n");
        return 1;
    }

    //Angaben in AOS übernehmen
    strcpy((aos + anzahl - 1)->inhaber, inhaberNeu);
    (aos + anzahl - 1)->guthaben = startguthaben;

    //Anzahl d. Konten global erhöhen
    *anzahlPtr += 1;

    return 0;
}

inline int withdraw(struct Konto *aos,int kontoNr, int betrag) {
    (aos + kontoNr)->guthaben -= betrag;

    if ((aos + kontoNr)->guthaben < 0) {
        printf("\nACHTUNG: Guthaben ausgenutzt. Kontostand negativ.\n");
    }
    return 0; //lasse ich noch offen, vielleicht return neues guthaben
} // Ohne Abfrage in der Funktion, um es mit transfer() kompatibel zu machen

inline int deposit(struct Konto *aos,int kontoNr, int betrag) {
    if ( (aos + kontoNr)->guthaben + betrag > INTMAX) {
        printf("Guthaben nicht zugeschrieben. Zu hohes Guthaben. Herzlichen Glückwunsch.\n");
        return 1;
    }
    (aos + kontoNr)->guthaben += betrag;

    return 0;
}

inline int transfer(struct Konto *aos, int zahlerNr, int empfangerNr, int betrag) {
    withdraw(aos, zahlerNr, betrag);
    deposit(aos, empfangerNr, betrag);

    return 0;
}


#endif //ERSTES_PROJEKT_BANKING_FUNKTIONEN_H