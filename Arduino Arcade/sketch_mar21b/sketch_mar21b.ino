#include <SoftwareSerial.h>

#define RX 2 // Pin RX del Arduino
#define TX 3 // Pin TX del Arduino
SoftwareSerial arduinoSerial(RX, TX);

#define NUM_RELES 9
#define NUM_SENSORES 8
int reles[NUM_RELES] = {4, 5, 6, 7, 8, 9, 10, 11, A5};
int sensores[NUM_SENSORES] = {A0, A1, A2, A3, A4, A5, 12, 13};

void setup() {
    Serial.begin(9600);
    arduinoSerial.begin(9600);
    for (int i = 0; i < NUM_RELES; i++) {
        pinMode(reles[i], OUTPUT);
        digitalWrite(reles[i], LOW);
    }
    for (int i = 0; i < 6; i++) { // Configurar pines analógicos como entrada
        pinMode(sensores[i], INPUT);
    }
    for (int i = 6; i < NUM_SENSORES; i++) { // Configurar pines digitales como entrada
        pinMode(sensores[i], INPUT_PULLUP);
    }
}

void loop() {
    if (arduinoSerial.available()) {
        String message = arduinoSerial.readStringUntil('\n');
        int relayIndex = message.toInt();
        if (relayIndex >= 0 && relayIndex < NUM_RELES) {
            digitalWrite(reles[relayIndex], HIGH);
            delay(500);
            digitalWrite(reles[relayIndex], LOW);
            arduinoSerial.println("OK");
        }
    }
    
    for (int i = 0; i < 6; i++) { // Leer pines analógicos
        if (analogRead(sensores[i]) > 500) {
            arduinoSerial.println(String(i));
            delay(1000);
        }
    }
    for (int i = 6; i < NUM_SENSORES; i++) { // Leer pines digitales
        if (digitalRead(sensores[i]) == LOW) {
            arduinoSerial.println(String(i));
            delay(1000);
        }
    }
}