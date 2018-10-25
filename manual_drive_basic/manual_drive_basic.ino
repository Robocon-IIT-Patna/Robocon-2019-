int dirF = 7;
int pwmF = 8;
int dirB = 9;
int pwmB = 10;
int dirR = 2;
int pwmR = 3;
int dirL = 4;
int pwmL = 5;

void setup() {
  pinMode(dirR , OUTPUT);
  pinMode(pwmR , OUTPUT);
  pinMode(dirL , OUTPUT);
  pinMode(pwmL , OUTPUT);
  pinMode(dirF , OUTPUT);
  pinMode(pwmF , OUTPUT);
  pinMode(dirB , OUTPUT);
  pinMode(pwmB , OUTPUT);

  digitalWrite(dirF , LOW);
  digitalWrite(dirR , HIGH);
  digitalWrite(dirB , LOW);
  digitalWrite(dirL , HIGH);

}

void loop() {
 Forward(100);
 delay(1000);
 Right(100);
 delay(1000);
 Backward(100);
 delay(1000);
 Left(100);
 delay(1000);
}

void Forward(int pwm) {
  digitalWrite(dirF , LOW);
  digitalWrite(dirB , LOW);
  analogWrite(pwmR , 0);
  analogWrite(pwmL , 0);
  analogWrite(pwmF , pwm);
  analogWrite(pwmB , pwm);
}

void Backward(int pwm) {
  digitalWrite(dirF , HIGH);
  digitalWrite(dirB , HIGH);
  analogWrite(pwmR , 0);
  analogWrite(pwmL , 0);
  analogWrite(pwmF , pwm);
  analogWrite(pwmB , pwm);
}

void Right(int pwm) {
  digitalWrite(dirR , LOW);
  digitalWrite(dirL , HIGH);
  analogWrite(pwmF , 0);
  analogWrite(pwmB , 0);
  analogWrite(pwmR , pwm);
  analogWrite(pwmL , pwm);
}

void Left(int pwm) {
  digitalWrite(dirR , HIGH);
  digitalWrite(dirL , LOW);
  analogWrite(pwmF , 0);
  analogWrite(pwmB , 0);
  analogWrite(pwmR , pwm);
  analogWrite(pwmL , pwm);
}
