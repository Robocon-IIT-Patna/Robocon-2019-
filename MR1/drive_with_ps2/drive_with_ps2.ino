#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

Cytron_PS2Shield ps2(0, 1);
/* Left Joystick: left-right movement
 * Right Joystick: forward-backward movement
 */

int dirF = 7;
int pwmF = 8;
int dirB = 9;
int pwmB = 10;
int dirR = 2;
int pwmR = 3;
int dirL = 4;
int pwmL = 5;

void setup()
{
  ps2.begin(9600); // This baudrate must same with the jumper setting at PS2 shield
  Serial.begin(9600); // Set monitor baudrate to 9600

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

  Stop();
}

void loop()
{
  int leftjoy = ps2.readButton(PS2_JOYSTICK_LEFT_X_AXIS);
  int rightjoy = ps2.readButton(PS2_JOYSTICK_RIGHT_Y_AXIS);

  int xdir = (leftjoy > 128)?1:0;
  int ydir = (rightjoy < 128)?1:0;

  int xpwm = abs(leftjoy-128)*2, ypwm = abs(rightjoy-128)*2;
  xpwm = constrain(xpwm,0,255);
  ypwm = constrain(ypwm,0,255);

  /*
  Serial.print((xdir?"R ":"L "));
  Serial.print(xpwm);
  Serial.print((ydir?" F ":" B "));
  Serial.print(ypwm);
  Serial.println();
  */

  Serial.print(xdir);
  Serial.print(" ");
  Serial.print(xpwm);
  Serial.print(" ");
  Serial.print(ydir);
  Serial.print(" ");
  Serial.print(ypwm);
  Serial.println();
  
  if (ypwm > 20 && xpwm < 20) {
    if (ydir) {
      Forward(ypwm);
    } else {
      Backward(ypwm);
    }
  } else if (xpwm > 20 && ypwm < 20) {
    if (xdir) {
      Right(xpwm);
    } else {
      Left(xpwm);
    }
  } else
    Stop();
  
  delay(100);
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

void Stop() {
  analogWrite(pwmF,0);
  analogWrite(pwmB,0);
  analogWrite(pwmL,0);
  analogWrite(pwmR,0);
}
