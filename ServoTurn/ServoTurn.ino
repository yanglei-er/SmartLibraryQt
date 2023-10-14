#include <Servo.h>

Servo servo1;
Servo servo2;

void setup() {
  servo1.attach(9);
  servo2.attach(10);

    servo1.write(0);
    servo2.write(180);

    delay(2000);
}

void loop() {
  for (int angle = 0; angle <=90; angle++) {
    servo1.write(angle);
    servo2.write(180-angle);
    delay(15);
}
  for (int angle = 0; angle <=90; angle++) {
    servo1.write(90-angle);
    servo2.write(90+angle);
    delay(15);
}


}
