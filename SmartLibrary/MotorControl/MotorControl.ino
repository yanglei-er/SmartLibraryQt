//控制中断
#define INTERRUPT_PIN 4
String previous_state = "启动";

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
  SPI.begin();
  mfrc522.PCD_Init(); 
  //中断设置
  pinMode(INTERRUPT_PIN, OUTPUT);
  digitalWrite(INTERRUPT_PIN, HIGH);
}

void loop()
{
  if(IrReceiver.decode())
  {
    if(IrReceiver.decodedIRData.command == 7)
    {
      motorStop();
    }
    else if(IrReceiver.decodedIRData.command == 21)
    {
      motorStart();
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
      if(status == MFRC522::STATUS_OK)
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

void motorStart()
{
  if(previous_state == "停止")
  {
    Serial.println("启动");
    previous_state = "启动";
    digitalWrite(INTERRUPT_PIN, LOW);
    delayMicroseconds(500);
    digitalWrite(INTERRUPT_PIN, HIGH);
  }
}

void motorStop()
{
  if(previous_state == "启动")
  {
    Serial.println("停止");
    previous_state = "停止";
    digitalWrite(INTERRUPT_PIN, LOW);
    delayMicroseconds(500);
    digitalWrite(INTERRUPT_PIN, HIGH);
  }
}