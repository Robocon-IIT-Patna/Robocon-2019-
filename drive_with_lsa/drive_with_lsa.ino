//#include <SoftwareSerial.h>
//#include <Cytron_PS2Shield.h>

// subtract ps2 values from 1 so that 0 = pressed and 1 = released
//Cytron_PS2Shield ps2(0, 1); // SoftwareSerial: Rx and Tx pin
/*  left joystick -> left-right
 *  right joystick -> forward-backward
 */

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

int autopilot = 0, dir = 1;
/* 0->stop
 * 1->forward
 * 2->right
 * 3->backward
 * 4->left
 */

void setup() {
  //ps2.begin(9600); // This baudrate must same with the jumper setting at PS2 shield
  Serial.begin(9600);
  stop();
  //ymove(-100);
  //xmove(100);
}

void loop() {
  int lsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
  int lsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
  int lsaR = (int)((float)analogRead(analogPinR)/921*70); //right
  int lsaL = (int)((float)analogRead(analogPinL)/921*70); //left

  Serial.println(lsaF+String("\t")+lsaB+String("\t")+lsaR+String("\t")+lsaL);

  follow_line();
}
/*
void loop() {
  int xpwm = (ps2.readButton(PS2_JOYSTICK_LEFT_X_AXIS)-128)*2;
  int ypwm = (ps2.readButton(PS2_JOYSTICK_RIGHT_Y_AXIS)-128)*2;
  int auto_enb = 1-ps2.readButton(PS2_TRIANGLE);
  int auto_dis = 1-ps2.readButton(PS2_CROSS);

  if (auto_dis) auto_enb = 0;

  if (au to_enb) {
    engage_autopilot(xpwm,ypwm);
  }

  if (auto_dis) {
    disengage_autopilot();
  }
  
  if (autopilot)
    follow_line();
  else
    manual_drive(xpwm,ypwm); 
}*/

void engage_autopilot(int xpwm, int ypwm) {
  if (abs(xpwm) > 20 && abs(ypwm) > 20) {
    autopilot = 0;
    return;
  }

  if (abs(xpwm) < 20 && abs(ypwm) < 20) {
    autopilot = 0;
    return;
  }

  autopilot = 1;
  
  // forward
  if (abs(ypwm) > 20 && ypwm >= 0) {
    dir = 1;
    return;
  }

  // right
  if (abs(xpwm) > 20 && xpwm >= 0) {
    dir = 2;
    return;
  }

  // backward
  if (abs(ypwm) > 20 && ypwm < 0) {
    dir = 3;
    return;
  }

  // left
  if (abs(xpwm) > 20 && xpwm < 0) {
    dir = 4;
    return;
  }
}

inline void disengage_autopilot() {
  autopilot = 0;
}

inline void manual_drive(int xpwm, int ypwm) {
  xmove(xpwm);
  ymove(ypwm);
}

void follow_line() {  
  int lsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
  int lsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
  int lsaR = (int)((float)analogRead(analogPinR)/921*70); //right
  int lsaL = (int)((float)analogRead(analogPinL)/921*70); //left
  lsaF = hasLine(lsaF);
  lsaB = hasLine(lsaB);
  lsaR = hasLine(lsaR);
  lsaL = hasLine(lsaL);

  if (dir == 1) {
    forward(100);
    left(0);
  } else if (dir == 2) {
    right(100);
    forward(0);
  } else if (dir == 3) {
    backward(100);
    left(0);
  } else if (dir == 4) {
    left(100);
    forward(0);
  }
  
  //balanceFB(); balanceLR();
  
  if (lsaF+lsaB+lsaR+lsaL >= 3)
    return;

  if (lsaF+lsaB+lsaR+lsaL <= 1) {
    //stop();
    //disengage_autopilot();
    return;
  }

/*
  if ((lsaF && !lsaB && !lsaR && !lsaL) || (!lsaF && lsaB && !lsaR && !lsaL)) {
    //balanceFB();
    return;
  }

  if ((!lsaF && !lsaB && lsaR && !lsaL) || (!lsaF && !lsaB && !lsaR && lsaL)) {
    //balanceLR();
    return;
  }
*/
  if (lsaF && lsaB) {
    //balanceFB();
    return;
  }

  if (lsaR && lsaL) {
   // balanceLR();
   return;
  }

  if (lsaF && lsaR) {
    if (dir == 1) ;
    else if (dir == 2) ;
    else if (dir == 3) dir = 2;
    else if (dir == 4) dir = 1;
  }

  if (lsaB && lsaR) {
    if (dir == 1) dir = 2;
    else if (dir == 2) ;
    else if (dir == 3) ;
    else if (dir == 4) dir = 3;
  }

  if (lsaF && lsaL) {
    if (dir == 1) ;
    else if (dir == 2) dir = 1;
    else if (dir == 3) dir = 4;
    else if (dir == 4) ;
  }

  if (lsaB && lsaL) {
    if (dir == 1) dir = 4;
    else if (dir == 2) dir = 3;
    else if (dir == 3) ;
    else if (dir == 4) ;
  }
}

void balanceFB() {
  int lsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
  int lsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
  int lsaR = (int)((float)analogRead(analogPinR)/921*70); //right
  int lsaL = (int)((float)analogRead(analogPinL)/921*70); //left
  
  if (hasLine(lsaF) && lsaF <= 18) { // nudge forward
    digitalWrite(fwdir , LOW);
    analogWrite(fwpwm , 100);
  } else if (hasLine(lsaF) && lsaF >= 53) { // nudge backward
    digitalWrite(fwdir , HIGH);
    analogWrite(fwpwm , 100);
  } else {
    analogWrite(fwpwm, 0);
  }

  if (hasLine(lsaB) && lsaB <= 18) {
    digitalWrite(bwdir, HIGH);
    analogWrite(bwpwm, 100);
  } else if (hasLine(lsaB) && lsaB >= 53) {
    digitalWrite(bwdir, LOW);
    analogWrite(bwpwm, 100);
  } else {
    analogWrite(bwpwm,0);
  }
}

void balanceLR() {
  int lsaF = (int)((float)analogRead(analogPinF)/921*70); //forward
  int lsaB = (int)((float)analogRead(analogPinB)/921*70); //backward
  int lsaR = (int)((float)analogRead(analogPinR)/921*70); //right
  int lsaL = (int)((float)analogRead(analogPinL)/921*70); //left
  
  if (hasLine(lsaR) && lsaR <= 18) { // nudge forward
    digitalWrite(rwdir , LOW);
    analogWrite(rwpwm , 100);
  } else if (hasLine(lsaR) && lsaR >= 53) { // nudge backward
    digitalWrite(rwdir , HIGH);
    analogWrite(rwpwm , 100);
  } else {
    analogWrite(rwpwm, 0);
  }

  if (hasLine(lsaL) && lsaL <= 18) {
    digitalWrite(lwdir, LOW);
    analogWrite(lwpwm, 100);
  } else if (hasLine(lsaL) && lsaL >= 53) {
    digitalWrite(lwdir, HIGH);
    analogWrite(lwpwm, 100);
  } else {
    analogWrite(lwpwm,0);
  }
}

inline int hasLine(int val) {
  if ((val>=0) && (val<=70))
    return 1;
  else
    return 0;
}

inline void xmove(int pwm) {
  if (pwm >= 0)
    right(constrain(pwm,0,250));
  else
    left(constrain(-1*pwm,0,250));  
}

inline void ymove(int pwm) {
  if (pwm >= 0)
    forward(constrain(pwm,0,250));
  else
    backward(constrain(-1*pwm,0,250));
}

void forward(int pwm) {
  digitalWrite(lwdir,HIGH);
  digitalWrite(rwdir,LOW);
  analogWrite(lwpwm,pwm);
  analogWrite(rwpwm,pwm);
}

void backward(int pwm) {
  digitalWrite(lwdir,LOW);
  digitalWrite(rwdir,HIGH);
  analogWrite(lwpwm,pwm);
  analogWrite(rwpwm,pwm);
}

void right(int pwm) {
  digitalWrite(fwdir,HIGH);
  digitalWrite(bwdir,HIGH);
  analogWrite(fwpwm,pwm);
  analogWrite(bwpwm,pwm);
}

void left(int pwm) {
  digitalWrite(fwdir,LOW);
  digitalWrite(bwdir,LOW);
  analogWrite(fwpwm,pwm);
  analogWrite(bwpwm,pwm);
}

void clockrot(int pwm) {
  digitalWrite(fwdir,HIGH);
  digitalWrite(rwdir,LOW);
  digitalWrite(bwdir,LOW);
  digitalWrite(lwdir,HIGH);
  analogWrite(fwpwm,pwm);
  analogWrite(bwpwm,pwm);
  analogWrite(lwpwm,pwm);
  analogWrite(rwpwm,pwm);
}

void anticlockrot(int pwm) {
  digitalWrite(fwdir,LOW);
  digitalWrite(rwdir,HIGH);
  digitalWrite(bwdir,HIGH);
  digitalWrite(lwdir,LOW);
  analogWrite(fwpwm,pwm);
  analogWrite(bwpwm,pwm);
  analogWrite(lwpwm,pwm);
  analogWrite(rwpwm,pwm);
}

void stop() {
  analogWrite(fwpwm,0);
  analogWrite(bwpwm,0);
  analogWrite(lwpwm,0);
  analogWrite(bwpwm,0);
}
