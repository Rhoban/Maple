#include <stdlib.h>
#include <wirish/wirish.h>
#include <main.h>

#define RESET_PIN       12
#define BOOT0_PIN       13
#define BOOT1_PIN       14

extern "C" {
    void debugLed() {
        digitalWrite(BOARD_LED_PIN, HIGH);
    }
}

void resetSequence()
{
    // Reset sequence
    digitalWrite(BOARD_LED_PIN, HIGH);
    digitalWrite(BOOT0_PIN, HIGH);
    digitalWrite(BOOT1_PIN, LOW);
    delay(100);
    digitalWrite(RESET_PIN, LOW);
    delay(150);
    digitalWrite(RESET_PIN, HIGH);
    delay(150);
    digitalWrite(BOOT0_PIN, LOW);
    digitalWrite(BOARD_LED_PIN, LOW);
}

/**
 * Setup function
 */
void setup()
{
    pinMode(BOARD_LED_PIN, OUTPUT);
    pinMode(BOARD_BUTTON_PIN, OUTPUT);
    pinMode(RESET_PIN, OUTPUT);
    pinMode(BOOT0_PIN, OUTPUT);
    pinMode(BOOT1_PIN, OUTPUT);

    // Serial
    Serial3.begin(57600);
        
    resetSequence();
}

volatile static bool btn = false;

/**
 * Loop function
 */
void loop()
{
    bool tmp = digitalRead(BOARD_BUTTON_PIN);

    if (tmp && !btn) {
        resetSequence();
    }
    btn = tmp;

    while (SerialUSB.available()) {
        Serial3.write(SerialUSB.read());
    }
    while (Serial3.available()) {
        char buffer[64];
        unsigned int n = 0;
        while (Serial3.available() && n<sizeof(buffer)) {
            buffer[n++] = Serial3.read();
        }
        SerialUSB.write(buffer, n);
    }
}
