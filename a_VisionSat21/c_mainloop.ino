/ * mainloop
  * mainloop & main capturedata() function * / 

void loop() {

  if (millis() - beaconDelay >= previousMillis) {
    Serial.println("looping...");

    // Capture Data
    sensorData = captureData(); 

    //Testing: Print data to serial 
    Serial.println(sensorData); 

    // Save Data 
    saveData();

    // Timer Functionality 
    previousMillis = millis(); 
  } 
 
}

// Sensor & Data Functions
String captureData() {

  captureGPS();
  String temperatureData = String(toFarenheit());
  String gpsData = String(getLocation());
  String altimeterData = String(getAltimeter());
  String timeData = (getTime());
  
  return temperatureData + "," + altimeterData + "," + gpsData +"," +timeData;
}
