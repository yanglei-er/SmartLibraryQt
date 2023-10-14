#include <avr/sleep.h>
bool stop = false;

//电机设置
#define right_IN1 36
#define right_IN2 34
#define right_IN3 32
#define right_IN4 30
#define right_ENA 10
#define right_ENB 11
#define left_IN1 22
#define left_IN2 24
#define left_IN3 26
#define left_IN4 28
#define left_ENA 12
#define left_ENB 13

// 循迹设置
#define leftA_track_PIN 40
#define leftB_track_PIN 42
#define middle_track_PIN 44
#define rightA_track_PIN 46
#define rightB_track_PIN 48
float Kp = 10, Ki = 0.5, Kd = 0;                    //pid弯道参数参数 
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;//pid直道参数 
float previous_error = 0, previous_I = 0;           //误差值 
static int initial_motor_speed = 120;               //初始速度
int sensor[5] = {0, 0, 0, 0, 0};                    //传感器状态

void read_sensor_values();  //读取初值 
void calc_pid();  //计算pid 
void motor_control(); //电机控制

void setup()
{
  Serial.begin(9600);
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();

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
  digitalWrite(left_IN1, 1);
  digitalWrite(left_IN2, 0);
  digitalWrite(left_IN3, 0);
  digitalWrite(left_IN4, 1);
  digitalWrite(right_IN1, 1);
  digitalWrite(right_IN2, 0);
  digitalWrite(right_IN3, 1);
  digitalWrite(right_IN4, 0);
}

void loop()
{
  if(!stop)
  {
    read_sensor_values(); //读取传感器值
    calc_pid(); //计算PID
    motor_control();  //驱动电机
    delay(300);
  }
  else
  {
    motorsStop();
    delay(500);
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
  if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))
  {
    error = 2;//          0 0 0 0 1 大右转
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0))
  {
    error = 1;//          0 0 0 1 0 小右转
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
  {
    error = 0;//          0 0 1 0 0 直走
  }
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
  {
    error = -1;//         0 1 0 0 0 小左转
  }
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
  {
    error = -2;//         1 0 0 0 0 大左转
  }
  else if((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))
  {//                     1 1 1 1 1 停车
    error = 0;
    stop = true;
  }
  else
  {
    error = 0;
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
  
  if(left_motor_speed < -255)
  {
    left_motor_speed = -255;
  }
  if(left_motor_speed > 255)
  {
    left_motor_speed = 255;
  }
  if(right_motor_speed < -255)
  {
    right_motor_speed = -255;
  }
  if(right_motor_speed > 255)
  {
    right_motor_speed = 255;
  }
  motorsWrite(left_motor_speed,right_motor_speed);

  // 输出
  if(error == 0)
  {
    Serial.println("直行"); 
  }
  else if(error == 1)
  {
    Serial.println("小右转");
  }
  else if(error == 2)
  {
    Serial.println("大右转");
  }
  else if(error == -1)
  {
    Serial.println("小左转");
  }
  else if(error == -2)
  {
    Serial.println("大左转");
  }
  else if((error == -3) || (error == 3))
  {
    Serial.println("停车");
  }
  else
  {
    Serial.println("左转或右转");
  }
}

//速度设定范围(0,255)
void motorsWrite(int speedL, int speedR)
{
  analogWrite(left_ENA, abs(speedR));
  analogWrite(left_ENB, abs(speedR));
  analogWrite(right_ENA, abs(speedL));
  analogWrite(right_ENB, abs(speedL));
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