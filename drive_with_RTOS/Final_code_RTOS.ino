#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

/* --- State Variables --- */
// LSA
int lsaF=0,lsaB=0,lsaR=0,lsaL=0;
int vlsaF,vlsaB,vlsaR,vlsaL;

// PS2
//int joy1x,joy1y,joy2x,joy2y;
int jxpwm=0,jypwm=0;
int triangle=0,cross=0; // put other buttons here

// Autopilot
int autopilot=0;
int dir=0;
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
const int LOW_PWM = 80
;

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
    jxpwm = (ps2.readButton(PS2_JOYSTICK_LEFT_X_AXIS)-128)*2;
    jypwm = (ps2.readButton(PS2_JOYSTICK_RIGHT_Y_AXIS)-128)*2;
    jxpwm = constrain(jxpwm,-255,255);
    jypwm = constrain(jypwm,-255,255);
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
  dir=1;
  if (manual_process != NULL)
    vTaskSuspend(manual_process);
  autopilot = 1;
  vTaskResume(automatic_process);
  
  for(;;) {
    vlsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
    vlsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
    vlsaR = (int)((float)analogRead(analogPinR)/921*70); //right
    vlsaL = (int)((float)analogRead(analogPinL)/921*70); //left

    lsaF = ((vlsaF>=0) && (vlsaF<70))?1:0;
    lsaB = ((vlsaB>=0) && (vlsaB<70))?1:0;
    lsaR = ((vlsaR>=0) && (vlsaR<70))?1:0;
    lsaL = ((vlsaL>=0) && (vlsaL<70))?1:0;
        
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

    fpwm = jxpwm; bpwm = jxpwm;
    rpwm = jypwm; lpwm = jypwm;
  }
}

void automatic(void *params) {
    for(;;) {
      vTaskDelay(1);

      if (autopilot == 0) // autopilot is disabled
        continue; // TODO: disable autopilot and destroy task
        
      if (lsaF+lsaB+lsaR+lsaL == 0) { // no line detected. so stop.
        //dir = 0; // TODO: disable autopilot and destroy task
        continue;
      }

      if (dir < 0 || dir > 4) { // invalid dir
        dir = 0; // TODO: disable autopilot and destroy task
        continue;
      }

      // set pwms
      if (dir == 1) {
        rpwm = HIGH_PWM; lpwm = HIGH_PWM;
      } else if (dir == 2) {
        fpwm = HIGH_PWM; bpwm = HIGH_PWM;
      } else if (dir == 3) {
        rpwm = -HIGH_PWM; lpwm = -HIGH_PWM;
      } else if (dir == 4) {
        fpwm = -HIGH_PWM; bpwm = -HIGH_PWM;
      }

      // align with line
      if (dir==1 || dir==3) {
        if (lsaF) {
          if (vlsaF <= 18)
            fpwm = -LOW_PWM;
          else if (vlsaF >= 53)
            fpwm = LOW_PWM;
          else
            fpwm = 0;
        } else {
            fpwm = 0;
        }

        if (lsaB) {
          if (vlsaB <= 18)
            bpwm = LOW_PWM;
          else if (vlsaB >= 53)
            bpwm = -LOW_PWM;
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

      // decide direction
      if (lsaF+lsaB+lsaR+lsaL != 2) // multiple or ambiguous paths. continue.
        continue;

      if (dir == 0) // bot should be stopped
        continue;

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

     analogWrite(fwpwm,ufpwm);
     analogWrite(bwpwm,ubpwm);
     analogWrite(rwpwm,urpwm);
     analogWrite(lwpwm,ulpwm);
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
