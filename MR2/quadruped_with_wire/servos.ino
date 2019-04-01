#include<Servo.h>

/*  Leg order: FL,FR,BR,BL
 *  Servo order: 2n -> thigh, 2n+1 -> knee
 */

const int servo_num = 8;
const int servo_pins[servo_num] = {6,7, 2,3, 4,5, 8,9};
const int servo_bias[servo_num] = {90,90, 90,90, 90,90, 90,90};
const int servo_dir[servo_num] = {-1,1, 1,-1, 1,-1, -1,1};

const int min_freq = 1000;
const int max_freq = 2000;

const Servo servos[servo_num];

void init_servos() {
  for (int i=0; i<servo_num; i++) {
    servos[i].attach(servo_pins[i]);  
  }

  for (int i=0; i<servo_num; i++) {
    servos[i].write(servo_bias[i]);
  }

  delay(3000);
}

void move_servo(int servo, float deg) {
  /*
  float eff_deg = servo_bias[servo] + servo_dir[servo]*deg;
  int freq = min_freq + (int)(eff_deg*(max_freq-min_freq)/(180-0));
  freq = constrain(freq,min_freq,max_freq);
  servos[servo].writeMicroseconds(freq);
  */

  float eff_deg = servo_bias[servo] + servo_dir[servo]*deg;
  servos[servo].write(eff_deg);
}
