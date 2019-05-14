
const int path_res = 40;
const int path_res2 = path_res/2;
const int path_res4 = path_res/4;

const int path_delay = 2000/path_res;
  
float path_x[path_res];
float path_y[path_res];

const float raise_height = 1.5;
const float lower_height = 1.5;
const float step_radius = 3;
int path_progress[4] = {0,0,0,0};

float t1,t2;

bool marching = false;

void start_march() {
  calc_traj();
  
  for(int i=0; i<4; i++) {
    path_progress[i] = 0;
  }

  for(int i=0; i<path_res2; i++) {
    ik(path_x[i],path_y[i],0,&t1,&t2);
    //Serial.print(t1);Serial.println(t2);
    move_leg(0,t1,t2);
    ik(path_x[i],path_y[i],2,&t1,&t2);
    //Serial.print(t1);Serial.println(t2);
    move_leg(2,t1,t2);
    path_progress[0] = i;
    path_progress[2] = i;
    delay(path_delay);
  }

  marching = true;
}

void march() {
  if (marching) {
    for(int i=0; i<4; i++) {
      int k = path_progress[i];
      ik(path_x[k],path_y[k],i,&t1,&t2);
      //Serial.print(t1);Serial.println(t2);
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

  for(int t=0; t<path_res2; t+=1) {
    path_x[t] = -step_radius*cos(-t*PI/(path_res2-1));
    path_y[t] = raise_height*sin(-t*PI/(path_res2-1)) + lower_height;
  }

  for(int t=0; t<path_res2; t+=1) {
    path_x[path_res2+t] = step_radius*cos(-t*PI/(path_res2-1));
    path_y[path_res2+t] = lower_height-lower_height*sin(-t*PI/(path_res2-1));
  }
}
