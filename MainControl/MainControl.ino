#include <SoftwareSerial.h>

SoftwareSerial bluetooth(8, 7); // (TX, RX)
SoftwareSerial barcode(12, 13); // (TX, RX)

void setup()
{
  bluetooth.begin(9600);
  barcode.begin(9600);
}

void loop()
{
  if(barcode.available())
  {
    delay(200);
    bluetooth.print(barcode.readString());
  }
}
