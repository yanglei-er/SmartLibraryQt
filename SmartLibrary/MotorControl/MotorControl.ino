//控制中断
#define INTERRUPT_PIN 3
bool state = HIGH;
bool previous_state = LOW;

// 遥控器配置
#include <IRremote.hpp>
#define RECV_PIN 8

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

void setup()
{
  Serial.begin(9600);
  //启动遥控器
  IrReceiver.begin(RECV_PIN); 
  //RFID启动
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init(); 
  //中断设置
  pinMode(INTERRUPT_PIN, OUTPUT);
  digitalWrite(INTERRUPT_PIN, LOW);
}

void loop()
{
  if(IrReceiver.decode())
  {
    if(IrReceiver.decodedIRData.command == 0x7)
    {
      if(previous_state == LOW)
      {
        Serial.println("停车");
        digitalWrite(INTERRUPT_PIN, state);
        previous_state = HIGH;
        state = !state;
      }
    }
    else if(IrReceiver.decodedIRData.command == 0x15)
    {
      if(previous_state == HIGH)
      {
        Serial.println("启动");
        digitalWrite(INTERRUPT_PIN, state);
        previous_state = LOW;
        state = !state;
      }
    }
    IrReceiver.resume();
  }

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
      if(state == MFRC522::STATUS_OK)
      {
        Serial.write(buffer[0]);
        Serial.println();
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
      }
    }
  }
  delay(50);
}