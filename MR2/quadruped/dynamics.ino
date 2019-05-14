
/*  Leg order: FL,FR,BR,BL
 *  Servo order: 2n -> thigh, 2n+1 -> knee
 */

const float l1[4] = {10,10,10,10}; // thigh
const float l2[4] = {17,17.25,17.7,18}; // knee
const float l3[4] = {4,4.1,3.2,4.3}; // shoe
const float stand_diff = 1;
const float stand_height = 30;
const int leg_2_servo[4] = {0,2,4,6};

const float x_bias[4] = {0,0,0,0};
const float y_bias[4] = {0.8,0,0.8,0};

void straight_pos() {
  float t1,t2;
  
  for(int i=0; i<4; i++) {
    ik(0,-stand_diff+0.01,i,&t1,&t2);
    move_leg(i,t1,t2);
  }
}

void rest_pos() {
  float t1,t2;

  for(int i=0; i<4; i++) {
    ik(0,0,i,&t1,&t2);
    move_leg(i,t1,t2);
  }
}

void ik(float x,float y,int lg,float *t1,float *t2) {
 
  // coordinate transform
  x = x+x_bias[lg];
  y = stand_height-y-l3[lg]-y_bias[lg];//-leg_bias[lg];
  
  //float k=acos(sqrt(x*x+y*y)/2/bone_length);
  //*t1 = (atan2(x,y)+k)*180/PI; // for > config
  //*t1 = (-atan2(x,y)+k)*180/PI; // for < config
  //*t2 = 2*k*180/PI;

  //*t1 = ((atan2(x,y)+acos((l1*l1+x*x+y*y-l2*l2)/2/l1/sqrt(x*x+y*y))))*180/PI;
  //*t2 = 180-acos((l1*l1+l2*l2-x*x-y*y)/2/l1/l2)*180/PI;

  float t3,t4;

  if (lg == 5 || lg == 6 || lg == 7 || lg == 8) {
    t3 = ((atan2(x,y)+acos((l1[lg]*l1[lg]+x*x+y*y-l2[lg]*l2[lg])/2/l1[lg]/sqrt(x*x+y*y))))*180/PI;
    t4 = 180-acos((l1[lg]*l1[lg]+l2[lg]*l2[lg]-x*x-y*y)/2/l1[lg]/l2[lg])*180/PI;
  } else {
    t3 = ((atan2(x,y)-acos((l1[lg]*l1[lg]+x*x+y*y-l2[lg]*l2[lg])/2/l1[lg]/sqrt(x*x+y*y))))*180/PI;
    t4 = -180+acos((l1[lg]*l1[lg]+l2[lg]*l2[lg]-x*x-y*y)/2/l1[lg]/l2[lg])*180/PI;
  }

  if (!isnan(t3) && !isnan(t4)) {
    *t1 = t3;
    *t2 = t4;
  }

  //if (lg == 2) {
  //  *t1 = 0.8*(*t1);
  //}
}

void move_leg(int lg,float t1, float t2) {
  move_servo(leg_2_servo[lg],t1);
  move_servo(leg_2_servo[lg]+1,t2);
}
