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
  char r = Serial.read();
  if (r == 't') {
    Serial.println("Init:");
    Serial.println(gI2C.Init());
    Serial.println(gI2C.Clear(255));
    Serial.println(gI2C.Feature_Rectangle(0,0,100,100,100));
  }
  if (r == 'r') {
    Serial.println("Init:");
    //Serial.println(gI2C.Init());
    Serial.println(gI2C.Clear(255));
    Serial.println(gI2C.Feature_Rectangle(0,0,100,100,100));
  }
  if (r == 'i') {
    Serial.println("Enter I2C: ");
    byte buffer[128];
    int length = 0;
    while(true){
      String read = Serial.readStringUntil(' ');
      int _int = read.toInt();
      if(_int<0 || _int>255){
        break;
      }
      buffer[length] = _int;
      length++;
    }
    Serial.println(length);
    Serial.println(gI2C.SendRawCmd(buffer,length));
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
