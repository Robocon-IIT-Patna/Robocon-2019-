extern int lsaF,lsaB,lsaR,lsaL;
extern int vlsaF,vlsaB,vlsaR,vlsaL;

extern int jxpwm, jypwm;
extern int fpwm,rpwm,bpwm,lpwm;

const int HIGH_PWM = 80;
const int LOW_PWM = 30;

enum directions {FORWARD, RIGHT, BACKWARD, LEFT};
enum directions dir;

int line_follower = 0;
int pwm_set = 0;

void follow_line() {
  if (line_follower == 0) // autopilot is disabled
    return;
        
  if (lsaF+lsaB+lsaR+lsaL == 0) { // no line detected. stop.
    disable_line_follower();
    return;
  }

  // set pwms
  
  if (dir == FORWARD) {
    rpwm =  pwm_set; lpwm =  pwm_set;
  } else if (dir == RIGHT) {
    fpwm =  pwm_set; bpwm =  pwm_set;
  } else if (dir == BACKWARD) {
    rpwm = -pwm_set; lpwm = -pwm_set;
  } else if (dir == LEFT) {
    fpwm = -pwm_set; bpwm = -pwm_set;
  }

  // align with line
  if (dir == FORWARD || dir == BACKWARD) {
    if (lsaB && vlsaB <= 18)
      fpwm = LOW_PWM;
    else if (lsaB && vlsaB >= 53)
      fpwm = -LOW_PWM;
    else
      fpwm = 0;
    
    if (lsaF && vlsaF <= 18)
      bpwm = -LOW_PWM;
    else if (lsaF && vlsaF >= 53)
      bpwm = LOW_PWM;
    else
      bpwm = 0;
  }

  if (dir == RIGHT || dir == LEFT) {
    if (lsaR && vlsaR <= 18)
      rpwm = LOW_PWM;
    else if (lsaR && vlsaR >= 53)
      rpwm = -LOW_PWM;
    else
      rpwm = 0;

    if (lsaL && vlsaL <= 18)
      lpwm = -LOW_PWM;
    else if (lsaL && vlsaL >= 53)
      lpwm = LOW_PWM;
    else
      lpwm = 0;
  }

  if (lsaF+lsaB+lsaR+lsaL == 1) {
      pwm_set = LOW_PWM;
      return;
  }
  
  // decide direction
  if (lsaF+lsaB+lsaR+lsaL > 2) // multiple or ambiguous paths. return.
    return;

  pwm_set = HIGH_PWM;
  
  if (lsaF && lsaR) {
    if      (dir == FORWARD);
    else if (dir == RIGHT);
    else if (dir == BACKWARD) dir = RIGHT;
    else if (dir == LEFT)     dir = FORWARD;
  }

  if (lsaF && lsaL) {
    if      (dir == FORWARD);
    else if (dir == RIGHT)    dir = FORWARD;
    else if (dir == BACKWARD) dir = LEFT;
    else if (dir == LEFT);
  }

  if (lsaB && lsaR) {
    if      (dir == FORWARD) dir = RIGHT;
    else if (dir == RIGHT);
    else if (dir == BACKWARD);
    else if (dir == LEFT)    dir = BACKWARD;
  }

  if (lsaB && lsaL) {
    if      (dir == FORWARD) dir = LEFT;
    else if (dir == RIGHT)   dir = BACKWARD;
    else if (dir == BACKWARD);
    else if (dir == LEFT);
  }
}


void enable_line_follower() {
  bool flag = true;
  
  if (lsaF && lsaB && abs(jypwm)>20)
    dir = (jypwm >= 0) ? FORWARD : BACKWARD;
  else if (lsaR && lsaL && abs(jxpwm)>20)
    dir = (jxpwm >= 0) ? RIGHT : LEFT;
  else
    // if above conditions have not been met, don't start line follower
    flag = false;

  line_follower = flag;
}

void disable_line_follower() {
  line_follower = 0;
}
