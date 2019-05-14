#define DEBUG 0

void setup() {
  init_sensors();
  init_actuators();

  #if DEBUG
  init_serial();
  #endif
}

void loop() {
  process_sensor_data();
  manual();
  follow_line();
  drive_actuators();
  delay(10);

  #if DEBUG
  print_info();
  #endif
}
