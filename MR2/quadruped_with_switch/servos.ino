#if WIRE_SERVOS
#include<Servo.h>

/*  Leg order: FL,FR,BR,BL
 *  Servo order: 2n -> thigh, 2n+1 -> knee
 */

const int servo_num = 8;
const int servo_pins[servo_num] = {2,3, 4,5, 6,7, 8,9};
const int servo_bias[servo_num] = {82,165, 110,30, 87,40, 87,130};
const int servo_dir[servo_num] = {-1,1, 1,-1, 1,-1, -1,1};

Servo servos[servo_num];

void init_servos() {
  for (int i=0; i<servo_num; i++) {
    servos[i].attach(servo_pins[i]);
  }

  /*
  for (int i=0; i<servo_num; i++) {
    move_servo(i,0);
  }*/

  #if ZERO_POS
  zero_pos();
  #else
  straight_pos();
  #endif

  //delay(3000);
}

void zero_pos() {
  for (int i=0; i<servo_num; i++) {
    move_servo(i,0);
  }
}

void move_servo(int servo, float deg) {
  int eff_deg = (int)(servo_bias[servo] + servo_dir[servo]*deg);
  eff_deg = constrain(eff_deg,0,180);
  servos[servo].write(eff_deg);
}
#endif
