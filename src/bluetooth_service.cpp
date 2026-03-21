#include "bluetooth_service.h"

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

namespace {
constexpr const char* SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab";
constexpr const char* CHAR_UUID_TX = "12345678-1234-1234-1234-1234567890ac";
constexpr const char* CHAR_UUID_RX = "12345678-1234-1234-1234-1234567890ad";
}

class BluetoothService::ServerCallbacks : public BLEServerCallbacks {
 public:
  explicit ServerCallbacks(BluetoothService* owner) : owner_(owner) {}

  void onConnect(BLEServer* pServer) override {
    (void)pServer;
    owner_->deviceConnected_ = true;
    Serial.println("BLE client connected.");
  }

  void onDisconnect(BLEServer* pServer) override {
    owner_->deviceConnected_ = false;
    Serial.println("BLE client disconnected. Restarting advertising...");
    pServer->startAdvertising();
  }

 private:
  BluetoothService* owner_;
};

class BluetoothService::RxCallbacks : public BLECharacteristicCallbacks {
 public:
  explicit RxCallbacks(BluetoothService* owner) : owner_(owner) {}

  void onWrite(BLECharacteristic* pCharacteristic) override {
    const std::string value = pCharacteristic->getValue();
    if (!value.empty()) {
      owner_->handleIncoming(value);
    }
  }

 private:
  BluetoothService* owner_;
};

void BluetoothService::begin(const char* deviceName) {
  Serial.println("\n==============================");
  Serial.println("ESP32 BLE STARTING...");
  Serial.println("==============================");

  Serial.println("\nDEVICE NAME:");
  Serial.println(deviceName);

  Serial.println("\nBLE UUIDs:");
  Serial.print("SERVICE: ");
  Serial.println(SERVICE_UUID);

  Serial.print("TX (ESP → Phone): ");
  Serial.println(CHAR_UUID_TX);

  Serial.print("RX (Phone → ESP): ");
  Serial.println(CHAR_UUID_RX);

  Serial.println("\nInitializing BLE...");

  BLEDevice::init(deviceName);

  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks(this));

  BLEService* service = server->createService(SERVICE_UUID);

  txCharacteristic_ =
      service->createCharacteristic(CHAR_UUID_TX,
                    BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY);
  txCharacteristic_->addDescriptor(new BLE2902());

  rxCharacteristic_ =
      service->createCharacteristic(CHAR_UUID_RX, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
  rxCharacteristic_->setCallbacks(new RxCallbacks(this));

  service->start();

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->start();

  Serial.println("\nBLE READY!");
  Serial.println("Now open nRF Connect and search for:");
  Serial.println(deviceName);
  Serial.println("==============================\n");
}

bool BluetoothService::isConnected() const { return deviceConnected_; }

bool BluetoothService::sendJson(const String& json) {
  if (!deviceConnected_ || txCharacteristic_ == nullptr) {
    Serial.println("Skip send: BLE not connected or TX characteristic unavailable.");
    return false;
  }

  txCharacteristic_->setValue(json.c_str());
  txCharacteristic_->notify();
  return true;
}

void BluetoothService::setOnReceive(std::function<void(const std::string&)> callback) {
  onReceive_ = std::move(callback);
}

void BluetoothService::handleIncoming(const std::string& payload) {
  Serial.println("Received JSON from phone:");
  Serial.println(payload.c_str());

  if (onReceive_) {
    onReceive_(payload);
  }
}