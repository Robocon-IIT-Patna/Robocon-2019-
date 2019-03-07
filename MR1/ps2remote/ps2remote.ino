#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

Cytron_PS2Shield ps2(0, 1);
/* Left Joystick: left-right movement
 * Right Joystick: forward-backward movement
 */

void setup()
{
  ps2.begin(9600); // This baudrate must same with the jumper setting at PS2 shield
  Serial.begin(9600); // Set monitor baudrate to 9600
}

void loop()
{
  int leftjoy = ps2.readButton(PS2_JOYSTICK_LEFT_X_AXIS);
  int rightjoy = ps2.readButton(PS2_JOYSTICK_RIGHT_Y_AXIS);

  int xdir = (leftjoy > 128)?1:0;
  int ydir = (rightjoy < 128)?1:0;

  int xpwm = abs(leftjoy-128)*2, ypwm = abs(rightjoy-128)*2;
  
  Serial.print((xdir?"R ":"L "));
  Serial.print(xpwm);
  Serial.print((ydir?" F ":" B "));
  Serial.print(ypwm);
  Serial.println();
  
  delay(100);
}
