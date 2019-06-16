
int path_res = 40;
int path_res2 = path_res/2;
int path_res4 = path_res/4;

int path_delay = 700/path_res;
  
float path_x[40];
float path_y[40];

const float raise_height = 3;
const float lower_height = 0.5;
const float step_radius = 5;
int path_progress[4] = {0,0,0,0};

float t1,t2;

bool marching = false;

void start_march() {
  calc_traj();
  
  for(int i=0; i<4; i++) {
    path_progress[i] = 0;
  }

  for(int i=0; i<path_res2; i++) {
    ik((step_radius+path_x[i])/2,path_y[i],0,&t1,&t2);
    //Serial.print(t1);Serial.println(t2);
    move_leg(0,t1,t2);
    ik((step_radius+path_x[i])/2,path_y[i],2,&t1,&t2);
    //Serial.print(t1);Serial.println(t2);
    move_leg(2,t1,t2);
    path_progress[0] = i;
    path_progress[2] = i;
    delay(path_delay);
  }

  marching = true;
}

void march(bool lg0, bool lg1, bool lg2, bool lg3) {
  if (marching) {
    for(int i=0; i<4; i++) {
      int k = path_progress[i];
      ik(path_x[k],path_y[k],i,&t1,&t2);
      //Serial.print(t1);Serial.println(t2);
      if (i==0 && lg0 || i==1 && lg1 || i==2 && lg2 || i==3 && lg3) 
        move_leg(i,t1,t2);
      path_progress[i] = (path_progress[i]==path_res-1)?0:path_progress[i]+1;
    }
    delay(path_delay);
  }
}

void stop_march() {
  marching = false;
  bool flag = false;

  while (flag) {
    flag = true;
    for(int i=0; i<4; i++) {
      int k = path_progress[i];
      ik(path_x[k],path_y[k],i,&t1,&t2);
      //Serial.print(t1);Serial.println(t2);
      move_leg(i,t1,t2);
      if (abs(path_x[k]) > 0.1 || abs(path_y[k]) > 0.1) 
        path_progress[i] = (path_progress[i]==path_res-1)?0:path_progress[i]+1;

      flag = flag && abs(path_x[k])<0.1 && abs(path_y[k])<0.1;
    }

    delay(path_delay);
  }

  rest_pos();
  delay(1000);
}

void calc_traj() {

  for (int t=0; t<path_res; t+=1) {
    path_x[t] = 0;
    path_y[t] = 0;  
  }

  /*
  for (int t=0; t<path_res/2/3; t+=1) {
    path_x[t] = -step_radius;
    path_y[t] = raise_height*t/(path_res/2/3-1) + lower_height;
  }

  for (int t=0; t<path_res/2/3; t+=1) {
    path_x[path_res/2/3+t] = -step_radius + 2*step_radius*t/(path_res/2/3-1);
    path_y[path_res/2/3+t] = raise_height + lower_height;
  }

  for (int t=0; t<path_res/2/3; t+=1) {
    path_x[path_res/2/3*2+t] = step_radius;
    path_y[path_res/2/3*2+t] = raise_height - raise_height*t/(path_res/2/3-1) +lower_height;
  }*/

  
  for(int t=0; t<path_res2; t+=1) {
    path_x[t] = -step_radius*cos(PI-t*PI/(path_res2-1));
    path_y[t] = raise_height*sin(PI-t*PI/(path_res2-1)) + lower_height;
  }

  for(int t=0; t<path_res2; t+=1) {
    path_x[path_res2+t] = step_radius*cos(PI-t*PI/(path_res2-1));
    path_y[path_res2+t] = lower_height-lower_height*sin(PI-t*PI/(path_res2-1));
  }

  // fix unfilled elements
  for(int t=0; t<path_res-1; t+=1) {
    if (path_x[t+1] == 0 && path_y[t+1] == 0) {
      path_x[t+1] = path_x[t];
      path_y[t+1] = path_y[t];
    }
  }
}
