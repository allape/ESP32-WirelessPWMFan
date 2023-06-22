#include <Arduino.h>
#include <analogWrite.h>
#include "ble.h"
#include "pwm_fan.h"

PWMFanBLECallback  *callback;
BLEController      *controller;

void setup() {
  Serial.begin(9600);

  if (!EEPROM.begin(1024)) {
    Serial.printf("failed to start EEPROM...");
    sleep(3600);
  }

  callback = new PWMFanBLECallback();
  controller = new BLEController(callback);
}

void loop() {
  sleep(3600);
}
