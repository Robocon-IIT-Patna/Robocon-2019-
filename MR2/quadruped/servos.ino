#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//good code Party Do 

Adafruit_PWMServoDriver pwm;

/*  Leg order: FL,FR,BR,BL
 *  Servo order: 2n -> thigh, 2n+1 -> knee
 */

const int servo_num = 8;
const int servo_pins[servo_num] = {0,1,2,3,4,5,6,7};
const int servo_bias[servo_num] = {70,90, 90,105, 80,65, 90,70};
const int servo_dir[servo_num] = {1,-1, 1,-1, 1,-1, -1,1};

const int servomin = 130;
const int servomax = 580;

void init_servos() {
  pwm = Adafruit_PWMServoDriver();
  
  pwm.begin();
  pwm.setPWMFreq(60); // 60 Hz
  delay(500);

  for (int i=0; i<servo_num; i++) {
    move_servo(i,0);
  }

  delay(3000);
}

void move_servo(int servo, float deg) {
  float eff_deg = (int)(servo_bias[servo] + servo_dir[servo]*deg);
  int pulse = map(eff_deg,0,180,servomin,servomax);
  pwm.setPWM(servo_pins[servo],0,pulse);
}
