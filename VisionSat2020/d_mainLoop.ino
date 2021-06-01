/*
   mainLoop

   Contains Main loop() & captureData()
    - captureData(): returns data from all sensors as a String

   INCOMPLETE
*/

void loop() {

  // True if it has been (beaconDelay) time since last beacon;

  if (millis() - beaconDelay[whichDelay]*1000 >= previousMillis) {//convert to milliseconds
     listenNow = true;
    Serial.println(beaconDelay[whichDelay]); //number of seconds
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
      arduinoHIghLow();

    // Testing
    Serial.println(sensorData);
    Serial.println(dataFileName);
    Serial.println(picFileName);
    Serial.println("  end of main loop");
    Serial.println();
    whichDelay++;
    whichDelay = whichDelay%3;
  }

  // Check if TNC has a message
  captureGroundStation();
   if(millis()>1000*60*30){
   cutDown();
   }
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
