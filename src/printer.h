#ifndef WIRELESS_PWM_FAN_PRINT_H
#define WIRELESS_PWM_FAN_PRINT_H

#include <Arduino.h>

class Printer {
private:
    static void printTag(const char tag[]) {
        Serial.print("[");
        Serial.print(tag);
        Serial.print("]: ");
    }
public:
    static void setup() {
        Serial.begin(9600);
    }

    static void println(const char tag[], const char c[])
    {
        printTag(tag);
        Serial.println(c);
    }

    static void println(const char tag[], uint8_t c)
    {
        printTag(tag);
        Serial.println(c);
    }

    static void println(const char tag[], uint16_t c)
    {
        printTag(tag);
        Serial.println(c);
    }

    static void println(const char tag[], uint64_t c)
    {
        printTag(tag);
        Serial.println(c);
    }

    static void println(const char tag[], uint32_t c)
    {
        printTag(tag);
        Serial.println(c);
    }

    static void println(const char tag[], bool c)
    {
        printTag(tag);
        Serial.println(c ? "true" : "false");
    }
};

#endif
