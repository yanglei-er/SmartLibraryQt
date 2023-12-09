#include <SoftwareSerial.h>
SoftwareSerial ble(4, 5); // (RX, TX)

void setup() 
{
  Serial.begin(9600);
  ble.begin(9600);
}

void loop() 
{
  // 蓝牙接收
  if(ble.available())
  {
    String BluetoothData = ble.readString();
    Serial.println(BluetoothData);
  }

  // 蓝牙发送
  if(Serial.available())
  {
    String SerialData = Serial.readString();
    ble.print(SerialData);
  }
  delay(200);
}