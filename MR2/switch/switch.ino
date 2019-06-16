const int switchApin = 19;
const int switchBpin = 20;

enum states {sIDLE,MARCH,SAND_DUNE,ROPE,CLIMB};
enum states state = sIDLE;
enum states prev_state = MARCH;

bool prevA = 0;
bool prevB = 0;
bool switchA = 0;
bool switchB = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchApin,INPUT_PULLUP);
  pinMode(switchBpin,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  switchA = digitalRead(switchApin);

  if (switchA != prevA) {
    
  }
}
