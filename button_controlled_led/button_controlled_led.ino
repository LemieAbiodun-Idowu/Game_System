void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor = digitalRead(2);
  if(!sensor){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  }

}
