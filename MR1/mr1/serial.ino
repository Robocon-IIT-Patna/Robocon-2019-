#if DEBUG

extern int vlsaF,vlsaB,vlsaR,vlsaL;
extern enum directions dir;

char tmp[50];

int last_printed = 0;
int threshold = 200;

void init_serial() {
  Serial.begin(9600);
}

void print_info() {
  // print once every 200 ms
  if (millis() - last_printed > threshold) {
    sprintf(tmp,"%d\t%d\t%d\t%d\t",vlsaF,vlsaB,vlsaR,vlsaL);
    Serial.print(tmp);
  
    if (dir == FORWARD)
      Serial.print("Forward");
    else if (dir == RIGHT)
      Serial.print("Right");
    else if (dir == BACKWARD)
      Serial.print("Backward");
    else if (dir == LEFT)
      Serial.print("Left");
    
    Serial.println();
    
    last_printed = millis();
  }
}

#endif
