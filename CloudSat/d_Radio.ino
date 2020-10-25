/*
   Radio Functions
*/


//Checks if enough time has elapsed since last beacon and keys up the radio if so.
//Checks against beaconInterval variable. Default period is 30 seconds (15, 30, 45, 60 seconds intervals possible)
void radioBeacon()
{
  currentMillis = millis();
  //Default beacon interval is 30 seconds.
  if (beaconInterval == 0 && currentMillis - lastBeacon >= 30000)
  {
    delay(1000);
    KEY_UP(header, beacon(title));
    delay(1000);
    lastBeacon = currentMillis;
  }
  else if (beaconInterval == 1 && currentMillis - lastBeacon >= 15000)
  {
    delay(1000);
    KEY_UP(header, beacon(title));
    delay(1000);
    lastBeacon = currentMillis;
  }
  else if (beaconInterval == 2 && currentMillis - lastBeacon >= 30000)
  {
    delay(1000);
    KEY_UP(header, beacon(title));
    delay(1000);
    lastBeacon = currentMillis;
  }
  else if (beaconInterval == 3 && currentMillis - lastBeacon >= 45000)
  {
    delay(1000);
    KEY_UP(header, beacon(title));
    delay(1000);
    lastBeacon = currentMillis;
  }
  else if (beaconInterval == 4 && currentMillis - lastBeacon >= 60000)
  {
    delay(1000);
    KEY_UP(header, beacon(title));
    delay(1000);
    lastBeacon = currentMillis;
  }

  //clear buffer so that ground commands are sent to the balloon clearly.
  for (int i = 0; i < 250; i++)
  {
    Serial1.read();
  }
}


//First checks if 2 minutes has elapsed since last tweet
//Uses KEY_UP with different header and message -> (IGATE header and tweet message) 
void radioTweet()
{
  currentMillis = millis();
  if (currentMillis - lastTweet >= 90000)
  {
    delay(1000);
    KEY_UP(headerIGATE, tweet);
    delay(1000);
    lastTweet = currentMillis;
  }
}


//Create payload for TNC beacon.
//This creates the string of data to be transmitted in KEY_UP()
//String title should either be title or titleForced for the two beacon types
String beacon(String title) {
  String comma = ",";
  String beaconBuffer = "*" + title + comma + String(latitude, 8) + comma + String(longitude, 8) + comma + String(currentAltitude, 9) + comma + String(ascentRate, 9) + comma + String(currentSpeed, 9) + comma + String(satellites) + comma + timeUTC + comma + String(picturesTaken) + comma + String(pressure, 2) + comma + String(temperature, 2) + comma + String(humidity, 2) + comma + String(externalTemp, 5) + comma + field + comma + cutDownStatus + comma + commandCameraToggle + comma + beaconInterval + comma + "#" + "\0";

  return beaconBuffer;
}


//Key up and put ax25 header.
//port1 leads into the TNC which ultimately transmits over the radio
//first write the header (regular or forced) and then the message (see beacon() function)
//0xC0 is the end character in ax.25
void KEY_UP (byte header[], String message)
{
  port1.write(header, 25);

  port1.print(message);//Data to be returned

  port1.write(0xC0);//c0.. End of frame

  delay(20);
  delay(6);
}
