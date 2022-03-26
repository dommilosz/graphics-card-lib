#include "I2CCom.h"
I2CCom_Master I2CCom(&Wire1);
#include "GraphicsCardI2CCom.h";
GraphicsCardI2CCom gI2C(&I2CCom);

void setup() {
  // put your setup code here, to run once:
  I2CCom.OnDeviceConnected = OnConnected;
  I2CCom.OnDeviceDisconnected = OnDisconnected;
  I2CCom.begin();
  I2CCom.Ignore(0x27);
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.read() == 't') {
    Serial.println("Init");
    gI2C.Init();
  }
  I2CCom.ScanDevices();
  delay(50);
}

void OnConnected(int address) {
  Serial.print("Connected: ");
  Serial.println(address);
}

void OnDisconnected(int address) {
  Serial.print("Disconnected: ");
  Serial.println(address);
}
