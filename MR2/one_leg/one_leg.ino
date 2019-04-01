/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo thigh;  // create servo object to control a servo
Servo knee;
// twelve servo objects can be created on most boards

float theta1=0,theta2=0;
float l1=31,l2=32;

float h = 60;

void setup() {
  thigh.attach(8);  // attaches the servo on pin 9 to the servo object
  knee.attach(11);
  thigh.write(70);
  knee.write(90);
  delay(5000);

  return;
  ik(8,h);
  thigh.write(90+(int)theta1);
  knee.write(90-(int)theta2);
  delay(2000);

  //Serial.begin(9600);
  
}

void ik(float x, float y) {
  theta1 = atan2(x,y) + acos((l1*l1+x*x+y*y-l2*l2)/2/l1/sqrt(x*x+y*y));
  theta2 = PI-acos((l1*l1+l2*l2-x*x-y*y)/2/l1/l2);

  theta1 = theta1*180/PI;
  theta2 = theta2*180/PI;
}

// 60 -> 0.02, 70 -> 0.02, 80 -> 0.02, 90 -> 0.4
void loop() {
  return;
  
  for(float t=0; t<=1; t+=0.05) {
    float x = 8-16*t;
    ik(x,h);

    thigh.write(90+(int)theta1);
    knee.write(90-(int)theta2);
    delay(50);
  }

  for(float t=0; t<=1; t+=0.05) {
    float x = 0-8*cos(t*PI);
    float y = h-8*sin(t*PI);
    ik(x,y);

    thigh.write(90+(int)theta1);
    knee.write(90-(int)theta2);
    delay(50);
  }
}
