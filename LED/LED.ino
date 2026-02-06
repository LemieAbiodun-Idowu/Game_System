
void setup() {
  int i;
  for(int i=2; i<10; i++){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  for(i=2;i<10;i++){
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }

}