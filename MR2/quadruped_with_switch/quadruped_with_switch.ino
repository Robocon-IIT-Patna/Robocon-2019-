#define ADA_SERVOS 0
#define WIRE_SERVOS 1
#define ZERO_POS 1

#define WAIT 0
#define REST_POS 0

#define GRIPPER 1

extern float x_bias[4];
extern float y_bias[4];
extern float x_frac[4];
extern float y_frac[4];
extern int path_delay;
extern int path_res;

const int switchApin = 19;
const int switchBpin = 20;

const int marchledpin = 11;
const int sandduneledpin = 10;
const int ropeledpin = 13;
const int climbledpin = 12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  init_servos();
  
  #if WAIT
  delay(10000);
  #endif

  #if REST_POS  
  rest_pos();
  #endif

//  #if GRIPPER
//  init_gripper();
//  get_gerege();
//  #endif
  //init_gripper();
  
  pinMode(switchApin,INPUT_PULLUP);
  pinMode(switchBpin,INPUT_PULLUP);
  
  pinMode(marchledpin, OUTPUT);
  pinMode(sandduneledpin, OUTPUT);
  pinMode(ropeledpin, OUTPUT);
  pinMode(climbledpin, OUTPUT);

  bool switchA = digitalRead(switchApin);
  bool switchB = digitalRead(switchBpin);
  
  if (!switchA && !switchB)
    march_forward();
  else if (!switchA && switchB)
    sand_dune();
  else if (switchA && !switchB)
    rope();
  else if (switchA && switchB)
    climb();
  // march_forward();
  
}

void march_forward() {
  Serial.println("March");
  
  analogWrite(marchledpin,100);
  digitalWrite(sandduneledpin,LOW);
  digitalWrite(ropeledpin,LOW);
  digitalWrite(climbledpin,LOW);
  
  delay(2000);
  
  #if GRIPPER
  init_gripper();
  get_gerege();
  #endif
  
  start_march();
  
  while(true) {
    march(true,true,true,true); 
  }
}

void sand_dune() {
  //Serial.println("Sand dune");

  digitalWrite(marchledpin,LOW);
  analogWrite(sandduneledpin,100);
  digitalWrite(ropeledpin,LOW);
  digitalWrite(climbledpin,LOW);

  close_gripper();
  delay(5000);

  start_march();
  for(int w=0; w<8*40; w++)
    march(true,true,true,true);
    
  zero_pos();
  delay(500);
  
  float t1,t2,t3,t4,t5,t6,t7,t8;


  // lift and place 1st leg
  ik(0,20,1,&t1,&t2);
  ik(-20,20,1,&t3,&t4);
  ik(-20,5,1,&t5,&t6);

  move_leg(1,t1,t2); delay(500);
  move_leg(1,t3,t4); delay(500);
  move_leg(1,t5,t6); delay(500);
  

  // lift and place 2nd leg
  ik(0,20,0,&t1,&t2);
  ik(-20,20,0,&t3,&t4);
  ik(-20,10,0,&t5,&t6);
  
  move_leg(0,t1,t2); delay(500);
  move_leg(0,t3,t4); delay(500);
  move_leg(0,t5,t6); delay(500);
  
//
//  ik(-20,5,1,&t1,&t2);
//  ik(-20,5,0,&t3,&t4);
//  move_leg(1,t1,t2);
//  move_leg(0,t3,t4);
//  move_leg(2,0,0);
//  move_leg(3,0,0);
//
//  delay(3000);


  // march over sand dune

  path_delay = 1500/path_res;
  x_bias[0] = -20; y_bias[0] = 5; x_frac[0] = 1; y_frac[0] = 1;
  x_bias[1] = -20; y_bias[1] = 5; x_frac[1] = 1; y_frac[1] = 1;
  
  start_march();

  for(int w=0; w<8*40; w++) {
    march(true,true,true,true);
    
    if (x_bias[0] < 0)
      x_bias[0] += 0.1;
    
    if (x_bias[1] < 0)
      x_bias[1] += 0.1;
  }
  
  zero_pos();
  delay(1000);


  // shift weight forward
  move_leg(0,15,0);
  move_leg(1,15,0);
  move_leg(2,15,0);
  move_leg(3,15,0);

  delay(1000);


  // lift and place 3rd leg
  ik(10,20,2,&t1,&t2);
  ik(-5,25,2,&t3,&t4);
  ik(-5,10,2,&t5,&t6);

  move_leg(2,t1,t2); delay(500);
  move_leg(2,t3,t4); delay(500);
  move_leg(2,t5,t6); delay(500);


  
  // lift and place 4th leg
  ik(10,20,3,&t1,&t2);
  ik(-5,25,3,&t3,&t4);
  ik(-5,10,3,&t5,&t6);
  
  move_leg(3,t1,t2); delay(500);
  move_leg(3,t3,t4); delay(500);
  move_leg(3,t5,t6); delay(500);


  // shift weight backward
  ik(0,10,2,&t1,&t2);
  ik(0,10,3,&t3,&t4);
  move_leg(0,10,0);
  move_leg(1,10,0);
  move_leg(2,t1,t2);
  move_leg(3,t3,t4);

  delay(1000);


  // march over sand dune
  x_bias[0] = 0; y_bias[1] = 0;
  x_bias[1] = 0; x_bias[1] = 0.5;
  x_bias[2] = 0; y_bias[2] = 10;
  x_bias[3] = 0; y_bias[3] = 10;
  path_delay = 700/path_res;
  
  start_march();

  for(int w=0; w<8*40; w++) {
    march(true,true,true,true);
  }

  while(true) {
    if (y_bias[2] > 0)
      y_bias[2] -= 0.1;

    if (y_bias[3] > 0.5)
      y_bias[3] -= 0.1;
  }
}

void rope() {
  digitalWrite(marchledpin,LOW);
  digitalWrite(sandduneledpin,LOW);
  analogWrite(ropeledpin,100);
  digitalWrite(climbledpin,LOW);

  start_march();
  for(int w=0; w<8*40; w++)
    march(true,true,true,true);
    
  zero_pos();
  delay(500);
  
  float t1,t2,t3,t4,t5,t6,t7,t8;


  // lift and place 1st leg
  ik(0,20,1,&t1,&t2);
  ik(-20,20,1,&t3,&t4);
  ik(-20,2,1,&t5,&t6);

  move_leg(1,t1,t2); delay(500);
  move_leg(1,t3,t4); delay(500);
  move_leg(1,t5,t6); delay(500);
  

  // lift and place 2nd leg
  ik(0,20,0,&t1,&t2);
  ik(-20,20,0,&t3,&t4);
  ik(-20,2,0,&t5,&t6);
  
  move_leg(0,t1,t2); delay(500);
  move_leg(0,t3,t4); delay(500);
  move_leg(0,t5,t6); delay(500);
  
//
//  ik(-20,5,1,&t1,&t2);
//  ik(-20,5,0,&t3,&t4);
//  move_leg(1,t1,t2);
//  move_leg(0,t3,t4);
//  move_leg(2,0,0);
//  move_leg(3,0,0);
//
//  delay(3000);


  // march over sand dune

  path_delay = 1200/path_res;
  x_bias[0] = -20; y_bias[0] = 2; x_frac[0] = 1; y_frac[0] = 1;
  x_bias[1] = -20; y_bias[1] = 2; x_frac[1] = 1; y_frac[1] = 1;
  
  start_march();

  for(int w=0; w<8*40; w++) {
    march(true,true,true,true);
    
    if (x_bias[0] < 0)
      x_bias[0] += 0.1;
    
    if (x_bias[1] < 0)
      x_bias[1] += 0.1;
  }
  
  zero_pos();
  delay(1000);


  // shift weight forward
  move_leg(0,15,0);
  move_leg(1,15,0);
  move_leg(2,15,0);
  move_leg(3,15,0);

  delay(1000);


  // lift and place 3rd leg
  ik(10,20,2,&t1,&t2);
  ik(-5,25,2,&t3,&t4);
  ik(-5,2,2,&t5,&t6);

  move_leg(2,t1,t2); delay(500);
  move_leg(2,t3,t4); delay(500);
  move_leg(2,t5,t6); delay(500);


  
  // lift and place 4th leg
  ik(10,20,3,&t1,&t2);
  ik(-5,25,3,&t3,&t4);
  ik(-5,2,3,&t5,&t6);
  
  move_leg(3,t1,t2); delay(500);
  move_leg(3,t3,t4); delay(500);
  move_leg(3,t5,t6); delay(500);


  // shift weight backward
  ik(0,10,2,&t1,&t2);
  ik(0,10,3,&t3,&t4);
  move_leg(0,10,0);
  move_leg(1,10,0);
  move_leg(2,t1,t2);
  move_leg(3,t3,t4);

  delay(1000);


  // march over sand dune
  x_bias[0] = 0; y_bias[1] = 0;
  x_bias[1] = 0; x_bias[1] = 0.5;
  x_bias[2] = 0; y_bias[2] = 10;
  x_bias[3] = 0; y_bias[3] = 10;
  path_delay = 700/path_res;
  
  start_march();

  for(int w=0; w<8*40; w++) {
    march(true,true,true,true);
  }

  for(int w=0; w<8*40; w++) {
    if (y_bias[2] > 0)
      y_bias[2] -= 0.1;

    if (y_bias[3] > 0.5)
      y_bias[3] -= 0.1;
  }
}

void climb() {
  digitalWrite(marchledpin,LOW);
  digitalWrite(sandduneledpin,LOW);
  digitalWrite(ropeledpin,LOW);
  analogWrite(climbledpin,100);
  close_gripper();
  up_gripper();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //measure_dist();
}
