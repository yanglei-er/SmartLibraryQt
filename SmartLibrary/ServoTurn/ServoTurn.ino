#include <Servo.h>
#define INTERRUPT_PIN 2

volatile bool turn = false;

Servo servo_left;
Servo servo_right;
#define LEFT_SERVO_PIN 12
#define RIGHT_SERVO_PIN 13

void servoTurn()
{
  turn = true;
}

void setup()
{
  Serial.begin(9600);

  //中断设置
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), servoTurn, FALLING);

  servo_left.attach(LEFT_SERVO_PIN);
  servo_right.attach(RIGHT_SERVO_PIN);
  servo_left.write(86);
  servo_right.write(94);
}

void loop()
{
  Serial.println("转动");
  for(int angle = 0; angle <=30; angle++)
  {
    servo_left.write(86-angle);
    servo_right.write(94+angle);
    delay(80);
  }
  delay(5000);
  for (int angle = 0; angle <=30; angle++)
  {
    servo_left.write(56+angle);
    servo_right.write(124-angle);
    delay(10);
  }
  delay(5000);

}

