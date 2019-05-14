#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

/* --- State Variables --- */
// LSA
int lsaF=0,lsaB=0,lsaR=0,lsaL=0;
int vlsaF,vlsaB,vlsaR,vlsaL;

// PS2
//int joy1x,joy1y,joy2x,joy2y;
int jxpwm=0,jypwm=0,rot=0;
int triangle=0,cross=0,circle=0,sqr=0,right_1=0,right_2=0, up = 0; // put other buttons here
int rotatearm = 0;
int high_speed = false;

// Autopilot
int autopilot=0;
int dir=0;
int pwm_set=0;
TaskHandle_t automatic_process = NULL;
TaskHandle_t manual_process = NULL;

// Actuation
int fpwm=0,bpwm=0,rpwm=0,lpwm=0;
int ufpwm,ubpwm,urpwm,ulpwm;
int dfpwm,dbpwm,drpwm,dlpwm;
//int _ufpwm,_ubpwm,_urpwm,_ulpwm;
//int _dfpwm,_dbpwm,_drpwm,_dlpwm;

/* --- End of State Variables --- */

/* -- Constants --- */
const int HIGH_PWM = 80;
const int LOW_PWM = 30;
const int ROT_PWM = 25;

const int MAN_LOW_PWM = 50;
const int MAN_HIGH_PWM = 125;

/* --- Pins --- */
const int fwdir = 4;
const int fwpwm = 5;
const int bwdir = 2;
const int bwpwm = 3;
const int lwdir = 9;
const int lwpwm = 10;
const int rwdir = 7;
const int rwpwm = 8;

const int analogPinF = A7;
const int analogPinB = A2;
const int analogPinL = A0;
const int analogPinR = A4;

const int thro = 29;
int prev_thro = 0;
const int open = 31;
int prev_open = 0;
const int grip = 27;
int prev_grip = 0;
int thro_toggle = 0;//0 = closed  , 1 = open
int open_toggle = 0;
int grip_toggle = 0;

const int armdir = 37;
const int armpwm = 39;
/* -- End of pins --- */

/* --- Processes --- */
void manual(void *);
void automatic(void *);
void actuate(void *);
void master(void *);
void telemetry(void *);

// TODO: Add master process

void setup() {
  //xTaskCreate(telemetry,"telemetry",100,NULL,2,NULL);
  xTaskCreate(actuate,"actuate",100,NULL,2,NULL);
  xTaskCreate(master,"master",100,NULL,2,NULL);
  xTaskCreate(manual,"manual",100,NULL,2,&manual_process);
  xTaskCreate(automatic,"automatic",100,NULL,2,&automatic_process);
  vTaskSuspend(manual_process);
  vTaskSuspend(automatic_process);
}

void loop() {
  // everything is handled by RTOS
}

// Actual master
/*
void master(void *params) {
  
  // subtract ps2 values from 1 so that 0 = pressed and 1 = released
  //Cytron_PS2Shield ps2(0, 1);
  //ps2.begin(9600);
  /*  left joystick -> left-right
   *  right joystick -> forward-backward
  for(;;) {
    // Sensors
    
    // LSA
    vlsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
    vlsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
    vlsaR = (int)((float)analogRead(analogPinR)/921*70); //right
    vlsaL = (int)((float)analogRead(analogPinL)/921*70); //left
    lsaF = ((lsaF>=0) && (lsaF<70))?1:0;
    lsaB = ((lsaB>=0) && (lsaB<70))?1:0;
    lsaR = ((lsaR>=0) && (lsaR<70))?1:0;
    lsaL = ((lsaL>=0) && (lsaL<70))?1:0;
    // PS2
    /* TODO: Uuncomment PS2
    jxpwm = (ps2.readButton(PS2_JOYSTICK_RIGHT_X_AXIS)-128)*2;
    jypwm = -(ps2.readButton(PS2_JOYSTICK_LEFT_Y_AXIS)-128)*2;
    jxpwm = constrain(jxpwm,-255,255);
    jypwm = constrain(jypwm,-255,255);
    rot = ps2.readButton(PS2_JOYSTICK_LEFT)-ps2.readButton(PS2_JOYSTICK_RIGHT);
    triangle = 1-ps2.readButton(PS2_TRIANGLE);
    cross = 1-ps2.readButton(PS2_CROSS);
    
    // Decisions
    if (triangle)
      enable_autopilot();
    if (cross)
      disable_autopilot();
      
    56vTaskDelay(50/portTICK_PERIOD_MS);
  }
  
}*/

// Debug master
void master(void *params) {
  // TODO: CONTINUE HERE

  Cytron_PS2Shield ps2(0,1);
  ps2.begin(9600);
  disable_autopilot();
  digitalWrite(thro,LOW);
  digitalWrite(open,LOW);
  digitalWrite(grip,LOW);
  
  for(;;) {
    vlsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
    vlsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
    vlsaR = (int)((float)analogRead(analogPinR)/921*70); //right
    vlsaL = (int)((float)analogRead(analogPinL)/921*70); //left

    lsaF = ((vlsaF>=0) && (vlsaF<70))?1:0;
    lsaB = ((vlsaB>=0) && (vlsaB<70))?1:0;
    lsaR = ((vlsaR>=0) && (vlsaR<70))?1:0;
    lsaL = ((vlsaL>=0) && (vlsaL<70))?1:0;

    
    jxpwm = (ps2.readButton(PS2_JOYSTICK_RIGHT_X_AXIS)-128);
    jypwm = -(ps2.readButton(PS2_JOYSTICK_LEFT_Y_AXIS)-128);

    if (!autopilot) {
      if (high_speed) {
        jxpwm = map(jxpwm,-128,128,-MAN_HIGH_PWM,MAN_HIGH_PWM);
        jypwm = map(jypwm,-128,128,-MAN_HIGH_PWM,MAN_HIGH_PWM);
      } else {
        jxpwm = map(jxpwm,-128,128,-MAN_LOW_PWM,MAN_LOW_PWM);
        jypwm = map(jypwm,-128,128,-MAN_LOW_PWM,MAN_LOW_PWM);
      }
    } else {
      jypwm = map(jypwm,-128,128,-HIGH_PWM,HIGH_PWM);
    }
    
    jxpwm = constrain(jxpwm,-255,255);
    jypwm = constrain(jypwm,-255,255);

    rot = ps2.readButton(PS2_JOYSTICK_LEFT)-ps2.readButton(PS2_JOYSTICK_RIGHT);

    triangle = 1-ps2.readButton(PS2_TRIANGLE);
    cross = 1-ps2.readButton(PS2_CROSS);
    circle = 1-ps2.readButton(PS2_CIRCLE);
    sqr = 1-ps2.readButton(PS2_SQUARE);
    right_1 = 1-ps2.readButton(PS2_RIGHT_1);
    right_2 = 1-ps2.readButton(PS2_RIGHT_2);
    up = 1-ps2.readButton(PS2_UP);

    if (1-ps2.readButton(PS2_LEFT_2))
      high_speed = false;
    else if (1-ps2.readButton(PS2_LEFT_1))
      high_speed = true;

    if (triangle && cross)
      triangle = 0;

    // Decisions
    if (triangle)
      enable_autopilot();

    if (cross)
      disable_autopilot();

    if (right_1 && right_2)
      rotatearm = 0;
    else if (right_1)
      rotatearm = 1;
    else if (right_2)
      rotatearm = -1;
    else
      rotatearm = 0;

    
    vTaskDelay(1);
  }
}

void enable_autopilot() {
  dir = 0;
  
  if (lsaF && lsaB && abs(jypwm)>20)
    dir = (jypwm >= 0)?1:3;
  else if (lsaR && lsaL && abs(jxpwm)>20)
    dir = (jxpwm >= 0)?2:4;

  if (dir == 0) {
    disable_autopilot();
    return;
  }

  if (manual_process != NULL)
    vTaskSuspend(manual_process);
  autopilot = 1;
  vTaskResume(automatic_process);
}

void disable_autopilot() {
  if (automatic_process != NULL)
    vTaskSuspend(automatic_process);
  autopilot = 0;
  vTaskResume(manual_process);
}

void manual(void *params) {
  for(;;) {
    vTaskDelay(1);
    
    if (autopilot == 1) // autopilot is enabled
      continue; // TODO: enable autopilot and destroy task

    if (rot == 0) {
      fpwm = jxpwm; bpwm = jxpwm;
      rpwm = jypwm; lpwm = jypwm;
    } else {
      if (rot == 1) {
        fpwm = -LOW_PWM; bpwm = LOW_PWM;
        rpwm = -LOW_PWM; lpwm = LOW_PWM;
      } else if (rot == -1) {
        fpwm = LOW_PWM; bpwm = -LOW_PWM;
        rpwm = LOW_PWM; lpwm = -LOW_PWM;
      }
    }
  }
}

void automatic(void *params) {
    for(;;) {
      vTaskDelay(1);

      if (autopilot == 0) // autopilot is disabled
        continue; // TODO: disable autopilot and destroy task
        
      if (lsaF+lsaB+lsaR+lsaL == 0) { // no line detected. so stop.
        //dir = 0; // TODO: disable autopilot and destroy task
        pwm_set = 0;
        disable_autopilot();
        continue;
      }

      if (dir < 0 || dir > 4) { // invalid dir
        dir = 0; // TODO: disable autopilot and destroy task
        disable_autopilot();
        continue;
      }

      // set pwms
      
      if (dir == 1) {
        rpwm = pwm_set; lpwm = pwm_set;
      } else if (dir == 2) {
        fpwm = pwm_set; bpwm = pwm_set;
      } else if (dir == 3) {
        rpwm = -pwm_set; lpwm = -pwm_set;
      } else if (dir == 4) {
        fpwm = -pwm_set; bpwm = -pwm_set;
      }
      //rpwm = 0; fpwm = 0; lpwm = 0; bpwm = 0;

      // align with line
      if (dir==1 || dir==3) {
        if (lsaB) {
          if (vlsaB <= 18)
            fpwm = LOW_PWM;
          else if (vlsaB >= 53)
            fpwm = -LOW_PWM;
          else
            fpwm = 0;
        } else {
            fpwm = 0;
        }

        if (lsaF) {
          if (vlsaF <= 18)
            bpwm = -LOW_PWM;
          else if (vlsaF >= 53)
            bpwm = LOW_PWM;
          else
            bpwm = 0;
        } else {
          bpwm = 0;
        }
      }

      if (dir==2 || dir==4) {
        if (lsaR) {
          if (vlsaR <= 18)
            rpwm = LOW_PWM;
          else if (vlsaR >= 53)
            rpwm = -LOW_PWM;
          else
            rpwm = 0;
        } else {
          rpwm = 0;
        }

        if (lsaL) {
          if (vlsaL <= 18)
            lpwm = -LOW_PWM;
          else if (vlsaL >= 53)
            lpwm = LOW_PWM;
          else
            lpwm = 0;  
        } else {
          lpwm = 0;
        }
      }

      if ((lsaF+lsaB+lsaR+lsaL == 0) || (dir == 0)) {
        dir = 0; pwm_set = 0;
        disable_autopilot();
        continue;
      }

      if (lsaF+lsaB+lsaR+lsaL == 1) {
          /*
          if (dir == 1 && lsaB) {
            pwm_set = 25;
          } else if (dir == 1 && lsaF) {
            pwm_set = LOW_PWM;
          } else if (dir == 2 && lsaR) {
            pwm_set = LOW_PWM;
          } else if (dir == 2 && lsaL) {
            pwm_set = 25;
          } else if (dir == 3 && lsaF) {
            pwm_set = 25;
          } else if (dir == 3 && lsaB) {
            pwm_set = LOW_PWM;
          } else if (dir == 4 && lsaR) {
            pwm_set = 25;
          } else if (dir == 4 && lsaL) {
            pwm_set = LOW_PWM;
          }*/
          pwm_set = LOW_PWM;
          continue;
      }
      
      // decide direction
      if (lsaF+lsaB+lsaR+lsaL > 2) // multiple or ambiguous paths. continue.
        continue;

      pwm_set = HIGH_PWM;
      
      if (lsaF && lsaR) {
        if (dir == 1);
        else if (dir == 2);
        else if (dir == 3) dir = 2;
        else if (dir == 4) dir = 1;
      }

      if (lsaF && lsaL) {
        if (dir == 1);
        else if (dir == 2) dir = 1;
        else if (dir == 3) dir = 4;
        else if (dir == 4);
      }

      if (lsaB && lsaR) {
        if (dir == 1) dir = 2;
        else if (dir == 2);
        else if (dir == 3);
        else if (dir == 4) dir = 3;
      }

      if (lsaB && lsaL) {
        if (dir == 1) dir = 4;
        else if (dir == 2) dir = 3;
        else if (dir == 3);
        else if (dir == 4);
      }
    }
}

void actuate(void *params) {
  pinMode(fwdir,OUTPUT);
  pinMode(fwdir,OUTPUT);
  pinMode(fwdir,OUTPUT);
  pinMode(fwdir,OUTPUT);
  
  pinMode(fwpwm,OUTPUT);
  pinMode(fwpwm,OUTPUT);
  pinMode(fwpwm,OUTPUT);
  pinMode(fwpwm,OUTPUT);

  pinMode(thro,OUTPUT);
  pinMode(open,OUTPUT);
  pinMode(grip,OUTPUT);

  pinMode(armdir,OUTPUT);
  pinMode(armpwm,OUTPUT);
  
  for(;;) {
     vTaskDelay(1);
     
     dfpwm = (fpwm>=0)?1:0;
     dbpwm = (bpwm>=0)?1:0;
     drpwm = (rpwm>=0)?1:0;
     dlpwm = (lpwm>=0)?1:0;
    
     ufpwm = abs(fpwm);
     ubpwm = abs(bpwm);
     urpwm = abs(rpwm);
     ulpwm = abs(lpwm);

     ufpwm = (ufpwm<20)?0:ufpwm;
     ubpwm = (ubpwm<20)?0:ubpwm;
     urpwm = (urpwm<20)?0:urpwm;
     ulpwm = (ulpwm<20)?0:ulpwm;
    
     digitalWrite(fwdir,dfpwm);
     digitalWrite(bwdir,dbpwm);
     digitalWrite(rwdir,drpwm);
     digitalWrite(lwdir,dlpwm);

     analogWrite(fwpwm,ufpwm*75/100);
     analogWrite(bwpwm,ubpwm);
     analogWrite(rwpwm,urpwm*75/100);
     analogWrite(lwpwm,ulpwm);

     if (circle==1&&thro_toggle==0&&prev_thro == 0){
      thro_toggle = 1;
      prev_thro = 1;
     }
     else if(circle == 0)
       prev_thro = 0;
     
    else if(circle==1&&thro_toggle==1 && prev_thro == 0){
       thro_toggle = 0;
       prev_thro = 1;
       }


    if(thro_toggle == 1)
       digitalWrite(thro , HIGH);
    else if (thro_toggle == 0)
       digitalWrite(thro , LOW);   
     
       

    if (sqr==1&&open_toggle==0&&prev_open == 0){
      open_toggle = 1;
      prev_open = 1;
     }
     else if(sqr == 0)
       prev_open = 0;
     
    else if(sqr==1&&open_toggle==1 && prev_open == 0){
       open_toggle = 0;
       prev_open = 1;
       }

    if(open_toggle == 1)
       digitalWrite(open , HIGH);
    else if (open_toggle == 0)
       digitalWrite(open , LOW);    

    if (up==1&&grip_toggle==0&&prev_grip == 0){
      grip_toggle = 1;
      prev_grip = 1;
     }
     else if(up == 0)
       prev_grip = 0;
     
    else if(up==1&&grip_toggle==1 && prev_grip == 0){
       grip_toggle = 0;
       prev_grip = 1;
       }   


    if(grip_toggle == 1)
       digitalWrite(grip , HIGH);
    else if (open_toggle == 0)
       digitalWrite(grip , LOW);  
      

    if (rotatearm == 1) {
      digitalWrite(armdir,LOW);
      digitalWrite(armpwm,HIGH);
    } else if (rotatearm == 0) {
      digitalWrite(armdir,LOW);
      digitalWrite(armpwm,LOW);
    } else if (rotatearm == -1) {
      digitalWrite(armdir,HIGH);
      digitalWrite(armpwm,HIGH);
    }
  }
}

void telemetry(void *params) {
  Serial.begin(9600);

  /*
  for(;;) {
    Serial.print(fpwm);
    Serial.print("\t");
    Serial.print(bpwm);
    Serial.print("\t");  
    Serial.print(rpwm);
    Serial.print("\t");  
    Serial.print(lpwm);
    Serial.println();
    vTaskDelay(200/portTICK_PERIOD_MS);
  }*/

  
  for(;;) {
    
    Serial.print(vlsaF);
    Serial.print("\t");
    Serial.print(vlsaB);
    Serial.print("\t");
    Serial.print(vlsaR);
    Serial.print("\t");
    Serial.print(vlsaL);
    Serial.print("\t");

    if (dir == 0)
      Serial.print("Stopped");
    else if (dir == 1)
      Serial.print("Forward");
    else if (dir == 2)
      Serial.print("Right");
    else if (dir == 3)
      Serial.print("Backward");
    else if (dir == 4)
      Serial.print("Left");
    else
      Serial.print("Invalid");
    
    Serial.println();

    vTaskDelay(200/portTICK_PERIOD_MS);
  }

  /*
  for(;;) {
    Serial.print(ufpwm);
    Serial.print("\t");
    Serial.print(ubpwm);
    Serial.print("\t");
    Serial.print(urpwm);
    Serial.print("\t");
    Serial.print(ulpwm);
    Serial.println();
    vTaskDelay(500/portTICK_PERIOD_MS);
  }*/
}
