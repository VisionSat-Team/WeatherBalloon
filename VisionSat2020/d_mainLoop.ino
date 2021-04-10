/*
   mainLoop

   Contains Main loop() & captureData()
    - captureData(): returns data from all sensors as a String

   INCOMPLETE
*/

void loop() {

  // True if it has been (beaconDelay) time since last beacon
  if (millis() - beaconDelay >= previousMillis) {
    // Capture Sensor Data 
    String sensorData = captureData();

    // Beacon TNC data to ground station 
    keyUp(sensorData);

    // Start new file name "####"
    String fileName = newFileName; 

    // Create Names for each file 
    String dataFileName = fileName + ".txt";      // "####.txt"
    String picFileName =  fileName + "pic.txt";   // "####pic.txt" 

    // Save sensorData to SD card
    saveData(sensorData, dataFileName);

    // Take Picture and save to SD Card 
    takePicture(picFileName); 

    // Update Timer 
    previousMillis = millis();
  }

  // Check if TNC has a message 
  captureGroundStation();
}



String captureData() {
  // Capture Data from each Sensor
  String temperatureData = String(toFarenheit());
  String gpsData = String(getLocation());
  String altimeterData = String(getPressure());
  String timeData = (getTime());
  // SDAvailable? 
  // Wire Cut? 

  return timeData + "," + gpsData + "," + altimeterData + "," + temperatureData;
}
