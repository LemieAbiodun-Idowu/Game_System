int RED = 3;
int GREEN = 5;
int BUZZER = 2;
int brightness = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(16, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int b1 = analogRead(A0);
  int b2 = analogRead(A1);
  b1 = map(b1, 0, 1023, 0, 255);
  b2 = map(b2, 0, 1023, 0, 255);
  analogWrite(RED, b1);
  analogWrite(GREEN, b2);

  if(digitalRead(16)){
    digitalWrite(BUZZER, LOW);
  }
  else{
    digitalWrite(BUZZER, HIGH);
  }
}
