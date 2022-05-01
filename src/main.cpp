#include <Arduino.h>
#include <analogWrite.h>
#include "printer.h"
#include "ble.h"
#include "pwm_fan.h"

#define MAIN_TAG  "MAIN"

uint8_t currentValue = 0;

PWMFanBLECallback* callback;
BLEController* controller;

void setup() {
  Printer::setup();
  callback = new PWMFanBLECallback();
  controller = new BLEController(callback);
  callback->setBLECharacteristic(controller->getChar());
}

void loop() {
  callback->refreshCurrentRPMAndNotifyIfChanged();
  delay(100);
}
