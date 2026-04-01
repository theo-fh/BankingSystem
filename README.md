# BankingSystem

## Inhaltsverzeichnis
1. [Beschreibung](#beschreibung)
2. [Hinweise](#hinweise)
3. [Features](#features)
   - [Interaktion mit der Datenbank](#interaktion-mit-der-datenbank)
   - [Kontoinformationen](#kontoinformationen)
   - [Kontoverwaltung](#kontoverwaltung)
   - [Dateien](#dateien)
4. [Installation](#installation)
5. [Nutzung](#nutzung)
6. [Funktionen und Variablen](#funktionen-und-variablen)
   - [Grundlegende Variablen](#grundlegende-variablen)
   - [Funktionen der Features](#funktionen-der-features)
   - [Weitere Funktionen](#weitere-funktionen)

## Beschreibung

BankingSystem ist eine terminalbasierte Anwendung zur einfachen Verwaltung von Bankkonten.
Nutzer können eine Datenbank bestehend aus Kontodaten mit beliebig großem Umfang erstellen und verwalten.

## Hinweise
Bitte lesen sie sich folgende Hinweise vor der Nutzung des Programms durch:
- Dieses Programm dient ausschließlich Lern- und Demonstrationszwecken. Es ist nicht für den produktiven Einsatz vorgesehen.


- Die Datenbank besitzt eine Tabellenform. Sie trägt zu einfacheren Features und einer besseren Lesbarkeit im Editor bei.
  - **Zeilenlänge:** Jede Zeile 61 Zeichen, endet mit `/`.
  - **Guthaben:** 20 Zeichen inkl. Leerzeichen, in Cent, Wert < 2 Milliarden.
  - **Inhaber:** 40 Zeichen lang, im Programm auf 30 Zeichen begrenzt.
  - **Kontonummer:** Entspricht der Zeilennummer minus 1.
  - Diese Form gilt auch für Backups.


- Eine Datenbank mit mindestens einem Eintrag (`accounts.txt` im `data`-Ordner) wird zum Start benötigt.
Es wird aufgrund ihrer streng einzuhaltenden Form empfohlen, diese nur mithilfe des Programms zu verwalten und sie nicht im Schreibmodus zu öffnen.


- Änderungen der Kontodaten werden erst dann in die Datenbank übernommen, wenn das Programm durch die Eingabe `e` beendet wird,
nicht etwa durch Schließen des Fensters.


## Features
Das Programm lässt sich über eine interaktive Auswahl aus einigen Features bedienen. Ihre entsprechenden Funktionen sind im Kapitel "Funktionen" näher erläutert.
### Interaktion mit der Datenbank
- **Auslesen:** Liest die Daten aus der bestehenden Datenbank. Die Datei muss folgende Form haben:
- **Beschreiben:** Schreibt die Kontodaten, bei Programmende in die selbe Datenbank oder nach Wahl auch früher in eine Backup-Datei ein. 
- **Backups:** laden und erstellen, gleiche Funktionsweise wie jeweils Beschreiben und Auslesen der Datenbank. 

### Kontoinformationen
- **Konto anlegen**: Abfrage von Inhaber und Startguthaben, hinzufügen eines neuen Eintrags
- **Drucken:** Abfrage der Kontonummer, Ausgabe der Kontodaten

### Kontoverwaltung
- **Abheben**: interaktiv, fragt Kontonummer und Betrag ab
- **Einzahlen**: siehe "Abheben"
- **Überweisung**: interaktiv, Abfrage von zwei Kontonummern (Zahler, Empfänger) sowie Betrag

### Dateien
Nach Ordner unterteilt:
- **bin/bankingsystem:** ausführbares Programm
- **data/:** enthält die zugrunde liegende Datenbank
  - **accounts.txt:** Grundlegende Datenbank mit einem Eintrag (vom Programm benötigt)
  - **accounts (Kopie).txt:** Kopie dieser Datei, für Wiederherstellung hinzugefügt
  - **accounts beispiel:** Beispielfall, einige zusätzliche Einträge
- **backups:** enthält Backup-Dateien, zuerst leer, kann enthalten:
  - **backup-new:** wird erstellt, um gegen Fehler beim erstellen eines neuen Kontos abzusichern. Diese enstehen, wenn der Arbeitsspeicher vollständig ausgenutzt wird.
  - **backup-auto:** beim Programmstart erstellt
  - **backup-user:** durch Befehl 's' erstellt.

## Installation
Das Installationspaket befindet sich im entsprechenden [Release](https://github.com/theo-fh/BankingSystem/releases).
Nach dem Entpacken des Archivs befindet sich das ausführbare Programm im `bin`-Ordner unter dem Namen `bankingsystem` (Windows: `bankingsystem.exe`).

Weiteres Vorgehen je nach Betriebssystem:
- Bei **Linux-Systemen** muss das Programm ausführbar gemacht werden, indem man den `bin`-Ordner im Terminal aufruft und `chmod +x ./bankingsystem` eingibt.
Dann kann das Programm mit `./bankingsystem` ausgeführt werden.

- **Windows-Systeme** warnen den Nutzer über die Ausführung des Programms, da der Herausgeber unbekannt ist. Diese Warnung kann ignoriert werden.

## Nutzung
Wie bereits im Kapitel "Features" angedeutet, besteht die Benutzeroberfläche aus einer Reihe von Eingabeaufforderungen. Sie besteht aus zwei aufeinander folgenden Abfragen:

1. **Operation:** der Nutzer wählt, welches Feature des Programms er anwenden will. Alternativ kann durch den Hilfe-Befehl `?` die Auswahl an Features aufgelistet oder mit `e` gespeichert und beendet werden.


2. **Werte:** die Werte, mit dem das gewählte Feature arbeiten soll werden abgefragt. Das sind z.B. Kontonummer und Betrag. Meist werden mehrere Werte nacheinander abgefragt.

Nach diesen beiden Abfragen wiederholt sich dieses Muster, bis im ersten Schritt `e` eingegeben wird.

## Funktionen und Variablen
Weitere Erläuterungen zu den einzelnen Funktionen sind in den Kommentaren im Header `banking_funktionen.h` zu finden.
### Grundlegende Variablen
- `anzahl`: Anzahl der momentan angelegten Konten, für mehrere Funktionen zur Überprüfung benötigt
- `konten`: Array des struct Konto, beinhaltet jeweils Inhaber und Guthaben aller Kontonummern (Indexe des Arrays)

### Funktionen der Features
- `auslesen`:
  - nimmt entgegen: Dateipfad, Adresse eines Arrays (immer `konten`) und Adresse der Anzahl-Variable
  - liest Zeile nach Zeile (Konto nach Konto) die Einträge aus der Datei in ein String und leitet daraus Guthaben und Inhaber des Kontos her
  - ändert die Anzahl der Konten je nach Inhalt der Datei
- `writeToFile` → Beschreiben:
  - nimmt entgegen: Dateipfad, Array und Anzahl
  - quasi das Gegenteil von `auslesen`: schreibt Daten aus Array in einen String, fügt diesen an das Dateiende
  - macht so viele Einträge, wie die Anzahl es vorgibt, zu viele Einträge → undefiniertes Verhalten
- `newAccount` → Konto anlegen:
  - nimmt entgegen: Adresse eines Arrays und Adresse der Anzahl
  - schreibt dem Array mehr Speicher für den neuen Eintrag zu, erstellt vorher ein Backup (andere Fehlerbehandlung zu aufwendig)
  - fragt Daten ab und erstellt einen neuen Eintrag am Ende des Arrays, überprüft vorher ob die Kontonummer existiert und Eintragen des Guthabens in Cent per Integer technisch realisierbar ist
- `drucken`:
  - nimmt entgegen: Array, Anzahl
  - fragt Kontonummer ab, druckt Inhaber und Guthaben, Überprüfung gleich
- `withdraw`, `deposit`, `transfer` → Abheben, Einzahlen, Überweisung:
  - nehmen entgegen: Array, Anzahl
  - fragen Daten ab und ändern die jeweiligen Kontostände, Überprüfung bei `deposit` und `transfer`

### Weitere Funktionen
- `clearInput`: nutzt getchar(), um den Input-Buffer zu leeren
- `anzahlKonten`: wird in `auslesen` gecallt, um `anzahl` anfänglich zu bestimmen bzw. beim laden eines Backups zu ändern
- `hilfe`: gibt alle Eingabemöglichkeiten für den ersten Schritt der Nutzung aus
- `chooseBackup`: Abfrage, um im Befehl `l` den Backup-Pfad auszuwählen ist von `auslesen` in diesem Fall getrennt, um komplizierten Umgang mit dem Array innerhalb mehrerer Funktionen zu vermeiden