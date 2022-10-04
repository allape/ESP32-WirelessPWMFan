#ifndef WIRELESS_PWM_FAN_BLE_H
#define WIRELESS_PWM_FAN_BLE_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLECharacteristic.h>

#define SERVICE_UUID        "7b742148-fe4e-411e-a762-fd5967d77491"
#define COMMON_CHAR_UUID    "10971da0-aa16-4042-a054-a1501bb3f2d5"
#define TAG_BLE             "[BLE]"
#define BLE_NAME            "WirelessPWMFan"

#define DEFAULT_VALUE       "AS"

class ServerCallbacks: public BLEServerCallbacks {
public:
    void onDisconnect(BLEServer *pServer) override {
        BLEDevice::startAdvertising();
    }
};

class BLEController {
private:
    BLECharacteristic *pChar;
public:
    explicit BLEController(BLECharacteristicCallbacks* charCallback) {
        Serial.printf("%s %s\r\n", TAG_BLE, "Starting BLE...");

        BLEDevice::init(BLE_NAME);
        BLEServer *pServer = BLEDevice::createServer();

        pServer->setCallbacks(new ServerCallbacks());
        BLEService *pService = pServer->createService(SERVICE_UUID);

        this->pChar = pService->createCharacteristic(
                COMMON_CHAR_UUID,
                BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE |
                BLECharacteristic::PROPERTY_NOTIFY
        );
        this->pChar->setCallbacks(charCallback);
        this->pChar->setValue(DEFAULT_VALUE);

        pService->start();

        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    }

    BLECharacteristic* getChar() {
        return this->pChar;
    }
};

#endif
