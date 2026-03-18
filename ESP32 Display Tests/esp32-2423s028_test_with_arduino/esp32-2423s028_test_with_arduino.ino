HardwareSerial mySerial(2);

  //Use UART for simplicity, ease of access and because SPI and I2C are inefficient due to added complexity of how the Touchscreen also uses the SPI
  //(Insert reason for inability to use I2C)
void setup() {
  Serial.begin(115200); // USB debug

  mySerial.begin(115200, SERIAL_8N1, 35, 22); 
  // RX = 35, TX = 22
}

//You need a level shift of 3.3V from Rx on the Display to Tx on the Arduino (2kOhm or 6.8kOhm to ground) (1kOhm or 4.7kOhm with Tx on uno)

//Arduino will send the uid and based on that a different animation and portrait will be played.
//Optimise loading the bitmaps will be necessary
//Will need to change the stored cry durations

void loop() {
  if (mySerial.available()) {
    String msg = mySerial.readStringUntil('\n');
    Serial.println(msg);
    delay(1000);
    mySerial.println("Hello Arduino");
  }
}