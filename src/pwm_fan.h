#ifndef WIRELESS_PWM_FAN_H
#define WIRELESS_PWM_FAN_H
#include <BLECharacteristic.h>
#include <analogWrite.h>
#include <EEPROM.h>

#define PWM_FAN_TAG  "[PWM_FAN]"

#define PWN_CHAN        0
#define PWN_FREQ        156250
#define PWM_PIN         5
// #define RPM_PIN 22  // TODO
#define MAX_PWM         255

#define CMD_RESERVED    0
#define CMD_THROTTLE    1

class PWMFanBLECallback: public BLECharacteristicCallbacks {
private:
    uint8_t            currentPWM = 0;
    uint               currentRPM = 0;
public:
    explicit PWMFanBLECallback() {
        ledcSetup(PWN_CHAN, PWN_FREQ, 8);
        ledcAttachPin(PWM_PIN, PWN_CHAN);

        this->setPWM(EEPROM.readByte(0));

        analogWrite(BUILTIN_LED, 0, 255);
    }

    void setPWM(byte pwm) {
        this->currentPWM = pwm;
        ledcWrite(PWN_CHAN, pwm);
        analogWrite(BUILTIN_LED, pwm, 255);
        EEPROM.put(0, pwm);
    }

    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string value = pCharacteristic->getValue();
        if (!value.empty()) {
            uint8_t cmd = value[0];
            switch (cmd) {
            case CMD_RESERVED:
                Serial.printf("%s CMD_RESERVED\r\n", PWM_FAN_TAG);
                break;
            case CMD_THROTTLE:
                if (value.length() >= 2) {
                    uint8_t pwmValue = value[1];
                    setPWM(pwmValue);
                    Serial.printf("%s new PWM: %d\r\n", PWM_FAN_TAG, pwmValue);
                }
                break;
            default:
                break;
            }
        }
    }
};

#endif
