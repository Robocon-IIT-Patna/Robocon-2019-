#include <Arduino_FreeRTOS.h>

#define ADA_SERVOS 0
#define WIRE_SERVOS 1
#define ZERO_POS 1

#define WAIT 0
#define REST_POS 0 
#define MARCH 1

#define GRIPPER 0

#define SAND_DUNE 0

extern float x_bias[4];
extern float y_bias[4];
extern float x_frac[4];
extern float y_frac[4];

void setup() {
  Serial.begin(9600);
  init_servos();
}

void loop() {
  
}

void march_forward() {
  start_march();

  for(;;) {
    march(true,true,true,true);
  }
}

void sand_dune() {
  float t1,t2,t3,t4,t5,t6,t7,t8;

  // lift and place 1st leg
  ik(0,20,1,&t1,&t2);
  ik(-20,20,1,&t3,&t4);
  ik(-20,5,1,&t5,&t6);

  move_leg(1,t1,t2); delay(2000);
  move_leg(1,t3,t4); delay(3000);
  move_leg(1,t5,t6); delay(2000);



  // lift and place 2nd leg
  ik(0,20,0,&t1,&t2);
  ik(-20,20,0,&t3,&t4);
  ik(-20,10,0,&t5,&t6);
  
  move_leg(0,t1,t2); delay(2000);
  move_leg(0,t3,t4); delay(3000);
  move_leg(0,t5,t6); delay(2000);
  

  // march over sand dune
  x_bias[0] = -20; y_bias[0] = 5; x_frac[0] = 1; y_frac[0] = 1;
  x_bias[1] = -20; y_bias[1] = 5; x_frac[1] = 1; y_frac[1] = 1;
  
  start_march();

  for(int w=0; w<12*40; w++) {
    march(true,true,true,true);    
  
    if (x_bias[0] < 0)
      x_bias[0] += 0.1;
    
    if (x_bias[1] < 0)
      x_bias[1] += 0.1;
  
  }
  
  zero_pos();
  delay(1500);

  // shift weight forward
  move_leg(0,20,0);
  move_leg(1,20,0);
  move_leg(2,20,0);
  move_leg(3,20,0);

  delay(1000);

  // lift and place 3rd leg
  ik(10,20,2,&t1,&t2);
  ik(-5,25,2,&t3,&t4);
  ik(-5,10,2,&t5,&t6);
  
  move_leg(2,t1,t2); delay(2000);
  move_leg(2,t3,t4); delay(3000);
  move_leg(2,t5,t6); delay(2000);

  

  // lift and place 4th leg
  ik(10,20,3,&t1,&t2);
  ik(-5,25,3,&t3,&t4);
  ik(-5,10,3,&t5,&t6);
  
  move_leg(3,t1,t2); delay(2000);
  move_leg(3,t3,t4); delay(3000);
  move_leg(3,t5,t6); delay(1000);

  // shift weight backward
  ik(0,10,2,&t1,&t2);
  ik(0,10,3,&t3,&t4);
  move_leg(0,10,0);
  move_leg(1,10,0);
  move_leg(2,t1,t2);
  move_leg(3,t3,t4);

  delay(1000);

  // march over sand dune
  x_bias[0] = 0; y_bias[0] = 0;
  x_bias[1] = 0; y_bias[1] = 0.5;
  x_bias[2] = 0; y_bias[2] = 10;
  x_bias[3] = 0; y_bias[3] = 10;
  
  start_march();

  for(int w=0; w<12*40; w++) {
    march(true,true,true,true);
  }

  y_bias[2] = 0;
  y_bias[3] = 0.5;

  for(;;) {}
}
