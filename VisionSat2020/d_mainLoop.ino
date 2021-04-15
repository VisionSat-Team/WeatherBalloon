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
    //    String sensorData = "";

    // Beacon TNC data to ground station
    keyUp(sensorData);

    // Start new file name "####"
    String genFileName = newFileName(); // General file name

    // Create Names for each file
    String dataFileName = genFileName + ".txt";      // "####.txt"
    String picFileName =  genFileName + "pic.txt";   // "####pic.txt"

    // Save sensorData to SD card
    saveData(sensorData, dataFileName);

    // Take Picture and save to SD Card
    takePicture(picFileName);

    // Update Timer
    previousMillis = millis();

    // Testing
    Serial.println(sensorData);
    Serial.println(dataFileName);
    Serial.println(picFileName);
    Serial.println("  end of main loop");
    Serial.println();
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
  // SDAvailable? ****
  // Wire Cut? ****

  return timeData + "," + gpsData + "," + altimeterData + "," + temperatureData;
}
