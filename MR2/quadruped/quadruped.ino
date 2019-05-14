#define ADA_SERVOS 0
#define WIRE_SERVOS 1
#define ZERO_POS 0

#define WAIT 0
#define REST_POS 1
#define MARCH 1

#define GRIPPER 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  init_servos();

  #if GRIPPER
  init_gripper();
  get_gerege();
  #endif

  #if WAIT
  delay(10000);
  #endif

  #if REST_POS
  rest_pos();
  #endif
 
  #if MARCH
  delay(2000);
  start_march();
  int start_time = millis();
  while(millis()-start_time<120000) {
    march(); 
  }
  stop_march();
  delay(3000);
  straight_pos();
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  //march();
}
