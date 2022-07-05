void setup() 
{
  Serial.begin(115200);
  pinMode(H, OUTPUT);
  pinMode(N, OUTPUT);
  pinMode(L, OUTPUT);
  pinMode(LL, OUTPUT);
  pinMode(button, INPUT);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

  uint32_t currentFrequency;
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");


  //Add Wifi & mqtt
  wifi_Setup();
  mqtt_Setup();
}

void loop() 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  
  unsigned long currenttime1 = millis();
  if (currenttime1 - previoustime1 >= event1){
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  //Add data to payload
  wifiLoop();
  mqttLoop();
  sprintf(payload, "\"loadvoltage\": %f,\"current_mA\": %f,\"power_mW\": %f", loadvoltage, current_mA, power_mW);
  client.publish(chPublish, payload); 

  
  if (loadvoltage >= 8){
    digitalWrite(H, HIGH);
    digitalWrite(N, LOW);
    digitalWrite(L, LOW);
    digitalWrite(LL, LOW);
  } else if (loadvoltage >= 7.53 && loadvoltage < 8){
    digitalWrite(H, LOW);
    digitalWrite(N, HIGH);
    digitalWrite(L, LOW);
    digitalWrite(LL, LOW);
  } else if (loadvoltage >= 7.41 && loadvoltage < 7.53){
    digitalWrite(H, LOW);
    digitalWrite(N, LOW);
    digitalWrite(L, HIGH);
    digitalWrite(LL, LOW);
  } else if (loadvoltage >= 6 && loadvoltage < 7.44){
    digitalWrite(H, LOW);
    digitalWrite(N, LOW);
    digitalWrite(L, LOW);
    digitalWrite(LL, HIGH);
  } else {
    digitalWrite(H, LOW);
    digitalWrite(N, LOW);
    digitalWrite(L, LOW);
    digitalWrite(LL, LOW);
  }
  previoustime1 = currenttime1;
  }


}
