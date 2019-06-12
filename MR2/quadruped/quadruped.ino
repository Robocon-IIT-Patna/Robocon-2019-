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

  /*
  ik(0,20,1,&t1,&t2);
  ik(-20,20,1,&t3,&t4);
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
  ik(0,20,0,&t1,&t2);
  ik(-20,20,0,&t3,&t4);
  ik(-20,10,0,&t5,&t6);

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

  x_bias[0] = -20; y_bias[0] = 5; x_frac[0] = 1; y_frac[0] = 1;
  x_bias[1] = -20; y_bias[1] = 5; x_frac[1] = 1; y_frac[1] = 1;
  
  start_march();

  for(int w=0; w<12*40; w++) {
    march(true,true,true,true);
    
    //if (w > 8*40) {
      if (x_bias[0] < 0)
        x_bias[0] += 0.1;
      
      if (x_bias[1] < 0)
        x_bias[1] += 0.1;
    //}
  }*/
  

  
  zero_pos();
  delay(1500);

  move_leg(0,20,0);
  move_leg(1,20,0);
  move_leg(2,20,0);
  move_leg(3,20,0);
  
  /*
  for(int i=0; i<10; i++) {
    ik(-20+i*20/9,5,0,&t1,&t2);
    ik(-20+i*20/9,5,1,&t3,&t4);
    ik(i*20/9,2,2,&t5,&t6);
    ik(i*20/9,2,3,&t7,&t8);
  
    move_leg(0,t1,t2);
    move_leg(1,t3,t4);
    move_leg(2,t5,t6);
    move_leg(3,t7,t8);
    delay(100);
  }*/

  delay(1000);
  ik(10,20,2,&t1,&t2);
  ik(-5,25,2,&t3,&t4);
  ik(-5,10,2,&t5,&t6);

  //Serial.print(String(t1) + " " + String(t2));
  //Serial.print(String(t3) + " " + String(t4));
  //Serial.print(String(t5) + " " + String(t6));
  
  move_leg(2,t1,t2);
  delay(2000);
  move_leg(2,t3,t4);
  delay(3000);
  move_leg(2,t5,t6);
  //delay(2000);
  //move_leg(3,0,0);

  delay(2000);

 // move_leg(0,20,0);
 // move_leg(1,20,1);
 // delay(1000);
  
  //float t1,t2,t3,t4,t5,t6;
  ik(10,20,3,&t1,&t2);
  ik(-5,25,3,&t3,&t4);
  ik(-5,10,3,&t5,&t6);

  //Serial.print(String(t1) + " " + String(t2));
  //Serial.print(String(t3) + " " + String(t4));
  //Serial.print(String(t5) + " " + String(t6));
  
  move_leg(3,t1,t2);
  delay(2000);
  move_leg(3,t3,t4);
  delay(3000);
  move_leg(3,t5,t6);

  delay(1000);

  ik(0,10,2,&t1,&t2);
  ik(0,10,3,&t3,&t4);
  move_leg(0,10,0);
  move_leg(1,10,0);
  move_leg(2,t1,t2);
  move_leg(3,t3,t4);

  delay(1000);
  
  x_bias[2] = 0; y_bias[2] = 10;
  x_bias[3] = 0; y_bias[3] = 10;
  
  start_march();

  for(int w=0; w<12*40; w++) {
    march(true,true,true,true);
  }
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  //march();
}
