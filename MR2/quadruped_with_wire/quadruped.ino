void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  init_servos();
  rest_pos();
  delay(5000);
  
  //start_march();
  //int start_time = millis();
  //while(millis()-start_time<10000) {
  //  march();  
  //}
  //stop_march();
}

void loop() {
  // put your main code here, to run repeatedly:
  //march();
}
