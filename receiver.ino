#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_NeoPixel.h>

#define PIN 15
#define NUMPIXELS 10

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
unsigned long previousMillis = 0;
const long interval = 250;
int currentLED = 0;

RF24 myRF24 (7, 8); // Inizializza l'oggetto RF24 con i pin CE e CSN
byte address [6] = "00001"; // Indirizzo del pipe di lettura

char text[32] = ""; // Buffer per il testo ricevuto

int val, valPuls, valPotLX, valPotRX;

const int pinbPuls0 = 14, pinbPuls1 = 15, pinbLX0 = 16, pinbLX1 = 17, pinbLX2 = 18, pinbLX3 = 19, pinbRX0 = 2, pinbRX1 = 3, pinbRX2 = 4, pinbRX3 = 5, pinbRX4 = 6;
int bPuls0, bLX0, bLX1, bLX2, bLX3, bRX0, bRX1, bRX2 , bRX3, bRX4;
int conta = 0;

void setup()
{
delay(1000); // Ritardo di 1 secondo per stabilizzare il sistema
myRF24.begin(); // Inizializza il modulo RF24
myRF24.openReadingPipe(0, address); // Apre il pipe di lettura
myRF24.setPALevel(RF24_PA_MIN); // Imposta il livello di potenza a minimo
myRF24.startListening(); // Inizia ad ascoltare
pinMode(pinbPuls0, OUTPUT); // Imposta i pin di uscita per i vari controlli
pinMode(pinbLX0, OUTPUT);
pinMode(pinbLX1, OUTPUT);
pinMode(pinbLX2, OUTPUT);
pinMode(pinbLX3, OUTPUT);
pinMode(pinbRX0, OUTPUT);
pinMode(pinbRX1, OUTPUT);
pinMode(pinbRX2, OUTPUT);
pinMode(pinbRX3, OUTPUT);
pinMode(pinbRX4, OUTPUT);
pixels.begin(); // Inizializza la striscia LED
for (int i = 0; i < NUMPIXELS; i++) {
pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Imposta tutti i LED al bianco
}
pixels.show(); // Mostra il colore impostato sui LED
}

void loop()
{
if (myRF24.available()) // Se sono disponibili dati RF
{
myRF24.read(&text, sizeof(text)); // Legge i dati RF nel buffer text
sscanf(text, "%d,%d,%d", &valPuls, &valPotLX, &valPotRX); // Analizza il testo ricevuto in variabili
conta = 0;
}
else
{
if (conta == 200)
{
valPuls = 0;
valPotLX = 0;
valPotRX = 0;
}
else
{
conta++;
}
delay(1);
}

// Calcola i valori dei bit per i vari pin
bLX0 = valPotLX % 2;
bLX1 = (valPotLX / 2) % 2;
bLX2 = (valPotLX / 4) % 2;
bLX3 = (valPotLX / 8) % 2;

bRX0 = valPotRX % 2;
bRX1 = (valPotRX / 2) % 2;
bRX2 = (valPotRX / 4) % 2;
bRX3 = (valPotRX / 8) % 2;
bRX4 = (valPotRX / 16) % 2;

// Imposta lo stato dei pin in base ai valori calcolati
digitalWrite(pinbLX0, bLX0);
digitalWrite(pinbLX1, bLX1);
digitalWrite(pinbLX2, bLX2);
digitalWrite(pinbLX3, bLX3);

digitalWrite(pinbRX0, bRX0);
digitalWrite(pinbRX1, bRX1);
digitalWrite(pinbRX2, bRX2);
digitalWrite(pinbRX3, bRX3);
digitalWrite(pinbRX4, bRX4);

unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= interval) {
previousMillis = currentMillis;

// Ripristina tutti i LED al bianco
for (int i = 0; i < NUMPIXELS; i++) {
pixels.setPixelColor(i, pixels.Color(255, 255, 255));
}

if (valPotRX > 17) {
// Esegui la sequenza di accensione dal LED 5 al LED 9
for (int i = 5; i <= 5 + currentLED; i++) {
pixels.setPixelColor(i, pixels.Color(255, 165, 0)); // Accendi LED dal 5 al 9 in arancione
}
currentLED++;
if (currentLED > 4) currentLED = 0; // Resetta la sequenza quando tutti i LED sono accesi
} else if (valPotRX < 13) {
// Esegui la sequenza di accensione dal LED 4 al LED 0
for (int i = 4; i >= 4 - currentLED; i--) {
pixels.setPixelColor(i, pixels.Color(255, 165, 0)); // Accendi LED dal 4 al 0 in arancione
}
currentLED++;
if (currentLED > 4) currentLED = 0; // Resetta la sequenza quando tutti i LED sono accesi
}

if (valPuls){
// Effetto arcobaleno sui LED
for(uint16_t j = 0; j < 256; j++) {
for(uint16_t i = 0; i < pixels.numPixels(); i++) {
pixels.setPixelColor(i, Wheel((i + j) & 255));
}
pixels.show();
delay(20); // Regola la velocità dell'effetto arcobaleno
}
}

pixels.show(); // Mostra il colore impostato sui LED
}
}

uint32_t Wheel(byte WheelPos) {
WheelPos = 255 - WheelPos;
if(WheelPos < 85) {
return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
} else if(WheelPos < 170) {
WheelPos -= 85;
return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
} else {
WheelPos -= 170;
return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
}


