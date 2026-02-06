int red = 3;
int green = 5;
int blue = 6;

#define COMMON_ANODE

void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  setColour(255, 0, 0);
  delay(1000);

  setColour(0, 255, 0);
  delay(1000);

  setColour(0, 0, 255);
  delay(1000);

  setColour(255, 255, 0);
  delay(1000);

  setColour(80, 0, 80);
  delay(1000);
 
  setColour(0, 255, 255);
  delay(1000);
}

void setColour(int r, int g, int b){
  #ifdef COMMON_ANODE
    r = 255-r;
    g = 255-g;
    b = 255-b;
  #endif
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}
