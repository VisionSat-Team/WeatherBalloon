void loop() {

  if (millis() - beaconDelay >= previousMillis) {
//    sensorData = captureData();
//    Serial.println(sensorData);
    Serial.println("looping...");

    Serial.println(lookUpTemperature()); 
    Serial.println(getAltimeter()); 
//    saveData();
    previousMillis = millis(); 
  } 
//  delay(5000); 
}

// Sensor & Data Functions
String captureData() {
  //Travis
  //Fix the CaptureData and delete the global varibles except sensorData
//  Serial.println("start");

//  captureGPS();
  String temperatureData = String(lookUpTemperature());
  String gpsData = String(getLocation());
  String altimeterData = String(getAltimeter());
  String timeData = (getTime());
  
  return timeData + "," + temperatureData + "," + gpsData + "," + altimeterData;
}
