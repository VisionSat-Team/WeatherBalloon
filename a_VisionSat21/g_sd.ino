void saveData() {
  fileName = getTime();

  if (fileName == lastFileName) {
    fileName = fileName + duplicate + ".txt"; /// Goes to string? needs to be tested 
    
    duplicate++;
    if (duplicate > 99) {
      duplicate = 0; 
    }
  }

  lastFileName = fileName;
  File currentFile = SD.open(fileName, FILE_WRITE);
  currentFile.println(sensorData);
  currentFile.close();
}
