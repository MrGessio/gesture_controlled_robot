#include <SoftwareSerial.h>
#include <VarSpeedServo.h>

// HC-12 radio module
SoftwareSerial HC12(10, 11);

// Motor driver L298N pins
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 12
#define ENB 13

// Servo objects for 3-axis manipulator + gripper
VarSpeedServo servo1;  // axis 1
VarSpeedServo servo2;  // axis 2
VarSpeedServo servo3;  // axis 3
VarSpeedServo servo4;  // gripper

// Servo positions (degrees)
int pos1 = 90, pos2 = 110, pos3 = 120, pos4 = 180;

// Sensor data from glove (fingers 1-5)
int data[5] = {0, 0, 0, 0, 0};

// Gripper toggle
int gripperState = 0;
unsigned long lastGripTime = 0;
const unsigned long GRIP_DEBOUNCE = 1000;

// Servo movement speed (0-255)
const int SERVO_SPEED = 30;

// Thresholds
const int FLEX_THRESHOLD  = 300;   // finger considered bent
const int THUMB_THRESHOLD = 230;   // strong thumb bend (gripper trigger)
const int PINKY_THRESHOLD = 930;   // pinky considered straight (>930 = extended)

void setup() {
    Serial.begin(9600);
    HC12.begin(9600);

    // Motor driver pins as output
    pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

    // Attach servos
    servo1.attach(6);
    servo2.attach(5);
    servo3.attach(3);
    servo4.attach(2);

    // Move to start positions
    servo1.write(pos1, SERVO_SPEED);
    servo2.write(pos2, SERVO_SPEED);
    servo3.write(pos3, SERVO_SPEED);
    servo4.write(pos4, SERVO_SPEED);

    Serial.println("Robot initialized.");
}

void loop() {
    unsigned long now = millis();

    // Wait for all 5 bytes from glove
    while (HC12.available() == 0) {}

    for (int i = 4; i >= 0; i--) {
        data[i] = HC12.read() * 4;  // scale back to 0-1023 range
        delay(20);
    }

    // Debug output
    Serial.print("Data: ");
    for (int i = 0; i < 5; i++) {
        Serial.print(data[i]);
        if (i < 4) Serial.print(" | ");
    }
    Serial.print(" | Pos: ");
    Serial.print(pos1); Serial.print(" ");
    Serial.print(pos2); Serial.print(" ");
    Serial.print(pos3); Serial.print(" ");
    Serial.println(pos4);

    // ---- MANIPULATOR CONTROL ----

    // Servo 1: finger 2 + optional thumb modifier
    if (data[1] < FLEX_THRESHOLD && data[4] > PINKY_THRESHOLD) {
        if (data[0] < 450) {
            pos1 = constrain(pos1 + 5, 0, 180);
        } else {
            pos1 = constrain(pos1 - 5, 0, 180);
        }
        servo1.write(pos1, SERVO_SPEED);
    }

    // Servo 2: finger 3 + optional thumb modifier
    if (data[2] < FLEX_THRESHOLD && data[4] > PINKY_THRESHOLD) {
        if (data[0] < 450) {
            pos2 = constrain(pos2 + 5, 0, 180);
        } else {
            pos2 = constrain(pos2 - 5, 0, 180);
        }
        servo2.write(pos2, SERVO_SPEED);
    }

    // Servo 3: finger 4 + optional thumb modifier
    if (data[3] < FLEX_THRESHOLD && data[4] > PINKY_THRESHOLD) {
        if (data[0] < 450) {
            pos3 = constrain(pos3 + 10, 0, 180);
        } else {
            pos3 = constrain(pos3 - 1, 0, 180);
        }
        servo3.write(pos3, SERVO_SPEED);
    }

    // Gripper: strong thumb bend toggles open/close (debounced)
    if (data[0] < THUMB_THRESHOLD) {
        if (now - lastGripTime >= GRIP_DEBOUNCE) {
            lastGripTime = now;
            gripperState = !gripperState;
            pos4 = gripperState ? 180 : 100;
            servo4.write(pos4, SERVO_SPEED);
        }
    }

    // ---- DRIVE CONTROL ----

    // Forward: pinky + finger 1
    if (data[4] < PINKY_THRESHOLD && data[0] < 400) {
        digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); digitalWrite(ENA, HIGH);
        digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); digitalWrite(ENB, HIGH);
        Serial.println(">> FORWARD");
    }
    // Backward: pinky + finger 2
    else if (data[4] < PINKY_THRESHOLD && data[1] < FLEX_THRESHOLD) {
        digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  digitalWrite(ENA, HIGH);
        digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  digitalWrite(ENB, HIGH);
        Serial.println(">> BACKWARD");
    }
    // Left: pinky + finger 3
    else if (data[4] < PINKY_THRESHOLD && data[2] < FLEX_THRESHOLD) {
        digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); digitalWrite(ENA, HIGH);
        digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  digitalWrite(ENB, HIGH);
        Serial.println(">> LEFT");
    }
    // Right: pinky + finger 4
    else if (data[4] < PINKY_THRESHOLD && data[3] < FLEX_THRESHOLD) {
        digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  digitalWrite(ENA, HIGH);
        digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); digitalWrite(ENB, HIGH);
        Serial.println(">> RIGHT");
    }
    // Stop
    else {
        digitalWrite(ENA, LOW);
        digitalWrite(ENB, LOW);
    }

    delay(15);
}
