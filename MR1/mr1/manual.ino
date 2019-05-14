extern int fpwm,rpwm,bpwm,lpwm;
extern int line_follower;

const int MAN_LOW_PWM = 50;
const int MAN_HIGH_PWM = 180;
const int ROT_PWM = 30;

int high_speed = false;
int jxpwm = 0, jypwm = 0, rot = 0;

void manual() {
  if (line_follower == 1)
    return;

  if (high_speed) {
    jxpwm = map(jxpwm,-128,128,-MAN_HIGH_PWM,MAN_HIGH_PWM);
    jypwm = map(jypwm,-128,128,-MAN_HIGH_PWM,MAN_HIGH_PWM);
  } else {
    jxpwm = map(jxpwm,-128,128,-MAN_LOW_PWM,MAN_LOW_PWM);
    jypwm = map(jypwm,-128,128,-MAN_LOW_PWM,MAN_LOW_PWM);
  }
    
  jxpwm = constrain(jxpwm,-255,255);
  jypwm = constrain(jypwm,-255,255);

  if (rot == 0) {
    fpwm = jxpwm; bpwm = jxpwm;
    rpwm = jypwm; lpwm = jypwm;
  } else {
    if (rot == 1) {
      fpwm = -ROT_PWM; bpwm = ROT_PWM;
      rpwm = -ROT_PWM; lpwm = ROT_PWM;
    } else if (rot == -1) {
      fpwm = ROT_PWM; bpwm = -ROT_PWM;
      rpwm = ROT_PWM; lpwm = -ROT_PWM;
    }
  }
}
