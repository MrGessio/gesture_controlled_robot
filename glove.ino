#include <SoftwareSerial.h>

// HC-12 radio module: TX=pin10, RX=pin11
SoftwareSerial HC12(10, 11);

// Analog pins for flex sensors (fingers 1-5)
const int FLEX_PIN[5] = {A1, A2, A3, A4, A5};

// Raw sensor readings
int sensorData[5] = {0, 0, 0, 0, 0};

// Transmission interval (ms)
const unsigned long SEND_INTERVAL = 120;
unsigned long lastSendTime = 0;

void setup() {
    Serial.begin(9600);
    HC12.begin(9600);

    for (int i = 0; i < 5; i++) {
        pinMode(FLEX_PIN[i], INPUT);
    }

    Serial.println("Glove controller initialized.");
}

void loop() {
    // Read all flex sensors
    for (int i = 0; i < 5; i++) {
        sensorData[i] = analogRead(FLEX_PIN[i]);
    }

    unsigned long now = millis();

    // Send data at fixed interval (non-blocking)
    if (now - lastSendTime >= SEND_INTERVAL) {
        lastSendTime = now;

        // Send each sensor value as a single byte (0-255)
        for (int i = 0; i < 5; i++) {
            HC12.write(sensorData[i] / 4);  // scale 0-1023 -> 0-255
            delay(20);
        }

        // Debug output to Serial Monitor
        Serial.print("Fingers: ");
        for (int i = 0; i < 5; i++) {
            Serial.print(sensorData[i]);
            if (i < 4) Serial.print(" | ");
        }
        Serial.println();
    }
}
