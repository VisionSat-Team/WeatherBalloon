void loop() {
  Serial.println("looping...");
  sensorData = captureData();
  Serial.println(sensorData);
  //  saveData();
}

// Sensor & Data Functions
String captureData() {
  //Travis
  //Fix the CaptureData and delete the global varibles except sensorData
  Serial.println("start");

//  captureGPS();
//  Serial.println("capture gps");

  String temperatureData = String(lookUpTemperature());
  String gpsData = String(getLocation());
  String altimeterData = String(getAltimeter());
  String timeData = (getTime());
  
  return timeData + "," + temperatureData + "," + gpsData + "," + altimeterData;
}
