#include <SoftwareSerial.h>
// 密码默认 1234

// 设置软串口针脚
SoftwareSerial ble(12, 13); // (TX, RX)
SoftwareSerial barcode(10, 11);
String barcode_Data = "";

void setup() 
{
  Serial.begin(9600);
  ble.begin(9600);
  barcode.begin(9600);
  Serial.println("蓝牙启动");
}

void loop() 
{
  // 蓝牙接收
  if(ble.available())
  {
    String BluetoothData = ble.readString();
    Serial.println(BluetoothData);
  }

  if(barcode.available())
  {
    barcode_Data = barcode.readString();
    ble.write(barcode_Data.c_str());
  }
  
  // 蓝牙发送
  if(Serial.available())
  {
    String SerialData = Serial.readString();
    ble.write(SerialData.c_str());
  }
  delay(500);
}
