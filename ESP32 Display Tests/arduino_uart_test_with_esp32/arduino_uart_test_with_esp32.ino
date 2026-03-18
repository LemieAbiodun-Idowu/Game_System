void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     //Using default Rx and Tx pins on the arduino
  }

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello Esp32!");
  if(Serial.available()){
    Serial.println(Serial.readStringUntil("\n"));
  }
}
