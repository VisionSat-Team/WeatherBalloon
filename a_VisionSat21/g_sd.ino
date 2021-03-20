void saveData() {
//  fileName = getTime();

  // Testing 
  fileName = "test";
  
  if (fileName == lastFileName) {
    fileName = fileName + String(duplicate) + ".txt"; /// Goes to string? needs to be tested 
    
    duplicate++;
    if (duplicate > 99) {
      duplicate = 0; 
    }
  }

  lastFileName = fileName;
  File currentFile = SD.open(fileName, FILE_WRITE);
  currentFile.println(  sensorData);
  currentFile.close();
}
