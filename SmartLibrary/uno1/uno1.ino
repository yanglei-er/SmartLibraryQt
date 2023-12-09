#include <SoftwareSerial.h>
SoftwareSerial ble(4, 5); // (RX, TX)
SoftwareSerial barcode(9, 6); //RX, TX
SoftwareSerial uno2(2, 3); //RX, TX

#define bar_trig 10

void setup()
{
  pinMode(bar_trig, OUTPUT);
  digitalWrite(bar_trig, HIGH);

  Serial.begin(9600);
  barcode.begin(9600);
  ble.begin(9600);
  uno2.begin(9600);
  ble.listen();

}

void loop()
{
  if(ble.available())
  {
    String bleData = ble.readString();
    Serial.println(bleData);
    if(bleData == "scan")
    {
      barcode.listen();
      digitalWrite(bar_trig, LOW);
    }
    else if(bleData == "停车")
    {
      uno2.print("停车");
    }
    else if(bleData.startsWith("带我去"))
    {
      uno2.print(bleData);
      //uno2.listen();
    }
  }

  if(barcode.available())
  {
    digitalWrite(bar_trig, HIGH);  
    String isbn = barcode.readString();
    ble.print(isbn);
    ble.listen();
  }

  if(uno2.available())
  {
    
  }

  delay(200);
}
