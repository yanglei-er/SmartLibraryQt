#include <SoftwareSerial.h>
SoftwareSerial softSerial(4, 5); // (TX, RX)

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
    softSerial.print(SerialData);
  }
  delay(500);
}