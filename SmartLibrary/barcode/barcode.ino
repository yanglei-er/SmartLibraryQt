#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // (RX, TX)

void setup() 
{
  Serial.begin(9600);
  softSerial.begin(9600);
}

void loop() 
{
  // 蓝牙接收
  if(softSerial.available())
  {
    String BluetoothData = softSerial.readString();
    Serial.println(BluetoothData);
  }

  // 蓝牙发送
  if(Serial.available())
  {
    String SerialData = Serial.readString();
    Serial.println(SerialData);
    softSerial.print(SerialData);
  }
  delay(500);
}