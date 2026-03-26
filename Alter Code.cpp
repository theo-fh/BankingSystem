//
// Created by theo on 26.03.26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addAccount(FILE* fptr, double bal, char own[30]) {
    int id = 0;

    int c;
    while ((c=fgetc(fptr)) != EOF) {
        if (c=='\n')
            id++;
    } //zählt Enters in der account File, entspricht Anzahl der Momentanen Konten -1
    id++; //jetzt Anzahl der Momentanen Konten
    id++; //jetzt Zahl (id) des neuen Kontos

    char balString[12];
    sprintf(balString, "%.2lf", bal);

    for (int i=0; i<12; i++) {
        if (balString[i]==0)
            balString[i] = ' ';
    }

    fprintf(fptr, "\nid: %d, owner: %s, balance: %s", id, own, balString); //neue Zeile mit Werten drucken

    return id; //ausgabe ist nummer des neuen Kontos
}

void removeNewline(char *input) {
    input[strcspn(input,"\n")] = 0;
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

long int readAccount(FILE* accounts, int id, char *ziel) {
    long int n = 0;
    int zeilen = 0;
    char c;
    while (1) {
        c = fgetc(accounts);
        n++;
        if (c==EOF) {
            printf("Diese ID gibt es nicht.");
            return -1;
        }

        if (c=='\n')
            zeilen++;

        if (zeilen == id-1) {
           break;
        }

    }
    fseek(accounts, n, SEEK_SET);

    char s;
    int i = 0;
    while ((s = fgetc(accounts)) != '\n' && s != EOF) {
        *(ziel + i) = s;
        i++;
    }
    fseek(accounts, 0, SEEK_SET);

    return n;
}

int withdraw(FILE* accounts, int id, double menge) {
    printf("0  ");
    char info[100];

    long int pos = readAccount(accounts, id, info);
    // [strcspn(info, "balance: ")+9] = ...;
    printf("1  ");
    char balString[12];
    for (int i = strstr(info, "balance: ") + 9 - info; info[i] != ' '; i++) {
        balString[i-strcspn(info, "balance: ")+9] = info[i];
    }

    printf("2  ");
    while (info[strlen(balString)-1] == 0) {
        info[strlen(info)-1]='\0';
    }
    printf("3  ");
    puts(balString);

    while (info[strlen(info)-1] != ':') {
        info[strlen(info)-1]='\0';
    } //gesamten eintrag nach "balance:" löschen

}

int deposit(FILE* accounts, int id, double menge) {

}

int main(void) {

    char befehl = 't';
    char owner[30];
    double balance;
    int id;

    if (befehl == 't') {
        int zahlerId, empfangId;
        double betrag;
        // char zahlerInfo[100], empfangInfo[100];

        //Abfrage
        printf("ÜBERWEISUNG \nZahler-ID: ");
        scanf(" %d", &zahlerId);
        clearInput();

        printf("Empfänger-ID: ");
        scanf(" %d", &empfangId);
        clearInput();

        printf("Betrag in €: ");
        scanf(" %lf", &betrag);
        clearInput();

        printf("A\n");
        FILE* accounts = fopen("../accounts", "r+");
        printf("B\n");
        if (!accounts) {
            perror("fopen fehlgeschlagen");
            return 1;
        }

        printf("C\n");

        withdraw(accounts, zahlerId, betrag); //hier kommt nichts raus...
        printf("D\n");
        deposit(accounts, empfangId, betrag);
        /*
        long int zahlerPos, empfangPos;
        zahlerPos = readAccount(accounts, zahlerId, zahlerInfo);
        empfangPos = readAccount(accounts, empfangId, empfangInfo);
        //lieber einzelne deposit() und withdraw() funktionen
        */


        fclose(accounts);

    } //Überweisung

    if (befehl == 'r') {
        FILE* accounts = fopen("../accounts", "r");

        printf("KONTO LESEN\nID eingeben: ");
        scanf(" %d", &id);
        clearInput();
        char ausgabe[100];
        readAccount(accounts, id, ausgabe);

        puts(ausgabe);

        fclose(accounts);
    } //Konto auslesen

    if (befehl == 'n') {
        printf("NEUES KONTO \nBesitzer: ");
        fgets(owner, 30, stdin);
        removeNewline(owner);
        printf("Startguthaben: ");
        scanf(" %lf", &balance);
        clearInput();

        FILE* accounts = fopen("../accounts", "a+");
        id = addAccount(accounts, balance, owner);
        printf("ID of new account: %d\n", id);
        fclose(accounts);
    } //neues Konto

    return 0;
}