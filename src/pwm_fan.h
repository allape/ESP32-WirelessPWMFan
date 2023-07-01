#ifndef WIRELESS_PWM_FAN_H
#define WIRELESS_PWM_FAN_H
#include <BLECharacteristic.h>
#include <analogWrite.h>
#include <EEPROM.h>

#define PWM_FAN_TAG  "[PWM_FAN]"

#define PWN_CHAN        0
#define PWN_FREQ        31
// #define RPM_PIN 22  // TODO

#define CMD_RESERVED    0
#define CMD_THROTTLE    1
#define CMD_NEXT_PIN    2

struct Data {
    uint8_t pwm;
    uint8_t outputPin; // fan drive pin
    uint8_t inputPin;  // speed detection
};

#define PIN_SET_COUNT 2
uint8_t PinSet[PIN_SET_COUNT] = {19, 5};

class PWMFanBLECallback: public BLECharacteristicCallbacks {
private:
    uint8_t            currentPin = 0;
    uint8_t            currentPWM = 0;
    uint               currentRPM = 0;
public:
    explicit PWMFanBLECallback() {
        ledcSetup(PWN_CHAN, PWN_FREQ, 8);

        Data data = {};
        EEPROM.get(0, data);

        if (!data.outputPin) {
            data.outputPin = PinSet[0];
        }

        this->setPin(data.outputPin);
        this->setPWM(data.pwm);
    }

    void setPin(uint8_t pin) {
        if (this->currentPin) {
            digitalWrite(this->currentPin, 0);
        }
        pinMode(pin, OUTPUT);
        this->currentPin = pin;
        ledcAttachPin(this->currentPin, PWN_CHAN);
    }

    void setPWM(byte pwm) {
        this->currentPWM = pwm;
        ledcWrite(PWN_CHAN, pwm);
        analogWrite(BUILTIN_LED, pwm);
        EEPROM.put(0, Data{
            pwm: pwm,
            outputPin: this->currentPin,
        });
        EEPROM.commit();
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
                break;
            case CMD_NEXT_PIN:
                if (value.length() >= 2) {
                    uint8_t pinIndex = value[1];
                    if (pinIndex >= PIN_SET_COUNT) {
                        return;
                    }
                    uint8_t pin = PinSet[pinIndex];
                    setPin(pin);
                    Serial.printf("%s new OutputPin: %d\r\n", PWM_FAN_TAG, pin);
                }
                break;
            default:
                break;
            }
        }
    }
};

#endif
