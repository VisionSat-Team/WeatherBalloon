/*
   SD

   SD Card Functions:
    - saveData(): 
        - saves sensorData to seperate text files
    - newFileName(): 
        - 

    INCOMPLETE
*/

void saveData (String sensorData, String fileName) {
  // Save File using String name
  File dataFile = SD.open(fileName, FILE_WRITE);

  if (dataFile) {
    dataFile.println(sensorData);
    dataFile.close();
  }
}


String newFileName() { 
  // *** Read values from EEPROM
  // Read # dataSaves 
  long numDataSaves = EEPROM.read(dataSaveCountAddress);

  // Read # of rollovers
  int numRollovers = EEPROM.read(dataSaveRolloverAddress);

  // Account for Rollovers
  numDataSaves += 256 * numRollovers;


  // *** Create String name
  String fileName = "";

  // Pad name with 0's
  if (numDataSaves == 0) {
    fileName = "0000";
  }
  else {
    for (int i = 0; i < 3 - log10(numDataSaves); i++) {
      fileName += "0";
    }
    fileName += String(numDataSaves);
  }

  // *** Update EEPROM values 
  // Increment #
  numDataSaves++;

  // Check if rollovers need to be updated
  if (numDataSaves % 256 == 0) {
    //    Serial.println(" rollover ");
    numDataSaves = 0;
    numRollovers++;
    EEPROM.update(dataSaveRolloverAddress, numRollovers);
  }
  // Update number of times data has been saved 
  EEPROM.update(dataSaveCountAddress, numDataSaves);

  return fileName; 
}

