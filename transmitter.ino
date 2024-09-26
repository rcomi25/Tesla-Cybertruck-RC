#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 myRF24(7, 8); // Inizializza l'oggetto RF24 con i pin CE e CSN
byte address[6] = "00001"; // Indirizzo del pipe di scrittura

const int potLX = A0, potRX = A1; // Pin analogici per i potenziometri
int valPotLX, valPotRX, val, valPuls, contatoreP, contatoreNeo;
const int puls0 = A2, puls1 = A3; // Pin analogici per i pulsanti
bool statoR = false, statoP = false, statoNeo = false; // Stati per i vari effetti LED

void setup() {
myRF24.begin(); // Inizializza il modulo RF24
myRF24.openWritingPipe(address); // Apre il pipe di scrittura
myRF24.setPALevel(RF24_PA_MIN); // Imposta il livello di potenza a minimo
myRF24.stopListening(); // Ferma l'ascolto per la modalità di trasmissione

// Impostazione dei pin come OUTPUT
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);

pinMode(puls0, INPUT_PULLUP); // Imposta i pin dei pulsanti con resistenza di pull-up interna
pinMode(puls1, INPUT_PULLUP);
}

void loop() {
// Legge i valori dei potenziometri e li mappa su un nuovo intervallo
valPotLX = map(analogRead(potLX), 0, 1023, 5, 15);
valPotRX = map(analogRead(potRX), 0, 1024, 10, 21) + 1;

if (digitalRead(puls0) && digitalRead(puls1)) {
valPuls = 0;
contatoreP = 0;
contatoreNeo = 0;
} else if (!digitalRead(puls0) && digitalRead(puls1)) {
contatoreNeo++;
if ((contatoreNeo == 10) && (statoP)) {
statoNeo = !statoNeo; // Cambia lo stato di Neo
}
} else if (digitalRead(puls0) && !digitalRead(puls1) && !statoNeo && !statoP) {
statoR = !statoR; // Cambia lo stato di R
delay(500); // Debounce
} else {
contatoreP++;
if ((contatoreP == 10) && (valPotLX == 5)) {
statoP = !statoP; // Cambia lo stato di P
}
}

// Controlla gli stati per impostare i LED
if (statoR) {
valPuls = 2;
digitalWrite(3, HIGH);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
} else if (statoNeo) {
valPuls = 1;
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
delay(100);
digitalWrite(3, HIGH);
digitalWrite(4, HIGH);
digitalWrite(5, HIGH);
delay(90);
} else if (statoP) {
valPuls = 3;
digitalWrite(3, LOW);
digitalWrite(4, HIGH);
digitalWrite(5, LOW);
} else {
valPuls = 0;
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, HIGH);
}

// Calcola il valore combinato
val = (valPuls * 1000) + (valPotLX * 100) + valPotRX;

// Crea il messaggio da inviare via RF24
char text[8];
sprintf(text, "%d,%d,%d", valPuls, valPotLX, valPotRX);
myRF24.write(&text, sizeof(text)); // Invia il messaggio

delay(10); // Piccolo ritardo per stabilizzare il ciclo
}

