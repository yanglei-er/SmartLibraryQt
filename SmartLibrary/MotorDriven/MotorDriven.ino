#include <avr/sleep.h>
volatile bool stop = false;

//中断口
#define INTERRUPT_PIN 21

//电机设置
#define right_IN1 22
#define right_IN2 23
#define right_IN3 24
#define right_IN4 25
#define right_ENA 6
#define right_ENB 5
#define left_IN1 26
#define left_IN2 27
#define left_IN3 28
#define left_IN4 29
#define left_ENA 8
#define left_ENB 7

// 循迹设置
#define leftA_track_PIN 13
#define leftB_track_PIN 12
#define middle_track_PIN 11
#define rightA_track_PIN 10
#define rightB_track_PIN 9
float Kp = 10, Ki = 0.5, Kd = 0;                    //pid弯道参数参数 
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;//pid直道参数 
float previous_error = 0, previous_I = 0;           //误差值 
int sensor[5] = {0, 0, 0, 0, 0};                    //传感器状态

static int initial_motor_speed = 80;               //初始速度
#define big_left -4
#define small_left -2
#define stright 0
#define small_right 2
#define big_right 4
#define turn_max 255
#define turn_min 0
#define delay_time 50

void read_sensor_values();  //读取初值 
void calc_pid();  //计算pid 
void motor_control(); //电机控制

void changeState()
{
  Serial.println("触发中断");
  stop = !stop;
  if(!stop)
  {
    sleep_disable();
    delayMicroseconds(500);
    motorStart();
  }
}

void setup()
{
  Serial.begin(9600);
  //设置睡眠参数
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  // 设置中断
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), changeState, FALLING);
  // 循迹引脚初始化
  pinMode (leftA_track_PIN, INPUT); //设置引脚为输入引脚
  pinMode (leftB_track_PIN, INPUT); //设置引脚为输入引脚
  pinMode (middle_track_PIN, INPUT);//设置引脚为输入引脚
  pinMode (rightA_track_PIN, INPUT); //设置引脚为输入引脚
  pinMode (rightB_track_PIN, INPUT); //设置引脚为输入引脚

  // 电控引脚初始化
  pinMode (left_IN1, OUTPUT); //设置引脚为输出引脚
  pinMode (left_IN2, OUTPUT); //设置引脚为输出引脚
  pinMode (left_IN3, OUTPUT); //设置引脚为输出引脚
  pinMode (left_IN4, OUTPUT); //设置引脚为输出引脚
  pinMode (left_ENA, OUTPUT); //设置引脚为输出引脚
  pinMode (left_ENB, OUTPUT); //设置引脚为输出引脚
  pinMode (right_IN1, OUTPUT); //设置引脚为输出引脚
  pinMode (right_IN2, OUTPUT); //设置引脚为输出引脚
  pinMode (right_IN3, OUTPUT); //设置引脚为输出引脚
  pinMode (right_IN4, OUTPUT); //设置引脚为输出引脚
  pinMode (right_ENA, OUTPUT); //设置引脚为输出引脚
  pinMode (right_ENB, OUTPUT); //设置引脚为输出引脚

  // 电控状态初始化
  motorStart();
}

void loop()
{
  if(!stop)
  {
    read_sensor_values(); //读取传感器值
    calc_pid(); //计算PID
    motor_control();  //驱动电机
    delay(delay_time);
  }
  else
  {
    motorsStop();
    error = 0, P = 0, I = 0, D = 0, PID_value = 0; 
    previous_error = 0, previous_I = 0;
    sleep_cpu();
  }
}

void read_sensor_values()
{
  sensor[0] = digitalRead(leftA_track_PIN);
  sensor[1] = digitalRead(leftB_track_PIN);
  sensor[2] = digitalRead(middle_track_PIN);
  sensor[3] = digitalRead(rightA_track_PIN);
  sensor[4] = digitalRead(rightB_track_PIN);

  if((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))
  {//                     1 0 0 0 1 停车
    error = 0;
    stop = true;
  }
  else if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
  {//                     0 0 0 0 0 停车
    error = 0;
    stop = true;
  }
  else if((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))
  {//                      1 1 1 1 1按原来走
    error = error;
  }
  else if (sensor[4] == 0)
  {
    error = big_right;//          1 1 1 1 0 大右转
  }
  else if (sensor[3] == 0)
  {
    error = small_right;//          1 1 1 0 1 小右转
  }
  else if (sensor[2] == 0)
  {
    error = stright;//          1 1 0 1 1 直走
  }
  else if (sensor[1] == 0)
  {
    error = small_left;//         1 0 1 1 1 小左转
  }
  else if (sensor[0] == 0)
  {
    error = big_left;//         0 1 1 1 1 大左转
  }
}

void calc_pid()
{
  P = error;
  I = I + error;
  D = error - previous_error;
 
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
 
  previous_error = error;
}

void motor_control()
{
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;

  if(error == 0)
  {
    if(left_motor_speed > initial_motor_speed)
    {
      left_motor_speed = left_motor_speed - 1;
    }
    else
    {
      left_motor_speed = left_motor_speed + 1;
    }
    if(right_motor_speed > initial_motor_speed)
    {
      right_motor_speed = right_motor_speed - 1;
    }
    else
    {
      right_motor_speed = right_motor_speed + 1;
    }
  }
  
  if(left_motor_speed < turn_min)
  {
    left_motor_speed = turn_min;
  }
  if(left_motor_speed > turn_max)
  {
    left_motor_speed = turn_max;
  }
  if(right_motor_speed < turn_min)
  {
    right_motor_speed = turn_min;
  }
  if(right_motor_speed > turn_max)
  {
    right_motor_speed = turn_max;
  }
  Serial.println(left_motor_speed);
  Serial.println(right_motor_speed);
  motorsWrite(left_motor_speed,right_motor_speed);

  // 输出
  if(error == stright)
  {
    Serial.println("直行"); 
  }
  else if(error == small_right)
  {
    Serial.println("小右转");
  }
  else if(error == big_right)
  {
    Serial.println("大右转");
  }
  else if(error == small_left)
  {
    Serial.println("小左转");
  }
  else if(error == big_left)
  {
    Serial.println("大左转");
  }
  else
  {
    Serial.println("停车");
  }
}

//速度设定范围(0,255)
void motorsWrite(int speedL, int speedR)
{
  analogWrite(left_ENA, speedR);
  analogWrite(left_ENB, speedR);
  // analogWrite(left_ENB, abs(speedR-error));
  analogWrite(right_ENA, speedL);
  analogWrite(right_ENB, speedL);
  // analogWrite(right_ENB, abs(speedL-error));
}

void motorStart()
{
  digitalWrite(left_IN1, 1);
  digitalWrite(left_IN2, 0);
  digitalWrite(left_IN3, 0);
  digitalWrite(left_IN4, 1);
  digitalWrite(right_IN1, 1);
  digitalWrite(right_IN2, 0);
  digitalWrite(right_IN3, 1);
  digitalWrite(right_IN4, 0);
  analogWrite(left_ENA, 0);
  analogWrite(left_ENB, 0);
  analogWrite(right_ENA, 0);
  analogWrite(right_ENB, 0);
}

void motorsStop() 
{
  digitalWrite(left_IN1, 0);
  digitalWrite(left_IN2, 0);
  digitalWrite(left_IN3, 0);
  digitalWrite(left_IN4, 0);
  digitalWrite(right_IN1, 0);
  digitalWrite(right_IN2, 0);
  digitalWrite(right_IN3, 0);
  digitalWrite(right_IN4, 0);
  analogWrite(left_ENA, 0);
  analogWrite(left_ENB, 0);
  analogWrite(right_ENA, 0);
  analogWrite(right_ENB, 0);
}