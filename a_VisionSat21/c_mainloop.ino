/ * mainloop
  * mainloop & main capturedata() function * / 

void loop() {
  if (millis() - previousBeacon >= beaconDelay) {
    previousBeacon = millis(); 
    Serial.println("looping...");
//    sensorData = captureData();
//    Serial.println(sensorData);
//    saveData();
    
    Serial.println(lookUpTemperature()); 
    Serial.println(getAltimeter()); 
  } 
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
