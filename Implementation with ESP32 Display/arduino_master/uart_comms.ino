void UARTsetup(){
  Serial.begin(115200); //we are using the default tx and rx pins for arduino UART
}

void failedCardMsg(){
  Serial.println(F("INVALID_CRD"));
}

void passedCardMsg(int CardNumber){
  char msg[10];
  snprintf(msg, sizeof(msg), "CRD:%d", CardNumber);
  Serial.println(msg);
}