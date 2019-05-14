int fpwm = 0, rpwm = 0, bpwm = 0, lpwm = 0;
int rotatearm = 0;

const int fdirPin = 4;
const int fpwmPin = 5;
const int bdirPin = 2;
const int bpwmPin = 3;
const int ldirPin = 9;
const int lpwmPin = 10;
const int rdirPin = 7;
const int rpwmPin = 8;

const int armdirPin = 37;
const int armpwmPin = 39;

const int throwPin = 29;
const int openPin = 31;
const int gripperPin = 27;

int throw_toggle = 0;
int open_toggle = 0;
int gripper_toggle = 0;

void init_actuators() {
  pinMode(fdirPin,OUTPUT);
  pinMode(rdirPin,OUTPUT);
  pinMode(bdirPin,OUTPUT);
  pinMode(ldirPin,OUTPUT);
  
  pinMode(fpwmPin,OUTPUT);
  pinMode(rpwmPin,OUTPUT);
  pinMode(bpwmPin,OUTPUT);
  pinMode(lpwmPin,OUTPUT);

  pinMode(armdirPin,OUTPUT);
  pinMode(armpwmPin,OUTPUT);

  analogWrite(fpwmPin,0);
  analogWrite(rpwmPin,0);
  analogWrite(bpwmPin,0);
  analogWrite(lpwmPin,0);

  analogWrite(armpwmPin,0);
  
  pinMode(throwPin,OUTPUT);
  pinMode(openPin,OUTPUT);
  pinMode(gripperPin,OUTPUT);

  digitalWrite(throwPin,LOW);
  digitalWrite(openPin,LOW);
  digitalWrite(gripperPin,LOW);
}

void drive_actuators() {
  int fdir = (fpwm>=0)?1:0;
  int bdir = (bpwm>=0)?1:0;
  int rdir = (rpwm>=0)?1:0;
  int ldir = (lpwm>=0)?1:0;
  
  unsigned int ufpwm = abs(fpwm);
  unsigned int ubpwm = abs(bpwm);
  unsigned int urpwm = abs(rpwm);
  unsigned int ulpwm = abs(lpwm);
  
  digitalWrite(fdirPin,fdir);
  digitalWrite(bdirPin,bdir);
  digitalWrite(rdirPin,rdir);
  digitalWrite(ldirPin,ldir);

  analogWrite(fpwmPin,ufpwm); //*75/100);
  analogWrite(bpwmPin,ubpwm);
  analogWrite(rpwmPin,urpwm); //*75/100);
  analogWrite(lpwmPin,ulpwm);

  if (rotatearm == 1) {
    digitalWrite(armdirPin,LOW);
    digitalWrite(armpwmPin,HIGH);
  } else if (rotatearm == 0) {
    digitalWrite(armdirPin,LOW);
    digitalWrite(armpwmPin,LOW);
  } else if (rotatearm == -1) {
    digitalWrite(armdirPin,HIGH);
    digitalWrite(armpwmPin,HIGH);
  }

  // pneumatics
  digitalWrite(throwPin,throw_toggle);
  digitalWrite(openPin, open_toggle);
  digitalWrite(gripperPin, gripper_toggle);
}
