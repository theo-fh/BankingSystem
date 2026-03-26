#include <math.h>
#include <stdio.h>
#define DATEI ("../accounts")

struct Konto {
    int guthaben; //in ct
    char inhaber[40];
} konten[];

int auslesen() {
    FILE* accounts = fopen(DATEI, "r");

    //Menge der char in der File lesen (bis EOF)

    //daraus Anzahl der Konten (--> feste Zeilenlänge)

    //konten = malloc(anzahl*sizeof(struct Konto)) für das AOS

    //Alle Daten in konten[i].guthaben und -.inhaber schreiben

    //evtl. return Anzahl der Konten

    fclose(accounts);
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

int newAccount(struct Konto *aos, char inhaber[40], int startguthaben) {

    //irgendwoher die Anzahl aller Konten vor dieser Funktion herausfinden

    //realloc(aos, anzahl*sizeof(struct Konto) + sizeof(struct Konto)) --> Platz für weiteren eintrag

    //letzter (neuer) Eintrag: aos[letzter].inhaber = inhaber[40]; -.guthaben = startguthaben

}

int withdraw() {

}

int deposit() {

}

int transfer() {

} //Kombination aus withdraw und deposit

int main() {

    char operation;
    printf("Operation: ");
    scanf(" %c", &operation);

    if (operation == 'w')
        withdraw();


    return 0;
}