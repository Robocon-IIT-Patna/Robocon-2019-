#define ADA_SERVOS 0
#define WIRE_SERVOS 1
#define ZERO_POS 1

#define WAIT 0
#define REST_POS 0 
#define MARCH 0

#define GRIPPER 0

#define SAND_DUNE 1

extern float x_bias[4];
extern float y_bias[4];
extern float x_frac[4];
extern float y_frac[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  init_servos();

  #if GRIPPER
  init_gripper();
  get_gerege();
  #endif

  #if WAIT
  delay(10000);
  #endif

  #if REST_POS
  rest_pos();
  #endif
 
  #if MARCH
  delay(2000);
  start_march();
  int start_time = millis();
  while(millis()-start_time<120000) {
    march(true,true,true,true); 
  }
  stop_march();
  //delay(3000);
  straight_pos();
  #endif

  #if SAND_DUNE
  delay(10000);
  float t1,t2,t3,t4,t5,t6,t7,t8;

  
  ik(0,15,1,&t1,&t2);
  ik(-20,15,1,&t3,&t4);
  ik(-20,5,1,&t5,&t6);

  //Serial.print(String(t1) + " " + String(t2));
  //Serial.print(String(t3) + " " + String(t4));
  //Serial.print(String(t5) + " " + String(t6));
  
  move_leg(1,t1,t2);
  delay(2000);
  move_leg(1,t3,t4);
  delay(3000);
  move_leg(1,t5,t6);
  //delay(2000);
  //move_leg(3,0,0);

  //float t1,t2,t3,t4,t5,t6;
  ik(0,15,0,&t1,&t2);
  ik(-20,15,0,&t3,&t4);
  ik(-20,5,0,&t5,&t6);

  //Serial.print(String(t1) + " " + String(t2));
  //Serial.print(String(t3) + " " + String(t4));
  //Serial.print(String(t5) + " " + String(t6));
  
  move_leg(0,t1,t2);
  delay(2000);
  move_leg(0,t3,t4);
  delay(3000);
  move_leg(0,t5,t6);
  //delay(2000);
  //move_leg(3,0,0);
  

  ik(-20,5,1,&t1,&t2);
  ik(-20,5,0,&t3,&t4);
  move_leg(1,t1,t2);
  move_leg(0,t3,t4);
  move_leg(2,0,0);
  move_leg(3,0,0);

  delay(3000);

  /*
  for(int i=0; i<10; i++) {
    ik(-15+i*15/9,5,0,&t1,&t2);
    ik(-15+i*15/9,5,1,&t3,&t4);
    ik(i*15/9,2,2,&t5,&t6);
    ik(i*15/9,2,3,&t7,&t8);
  
    move_leg(0,t1,t2);
    move_leg(1,t3,t4);
    move_leg(2,t5,t6);
    move_leg(3,t7,t8);

    delay(100);
  }*/

  x_bias[0] = -20; y_bias[0] = 5; x_frac[0] = 1; y_frac[0] = 1;
  x_bias[1] = -20; y_bias[1] = 5; x_frac[1] = 1; y_frac[1] = 1;
  
  start_march();
  while(true)
    march(true,true,true,true);
  
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  //march();
}
