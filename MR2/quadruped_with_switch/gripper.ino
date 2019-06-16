#include <Servo.h>

//const int ultra_power = 27;
const int trigPin = 22;
const int echoPin = 23;

const int gerege_thres = 50;
const int wait_time_1 = 10000; //10s
const int wait_time_2 = 5000; //5s

Servo gripper_servo;
const int gripper_servo_pin = 13;
const int gripper_open_pos = 50;
const int gripper_close_pos = 103;

Servo tail_servo;
const int tail_servo_pin = 12 ;
const int tail_up_pos = 115;
const int tail_down_pos = 125;

void init_gripper() {
  // init ultrasonic sensor
  //pinMode(ultra_power,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  //digitalWrite(ultra_power,HIGH);

  gripper_servo.attach(gripper_servo_pin); // init gripper
  gripper_servo.write(gripper_open_pos); // open the gripper
  tail_servo.attach(tail_servo_pin);
  tail_servo.write(tail_up_pos);
}

void get_gerege() {
  while(true) {
    // wait till gerege arrives
    if (measure_dist() <= gerege_thres) {
       break;
    }
  }

  // wait for wait_time 1
  delay(wait_time_1);

  // close servo
  gripper_servo.write(gripper_close_pos);

  // wait for wait time 2
  delay(wait_time_2);

  // if mr1 away, move on
  if (measure_dist() >= gerege_thres) {
    int i;
    for(i=tail_up_pos; i!=tail_down_pos; ) {
      tail_servo.write(i);
      if (i<tail_down_pos) i++;
      if (i>tail_down_pos) i--;
      delay(50);
    }
    return;
  }

  // get ready to receive gerege again
  gripper_servo.write(gripper_open_pos);

  get_gerege();
}

void close_gripper() {
  init_gripper();
  gripper_servo.write(gripper_close_pos);
  tail_servo.write(tail_down_pos);
}

void up_gripper() {
  tail_servo.write(60);
}

int measure_dist() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long ultra_distance = (long)pulseIn(echoPin, HIGH)*34/2000;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(ultra_distance);

  return ultra_distance;
}
