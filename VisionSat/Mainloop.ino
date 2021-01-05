void loop() {
  if (timerMilis > millis()) timerMilis = millis();
  
  if (timerMilis - millis() >=  beaconDelay) {
     timerMilis = millis();
    
    captureData();
    saveData();
    keyUp(SensorData);

  }

  // Poll Serial Buffer for TNC Data
  if (portTNC.available() > 0) {
    //    Serial.println("Packet Recieved");
    TNC_message = portTNC.readStringUntil(59);  // Reads until ASCII#59 (;)
    TNC_message = TNC_message.substring(25);    // Starts reading after Header
    TNC_message.toLowerCase();            // convert TNC_message to lower case
    
     if(TNC_message.toLowerCase() == "cutdown") {
        cutDown();
        KeyUp("Cutdown Command Recieved");
     }
     else if(TNC_message.toLowerCase() == "beacon"){
        keyUp("Manual Beacon: " + sensorData);
     }
     else if(TNC_message.substring(0,5).toLowerCase() == "delay")
        /* 
            Parses the TNC_message after the first 5 characters.
            The next characters should be the time for the delay in MilliSeconds 
            To change the delay variable "beaconDelay"
        */
        changeDelay(TNC_message.substring(5));
     }
     else if(TNC_message == "capture"){
        //This our instant capture
         captureData();
     }  
     else 
        keyUp("Command does not exist");
  }
}
