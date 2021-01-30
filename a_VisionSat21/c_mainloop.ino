void loop() {

  if (millis() - beaconDelay >= previousMillis) {
//    Serial.println(sensorData);
    Serial.println("looping...");
    sensorData = captureData();
    Serial.println(  sensorData); 
    
    saveData();
    previousMillis = millis(); 
  } 
 
}

// Sensor & Data Functions
String captureData() {
  //Travis
  //Fix the CaptureData and delete the global varibles except sensorData
//  Serial.println("start");

  captureGPS();
  String temperatureData = String(toFarenheit());
  String gpsData = String(getLocation());
 String altimeterData = String(getAltitude(getPressure(),p_ref,lookUpTemperature()));
  String timeData = (getTime());
  
  return temperatureData + "," + altimeterData + "," + gpsData +"," +timeData;
}
