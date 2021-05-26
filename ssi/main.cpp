#include <main.h>
#include <stdlib.h>
#include <terminal.h>
#include <wirish/wirish.h>

#define DIRECTION_DATA 29
#define DIRECTION_CLOCK 30
#define DATA 25
#define CLOCK 9

void clock_tick() {
  digitalWrite(CLOCK, LOW);
  delay_us(1);
  digitalWrite(CLOCK, HIGH);
  delay_us(1);
}

/**
 * Setup function
 */
void setup() {
  pinMode(DIRECTION_DATA, OUTPUT);
  digitalWrite(DIRECTION_DATA, LOW);

  pinMode(DIRECTION_CLOCK, OUTPUT);
  digitalWrite(DIRECTION_CLOCK, HIGH);

  pinMode(CLOCK, OUTPUT);
  digitalWrite(CLOCK, HIGH);

  terminal_init(&SerialUSB);
}

/**
 * Loop function
 */
void loop() { terminal_tick(); }

TERMINAL_COMMAND(enc, "Read encoder") {
  bool ack = false;
  int bits = 30;
  uint32_t data = 0;

  while (bits > 0) {
    clock_tick();
    uint8_t d = digitalRead(DATA);

    if (ack) {
      data = (data << 1) | d;
      bits--;
    } else {
      if (d == 0) {
        ack = true;
      }
    }
  }

  uint32_t crc = (data >> 0) & 0b111111;
  uint32_t warn = (data >> 6) & 0b1;
  uint32_t error = (data >> 7) & 0b1;
  uint32_t value = (data >> 8) & 0b11111111111111111111;

  terminal_io()->print("CRC: ");
  terminal_io()->println(crc);
  terminal_io()->print("Error: ");
  terminal_io()->println(error);
  terminal_io()->print("Warn: ");
  terminal_io()->println(warn);
  terminal_io()->print("value: ");
  terminal_io()->println(value);
}