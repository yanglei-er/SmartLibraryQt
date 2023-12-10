#include <SoftwareSerial.h>
SoftwareSerial uno1(2,3); //RX, TX
SoftwareSerial uno3(5,6); //RX, TX

String state = "stop";

//RFID配置
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN  9
#define MISO_PIN 12
#define MOSI_PIN 11
#define SCK_PIN  13
#define SDA_PIN  10
MFRC522 mfrc522(SDA_PIN, RST_PIN);
byte nuidPICC[4]; // 存储读取的UID
byte pageAddr = 0x06;

int shelfNum = 0;

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() 
{
  Serial.begin(9600);
  uno1.begin(9600);
  uno3.begin(9600);
  uno1.listen();
  //RFID启动
  SPI.begin();
  mfrc522.PCD_Init(); 
}

void loop()
{
  if(uno1.available())
  {
    String data = uno1.readString();
    Serial.println(data);
    if(data == "stop")
    {
      state = "stop";
      uno3.print("stop");
    }
    else if(data.startsWith("带我去"))
    {
      state = "start";
      uno3.print("start");
      shelfNum = getValue(data, ',', 1).toInt();
    }
  }

  if(uno3.available())
  {
    String data = uno3.readString();
    if(data == "arrived")
    {
      uno1.print("servo_turn");
      uno1.listen();
    }
    if(data == "return")
    {
      uno3.print("return");
      uno1.listen();
    }
  }

  if(state == "start")
  {
      //RFID读取
    if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
      if(mfrc522.uid.uidByte[0] != nuidPICC[0] || mfrc522.uid.uidByte[1] != nuidPICC[1] || mfrc522.uid.uidByte[2] != nuidPICC[2] || mfrc522.uid.uidByte[3] != nuidPICC[3])
      {
        // 保存读取到的UID
        for (byte i = 0; i < 4; i++)
        {
          nuidPICC[i] = mfrc522.uid.uidByte[i];
        }
        byte buffer[18];
        byte size = sizeof(buffer);
        MFRC522::StatusCode status;
        status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(pageAddr, buffer, &size);
        if(status == MFRC522::STATUS_OK)
        {
          check_Num(buffer[0]-48);
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();
        }
      }
    }
    delay(20);
  }
  else
  {
    delay(200);
  }
}

void check_Num(const int num)
{
  Serial.println(num);
  if(num == shelfNum)
  {
    uno3.print("turn");
    uno3.listen();
  }
}
