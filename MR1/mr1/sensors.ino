#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

extern int jxpwm, jypwm, rot, rotatearm;
extern int throw_toggle, open_toggle, gripper_toggle;

const int lsaPinF = A7;
const int lsaPinR = A4;
const int lsaPinB = A2;
const int lsaPinL = A0;

int lsaF=0,lsaB=0,lsaR=0,lsaL=0;
int vlsaF,vlsaB,vlsaR,vlsaL;

int prev_throw = 0, prev_open = 0, prev_gripper = 0;

Cytron_PS2Shield ps2(0,1);

void init_sensors() {
  ps2.begin(9600);
}

void process_sensor_data() {
  int i;
  
  vlsaF = (int)((float)analogRead(lsaPinF)/921*70); //forward
  vlsaB = (int)((float)analogRead(lsaPinB)/921*70); //backward
  vlsaR = (int)((float)analogRead(lsaPinR)/921*70); //right
  vlsaL = (int)((float)analogRead(lsaPinL)/921*70); //left

  lsaF = ((vlsaF>=0) && (vlsaF<70))?1:0;
  lsaB = ((vlsaB>=0) && (vlsaB<70))?1:0;
  lsaR = ((vlsaR>=0) && (vlsaR<70))?1:0;
  lsaL = ((vlsaL>=0) && (vlsaL<70))?1:0;
  
  jxpwm = (ps2.readButton(PS2_JOYSTICK_RIGHT_X_AXIS)-128);
  jypwm = -(ps2.readButton(PS2_JOYSTICK_LEFT_Y_AXIS)-128);

  rot = ps2.readButton(PS2_JOYSTICK_LEFT)-ps2.readButton(PS2_JOYSTICK_RIGHT);

  int triangle = 1-ps2.readButton(PS2_TRIANGLE);  // enable line follower
  int cross    = 1-ps2.readButton(PS2_CROSS);     // disable line follower
  int circle   = 1-ps2.readButton(PS2_CIRCLE);    // toggle shagai throw
  int square   = 1-ps2.readButton(PS2_SQUARE);    // toggle shagai picker
  int right_1  = 1-ps2.readButton(PS2_RIGHT_1);   // rotate arm up
  int right_2  = 1-ps2.readButton(PS2_RIGHT_2);   // rotate arm down
  int up       = 1-ps2.readButton(PS2_UP);        // toggle gerege gripper

  if (1-ps2.readButton(PS2_LEFT_2))
    high_speed = false;
  else if (1-ps2.readButton(PS2_LEFT_1))
    high_speed = true;

  if (triangle && cross)
    triangle = 0;

  // Decisions
  if (triangle)
    enable_line_follower();

  if (cross)
    disable_line_follower();

  if (right_1 && right_2)
    rotatearm = 0;
  else if (right_1)
    rotatearm = 1;
  else if (right_2)
    rotatearm = -1;
  else
    rotatearm = 0;

  // pneumatic toggle switches
  if (circle == 1 && prev_throw == 0)
    throw_toggle = !throw_toggle;

  if (square == 1 && prev_open == 0)
    open_toggle = !open_toggle;

  if (up == 1 && prev_gripper == 0)
    gripper_toggle = !gripper_toggle;
    
  prev_throw = circle;
  prev_open = square;
  prev_gripper = up;
}
