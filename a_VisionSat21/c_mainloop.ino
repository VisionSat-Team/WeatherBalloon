/ * mainloop
  * mainloop & main capturedata() function * / 

void loop() {

  if (millis() - beaconDelay >= previousMillis) {
    
    Serial.println("looping...");
    sensorData = captureData();
    keyUp(sensorData);
    saveData();

    takePicture(); 
    
//  Serial.println( pressureSensor.GetPres());
    previousMillis = millis();
  }
  captureGroundStation();
}

// Sensor & Data Functions
String captureData() {
  //Travis
  //Fix the CaptureData and delete the global varibles except sensorData
  Serial.println("start");
  
  captureGPS();
  String temperatureData = String(toFarenheit());
  String gpsData = String(getLocation());
  String altimeterData = String(getAltitude());
  String timeData = (getTime());

  return temperatureData + "," + altimeterData + "," + gpsData + "," + timeData;
}
