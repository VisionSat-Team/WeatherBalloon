/*
   Collect Data
*/


//Collects all data. Tries 5 times to get accurate data.
void collectData()
{
  commandCheck();
  int tries = 5; // the amount of tries incase an error occurs

  //Humidity
  humidity = getHumidity();
  while (isnan(humidity) && tries > 0) {
    humidity = getHumidity();
    tries--;
  }
  if (isnan(humidity)) { //if we never got a good reading
    humidity = 0;
  }

  commandCheck();
  tries = 5;

  //Temperature (internal)
  temperature = getTemperature();
  while (isnan(temperature) && tries > 0) {
    temperature = getTemperature();
    tries--;
  }
  if (isnan(temperature)) {
    temperature = 0;
  }

  commandCheck();
  tries = 5;

  //Pressure
  pressure = getPressure();
  while (isnan(pressure) && tries > 0) {
    pressure = getPressure();
    tries--;
  }
  if (isnan(pressure)) {
    pressure = 0;
  }

  commandCheck();
  tries = 5;
  
  readGPS();

  while (GPS.fix == 0 && tries > 0) {
    readGPS();
    tries--;
  }

  //Latitude
  latitude = getLatitude();

  commandCheck();

  //Longitude
  longitude = getLongitude();

  commandCheck();

  //Altitude
  currentAltitude = getAltitude();

  commandCheck();

  //Speed
  currentSpeed = getSpeed();

  commandCheck();

  //Time
  timeUTC = getTime();

  commandCheck();

  //Satellites locked onto
  satellites = getSatellites();

  commandCheck();

  //Temperature (external)
  externalTemp = getExternalTemperature();

  commandCheck();

  //Field
  for (int i = 0; i < 1000; i++)
  {
    if (isField())
    {
      positiveReadings++;
    }
  }

  //70% sure
  if (positiveReadings >= 700)
  {
    field = true;
  }

  //reset hall effect variables
  field = false;
  positiveReadings = 0;

  commandCheck();


  //calculate ascention rate
  currentMillis = millis() / 1000;
  ascentRate = (currentAltitude - previousAltitude) / (currentMillis - previousTime);
  previousTime = currentMillis;
  previousAltitude = currentAltitude;


  //take picture every 30 seconds
  currentMillis = millis();
  if (currentMillis - lastPicTaken >= 10000 && canTakePicture())
  {   
    picturesTaken++;
    takePicture();
    lastPicTaken = currentMillis;
    EEPROM.write(0, picturesTaken);
  }


  //Small delay to ensure photo saving and data saving dont overlap
  delay(500);


  //Create dataLog file
  File dataLog;

  //Save data to dataLog file
  dataLog = SD.open("dataLog.csv", FILE_WRITE);
  if (dataLog)
  {
    Serial.print("Writing to dataLog.txt...");
    dataLog.println(beacon(title));
    dataLog.close();
    Serial.println("Done");
  }
  else
  {
    Serial.println("Error with dataLog");
  }


  commandCheck();
}
