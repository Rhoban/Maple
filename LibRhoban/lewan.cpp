#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wirish/wirish.h>
#include "terminal.h"
#include "lewan.h"

#define SERVO_MOVE_TIME_WRITE   1
#define SERVO_LED_CTRL_WRITE    33

void lewan_init()
{
    pinMode(LEWAN_DIRECTION, OUTPUT);
    digitalWrite(LEWAN_DIRECTION, LEWAN_DIRECTION_RX);
    LEWAN_DEVICE.begin(115200);
}

void lewan_send(uint8_t *data, int n)
{
    digitalWrite(LEWAN_DIRECTION, LEWAN_DIRECTION_TX);
    LEWAN_DEVICE.write(data, n);
    LEWAN_DEVICE.waitDataToBeSent();
    digitalWrite(LEWAN_DIRECTION, LEWAN_DIRECTION_RX);
}

void lewan_send_packet(uint8_t id, uint8_t command, uint8_t *payload, int n)
{
    uint8_t packet[n + 6];
    uint8_t checksum = 0;

    packet[0] = 0x55;
    packet[1] = 0x55;
    packet[2] = id;
    packet[3] = n + 3;
    packet[4] = command;
    checksum = id + n + 3 + command;

    for (int k=0; k<n; k++) {
        packet[5 + k] = payload[k];
        checksum += payload[k];
    }

    checksum = ~checksum;
    packet[n + 5] = checksum;
    lewan_send(packet, sizeof(packet));
}

void lewan_set_pos(uint8_t id, uint16_t angle, uint16_t duration)
{
    uint8_t payload[4];

    payload[0] = angle&0xff;
    payload[1] = (angle>>8)&0xff;
    payload[2] = duration&0xff;
    payload[3] = (duration>>8)&0xff;

    lewan_send_packet(id, SERVO_MOVE_TIME_WRITE, payload, sizeof(payload));
}

void lewan_set_led(uint8_t id, uint8_t on)
{
    lewan_send_packet(id, SERVO_LED_CTRL_WRITE, &on, 1);
}

TERMINAL_COMMAND(lw_angle, "Set an angle")
{
    if (argc != 2) {
        terminal_io()->println("Usage: lw_angle [id] [angle:0-1000]");
    } else {
        lewan_set_pos(atoi(argv[0]), atoi(argv[1]), 0);
    }
}

TERMINAL_COMMAND(lw_blink, "Identify servo")
{
    if (argc != 1) {
        terminal_io()->println("Usage: ld_blink [id]");
    } else {
        uint8_t id = atoi(argv[0]);
        while (!SerialUSB.available()) {
            lewan_set_led(id, 1);
            delay(300);
            lewan_set_led(id, 0);
            delay(300);
        }
    }
}