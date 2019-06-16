
/*  Leg order: FL,FR,BR,BL
 *  Servo order: 2n -> thigh, 2n+1 -> knee
 */

const float l1[4] = {17,18,16.5,17}; // thigh
const float l2[4] = {24,24,24,24}; // knee
const float l3[4] = {0,0,0,0}; // shoe
const float stand_diff = 1;
const float stand_height = 40;
const int leg_2_servo[4] = {0,2,4,6};

float x_bias[4] = {0,0,0,0};//{3,3,2,2};//{2,-1,3,2};
float y_bias[4] = {0.5,0,0.5,0};

float x_frac[4] = {1,1,1,1};
float y_frac[4] = {1,1,1,1};

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

  x = x_frac[lg]*x;
  y = y_frac[lg]*y;
  
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

  if (lg == 2) {
    Serial.println(String(x) + " " + String(y) + " " + String(t3) + " " + String(t4)); 
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
