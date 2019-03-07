#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

/* --- State Variables --- */
// LSA
int lsaF=0,lsaB=0,lsaR=0,lsaL=0;
int vlsaF,vlsaB,vlsaR,vlsaL;

// PS2
int jxpwm=0,jypwm=0,rot=0;
int triangle=0, cross=0, lane_follow=0;

// Actuation
int fpwm=0,bpwm=0,rpwm=0,lpwm=0;
int ufpwm,ubpwm,urpwm,ulpwm;
int dfpwm,dbpwm,drpwm,dlpwm;
//int _ufpwm,_ubpwm,_urpwm,_ulpwm;
//int _dfpwm,_dbpwm,_drpwm,_dlpwm;

/* --- End of State Variables --- */

/* -- Constants --- */
const int HIGH_PWM = 80; //TODO: double
const int LOW_PWM = 30;

/* --- Pins --- */
const int fwdir = 2;
const int fwpwm = 3;
const int bwdir = 4;
const int bwpwm = 5;
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
void actuate(void *);
void master(void *);
void telemetry(void *);

// TODO: Add master process

void setup() {
  //xTaskCreate(telemetry,"telemetry",100,NULL,2,NULL);
  xTaskCreate(actuate,"actuate",100,NULL,2,NULL);
  xTaskCreate(master,"master",100,NULL,2,NULL);
}

void loop() {
  // everything is handled by RTOS
}

// Debug master
void master(void *params) {
  // TODO: CONTINUE HERE
  Cytron_PS2Shield ps2(0,1);
  ps2.begin(9600);
  pinMode(25,OUTPUT);
  digitalWrite(25,LOW);
  
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
    jxpwm = constrain(jxpwm,-50,50);
    jypwm = constrain(jypwm,-50,50);

    rot = ps2.readButton(PS2_JOYSTICK_LEFT)-ps2.readButton(PS2_JOYSTICK_RIGHT);

    triangle = 1-ps2.readButton(PS2_TRIANGLE);
    cross = 1-ps2.readButton(PS2_CROSS);

    if (triangle && cross)
      triangle = 0;

    if (triangle)
      lane_follow = 1;

    if (cross)
      lane_follow = 0;

    if (rot == 1) {
      fpwm = LOW_PWM; bpwm = -LOW_PWM;
      rpwm = -LOW_PWM; lpwm = LOW_PWM;
      continue;
    } 
    
    if (rot == -1) {
      fpwm = -LOW_PWM; bpwm = LOW_PWM;
      rpwm = LOW_PWM; lpwm = -LOW_PWM;
      continue;
    }

    fpwm = jxpwm; bpwm = jxpwm;
    rpwm = jypwm; lpwm = jypwm;

/*
    if (lane_follow) {
      if (abs(jxpwm)<20 && abs(jypwm)>20) { // forward or backward
        if (lsaF) {
            if (vlsaF <= 18)
              fpwm = LOW_PWM;
            else if (vlsaF >= 53)
              fpwm = -LOW_PWM;
            else
              fpwm = 0;
          } else {
              fpwm = 0;
          
  
          if (lsaB) {
            if (vlsaB <= 18)
              bpwm = -LOW_PWM;
            else if (vlsaB >= 53)
              bpwm = LOW_PWM;
            else
              bpwm = 0;
          } else {
            bpwm = 0;
          }
      }
  
      if (abs(jxpwm)>20 && abs(jypwm)<20) { // right or left
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
    }*/
      
    vTaskDelay(1);
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
