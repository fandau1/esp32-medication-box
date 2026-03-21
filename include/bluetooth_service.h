#pragma once

#include <Arduino.h>
#include <BLECharacteristic.h>

#include <functional>
#include <string>

class BluetoothService {
 public:
  void begin(const char* deviceName);
  bool isConnected() const;
  bool sendJson(const String& json);
  void setOnReceive(std::function<void(const std::string&)> callback);

 private:
  class ServerCallbacks;
  class RxCallbacks;

  void handleIncoming(const std::string& payload);

  bool deviceConnected_ = false;
  BLECharacteristic* txCharacteristic_ = nullptr;
  BLECharacteristic* rxCharacteristic_ = nullptr;
  std::function<void(const std::string&)> onReceive_;
};