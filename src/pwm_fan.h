#ifndef WIRELESS_PWM_FAN_H
#define WIRELESS_PWM_FAN_H
#include <BLECharacteristic.h>
#include <analogWrite.h>
#include <EEPROM.h>
#include "printer.h"

#define PWM_FAN_TAG  "PWM_FAN"

#define PWM_PIN 25
// #define RPM_PIN 22  // TODO
#define MAX_PWM 255

#define CMD_RESERVED 0
#define CMD_THROTTLE 1

class PWMFanBLECallback: public BLECharacteristicCallbacks {
private:
    uint8_t            currentPWM = 0;
    uint               currentRPM = 0;
    BLECharacteristic* pCharacteristic;
public:
    explicit PWMFanBLECallback() {
        this->currentPWM = EEPROM.readByte(0);
    }

    void setBLECharacteristic(BLECharacteristic* pCharacteristic) {
        this->pCharacteristic = pCharacteristic;
    }

    void refreshCurrentRPMAndNotifyIfChanged() {
        // uint currentRPM = analogRead(RPM_PIN);
        // if (currentRPM != this->currentRPM) {
        //     this->currentRPM = currentRPM;
        //     if (this->pCharacteristic != nullptr) {
        //         this->pCharacteristic->setValue(this->currentRPM);
        //         this->pCharacteristic->notify();
        //     }
        // }
    }

    void setPWM(uint8_t pwm) {
        this->currentPWM = pwm;
        analogWrite(PWM_PIN, pwm, MAX_PWM);
        EEPROM.put(0, pwm);
    }

    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string value = pCharacteristic->getValue();
        if (!value.empty()) {
            uint8_t cmd = value[0];
            switch (cmd) {
            case CMD_RESERVED:
                Printer::println(PWM_FAN_TAG, "CMD_RESERVED");
                break;
            case CMD_THROTTLE:
                if (value.length() >= 2) {
                    uint8_t pwmValue = value[1];
                    setPWM(pwmValue);
                    Printer::println(PWM_FAN_TAG, "NEW PWM");
                    Printer::println(PWM_FAN_TAG, pwmValue);
                    this->refreshCurrentRPMAndNotifyIfChanged();
                }
                break;
            default:
                break;
            }
        }
    }
};

#endif
