const byte analogPinF = A7;   // Connect AN output of LSA1 to analog pin 0
const byte junctionPulseF = 28; // Connect JPULSE of LSA1 to pin 28
const byte analogPinB = A2;   // Connect AN output of LSA1 to analog pin 0
const byte junctionPulseB = 28; // Connect JPULSE of LSA1 to pin 28
const byte analogPinR = A4;   // Connect AN output of LSA2 to analog pin 1
const byte junctionPulseR = 29; // Connect JPULSE of LSA2 to pin 29
const byte analogPinL = A0;   // Connect AN output of LSA3 to analog pin 2
const byte junctionPulseL = 30; // Connect JPULSE of LSA3 to pin 30

int dirF =  9;              //Forward
int dirB = 7;
int pwmF = 10;
int pwmB = 8;
int dirR = 2; //RIGHT
int dirL = 4;
int pwmR = 3;
int pwmL = 5;


int readValueF,readValueB,readValueR,readValueL,lsa_F,lsa_B,lsa_R,lsa_L;    // Variables to store analog and line position value
unsigned int junctionCount = 0;   // Variable to store junction count value

int DIR = 0; // 0 -> forward 1-> right 2-> backward 3->left
const int pwm_l = 100;
const int pwm_h = 100;
int pwmg = pwm_l;

void setup() {
  pinMode(junctionPulseF,INPUT);
  pinMode(junctionPulseR,INPUT);
  pinMode(junctionPulseL,INPUT);
  
  pinMode(dirR , OUTPUT);
  pinMode(pwmR , OUTPUT);
  pinMode(dirL , OUTPUT);
  pinMode(pwmL , OUTPUT);
  pinMode(dirF , OUTPUT);
  pinMode(pwmF , OUTPUT);
  pinMode(dirB , OUTPUT);
  pinMode(pwmB , OUTPUT);

  Serial.begin(9600);
  
  digitalWrite(dirF , LOW);
  digitalWrite(dirR , HIGH);
  digitalWrite(dirB , LOW);
  digitalWrite(dirL , HIGH);

  wait();
  Serial.begin(9600);
  /*
  Serial.println("Testing motors:\n");
  Serial.println("Forward");
  Forward();
  delay(5000);
  Serial.println("Backward");
  Backward();
  delay(5000);
  Serial.println("Right");
  Right();
  delay(5000);
  Serial.println("Left");
  Left();
  delay(5000);
  Serial.println("\nMotor testing done");
  */
 
  Serial.println("\nReading LSA values\n");
  Serial.println("LSA_F\tLSA_R\tLSA_L\tDirection");

  //Forward();
}

void loop() {
  readValueF = analogRead(analogPinF);    // Read value from analog pin A0 (of LSA1)
  readValueB = analogRead(analogPinB);    // Read value from analog pin A0 (of LSA1)
  readValueR = analogRead(analogPinR);    // Read value from analog pin A1 (of LSA1)
  readValueL = analogRead(analogPinL);    // Read value from analog pin A2 (of LSA1)

  lsa_F = ((float)readValueF/921)*70; //forward
  lsa_B = ((float)readValueB/921)*70; //back
  lsa_R = ((float)readValueR/921)*70; //right
  lsa_L = ((float)readValueL/921)*70; //left

  Serial.print(lsa_F + String("\t") + lsa_B + String("\t") + lsa_R + String("\t") + lsa_L + String("\t"));

  if(hasLine(lsa_F) && hasLine(lsa_R) && hasLine(lsa_L)) {
    if (DIR == 1 || DIR == 3) {
      DIR = 0;
    }
    pwmg = pwm_h;
  }
  
  else if(hasLine(lsa_F) && !hasLine(lsa_R) && !hasLine(lsa_L)) {
    if (DIR == 1 || DIR == 3) {
    DIR = 0;
    }
    pwmg = pwm_h;
  }
  
  else if(hasLine(lsa_F) && hasLine(lsa_R) && !hasLine(lsa_L)) {
    if (DIR == 0) DIR = 0;
    else if (DIR == 1) DIR = 1;
    else if (DIR == 3) DIR = 0;
    pwmg = pwm_h;
  }
  
  else if(hasLine(lsa_F) && !hasLine(lsa_R) && hasLine(lsa_L)) {
    if (DIR == 0) DIR = 0;
    else if (DIR == 1) DIR = 0;
    else if (DIR == 3) DIR = 3;
    pwmg = pwm_h;
  }
  
  else if(!hasLine(lsa_F) && hasLine(lsa_R) && !hasLine(lsa_L)) {
    if (DIR == 1) DIR = 1;
    else if (DIR == 3) DIR = 3;
    else if (DIR == 0) DIR = 1;
    pwmg = pwm_l;
  }
  
  else if(!hasLine(lsa_F) && !hasLine(lsa_R) && hasLine(lsa_L)) {
    if (DIR == 1) DIR = 1;
    else if (DIR == 3) DIR = 3;
    else if (DIR == 0) DIR = 3;
    pwmg = pwm_l;
  }
  
  else if(!hasLine(lsa_F) && hasLine(lsa_R) && hasLine(lsa_L)) {
    // continue in previous direction
    pwmg = pwm_h;
  }
  
  else{
    DIR = 0;
    pwmg = pwm_l;
  }

  if (DIR == 0){ Serial.print("Forward"); //}
  Forward();}
  else if (DIR == 1) {Serial.print("Right"); //}
  Right();}
  else if (DIR == 2){ Serial.print("Backward"); //}
  Backward();}
  else if (DIR == 3){ Serial.print("Left"); //}
  Left();}
  
  Serial.print(String("\t")+DIR);
  
  Serial.println();
                                      
}

void Forward(){
digitalWrite(dirF , HIGH);
digitalWrite(dirB , LOW);
analogWrite(pwmR , 0);
analogWrite(pwmL , 0);
analogWrite(pwmF , pwmg);
analogWrite(pwmB , pwmg);

balanceFB();
}

void Backward(){
digitalWrite(dirF , LOW);
digitalWrite(dirB , HIGH);
analogWrite(pwmR , 0);
analogWrite(pwmL , 0);
analogWrite(pwmF , pwmg);
analogWrite(pwmB , pwmg);

balanceFB();
}

void Right(){
  digitalWrite(dirR , HIGH);
  digitalWrite(dirL , HIGH);
  analogWrite(pwmF , 0);
  analogWrite(pwmB , 0);
  analogWrite(pwmR , pwmg);
  analogWrite(pwmL , pwmg);

  balanceLR();
}

void Left(){
  digitalWrite(dirR , LOW); 
  digitalWrite(dirL , LOW);
  analogWrite(pwmF , 0);
  analogWrite(pwmB , 0);
  analogWrite(pwmR , pwmg);
  analogWrite(pwmL , pwmg);

  balanceLR();
}

void wait(){
  analogWrite(pwmR , 0);
  analogWrite(pwmL , 0);
  analogWrite(pwmF , 0);
  analogWrite(pwmB , 0);
}

void balanceFB() {
  if (hasLine(lsa_F) && lsa_F <= 18) { // nudge left
    digitalWrite(dirL , LOW);
    analogWrite(pwmL , pwm_l);
  } else if (hasLine(lsa_F) && lsa_F >= 53) { // nudge right
    digitalWrite(dirL , HIGH);
    analogWrite(pwmL , pwm_l);
  } else {
    analogWrite(pwmL, 0);
  }
}

void balanceLR() {
  if (hasLine(lsa_R) && lsa_R <= 18) { // nudge forward
    digitalWrite(dirB , LOW);
    analogWrite(pwmB , pwm_l);
  } else if (hasLine(lsa_R) && lsa_R >= 53) { // nudge backward
    digitalWrite(dirB , HIGH);
    analogWrite(pwmB , pwm_l);
  } else {
    analogWrite(pwmB, 0);
  }

  if (hasLine(lsa_L) && lsa_L <= 18) {
    digitalWrite(dirF, LOW);
    analogWrite(pwmF, pwm_l);
  } else if (hasLine(lsa_L) && lsa_L >= 53) {
    digitalWrite(dirF, HIGH);
    analogWrite(pwmF, pwm_l);
  } else {
    analogWrite(pwmF,0);
  }
}

inline boolean hasLine(int val) {
  return (val >= 0) && (val <= 70);
}
