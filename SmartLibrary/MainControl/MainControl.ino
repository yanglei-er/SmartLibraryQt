#include <IRremote.hpp>

//舵机设置
#include <Servo.h>
Servo servo_left;
Servo servo_right;
#define LEFT_SERVO_PIN 12
#define RIGHT_SERVO_PIN 13
#define INTERRUPT_PIN 2
volatile bool turn = false;

#include <SoftwareSerial.h>
//蓝牙设置 密码默认1234
SoftwareSerial ble(10, 11); // (RX, TX)
//扫码设置
#define scanControl 9
SoftwareSerial barcode(4,5);//(RX, TX)

SoftwareSerial uno2(2,3);//(RX, TX)

//舵机转动
void servoTurn()
{
  turn = true;
}

void setup() 
{
  Serial.begin(9600);
  ble.begin(9600);
  barcode.begin(9600);
  uno2.begin(9600);

  //扫码设置
  pinMode(scanControl, OUTPUT);
  digitalWrite(scanControl, LOW);

  //中断设置
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), servoTurn, FALLING);

  //舵机归位
  servo_left.attach(LEFT_SERVO_PIN);
  servo_right.attach(RIGHT_SERVO_PIN);
  servo_left.write(86);
  servo_right.write(94);
}

void loop() 
{
  
  if(ble.available())
  {
    //uno2.println(bleRead());
    Serial.println(bleRead());
  }
  if(barcode.available())
  {
    String str = barcodeRead();
    bleWrite(str);
  }
  bleWrite("123496");
  delay(500);
}

void bleWrite(const String &str)
{
  ble.write(str.c_str());
}

String bleRead()
{
  String data = ble.readString();
  return data;
}

void scanStart()
{
  digitalWrite(scanControl, HIGH);
}

String barcodeRead()
{
  String data = barcode.readString();
  return data;
}

void servocontrol()
{
  Serial.println("转动");
  for(int angle = 0; angle <=30; angle++)
  {
    servo_left.write(86-angle);
    servo_right.write(94+angle);
    delay(80);
  }
  delay(5000);
  
  }
  

