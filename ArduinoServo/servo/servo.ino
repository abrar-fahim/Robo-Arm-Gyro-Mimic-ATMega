#include <Servo.h>

Servo servo1;
int one=90;

void setup() {
  servo1.attach(3);  
 // servo1.write(one);
}

void incAngle(int start, int finish)
{
   int i;
  for (i=start;i<=finish;i++){
    servo1.write(i);
    delay(20);
  }
}

void decAngle(int start, int finish)
{
   int i;
  for (i=start;i>=finish;i--){
    servo1.write(i);
    delay(20);
  }
}

void moveArm(int start, int finish, int DELAY)
{
   if(start>finish)
   {
      decAngle(start,finish);
      delay(DELAY);
      incAngle(finish,start);
      delay(DELAY);
   }
   else
   {
      incAngle(start,finish);
      delay(DELAY);
      decAngle(finish,start);
      delay(DELAY);
   }
}

void loop() {
  
    moveArm(90,45,500); //start angle, stop angle, delay after each motion
}
