class Leg {
  public:
    float l1,l2;
    float theta1,theta2;
    float bias1,bias2;

    Servo thigh,knee;

   Leg(float l1,float l2,int bias1,int bias2,int thigh_pin,int knee_pin) {
    this.l1 = l1;
    this.l2 = l2;
    this.bias1 = bias1;
    this.bias2 = bias2;

    thigh.attach(thigh_pin);
    knee.attach(knee_pin);
   }

   void ik(float x, float y) {
     theta1 = atan2(x,y) + acos((l1*l1+x*x+y*y-l2*l2)/2/l1/sqrt(x*x+y*y));
     theta2 = PI-acos((l1*l1+l2*l2-x*x-y*y)/2/l1/l2);
    
     theta1 = theta1*180/PI;
     theta2 = theta2*180/PI;
   }

   void move() {
    thigh.write(bias1+(int)theta1);
    knee.write(bias2+(int)theta2);
   }
}
