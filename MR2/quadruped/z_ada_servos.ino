#if ADA_SERVOS
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm;

/*  Leg order: FL,FR,BR,BL
 *  Servo order: 2n -> thigh, 2n+1 -> knee
 */

const int servo_num = 8;
const int servo_pins[servo_num] = {0,1,2,3,4,5,6,7};
const int servo_bias[servo_num] = {75,70, 80,70, 63,100, 87,90};
const int servo_dir[servo_num] = {-1,-1, 1,-1, 1,1, 1,-1};

const int servomin[servo_num] = {130,130,130,130,130,130,210,130};
const int servomax[servo_num] = {580,580,580,580,580,580,480,580};

void init_servos() {
  pwm = Adafruit_PWMServoDriver();
  
  pwm.begin();
  pwm.setPWMFreq(60); // 60 
  delay(500);

  for (int i=0; i<servo_num; i++) {
    move_servo(i,0);
  }

  delay(3000);
}

void move_servo(int servo, float deg) {
  float eff_deg = (int)(servo_bias[servo] + servo_dir[servo]*deg);
  int pulse = map(eff_deg,0,180,servomin[servo],servomax[servo]);
  pulse = constrain(pulse,servomin[servo],servomax[servo]);
  pwm.setPWM(servo_pins[servo],0,pulse);
}
#endif
