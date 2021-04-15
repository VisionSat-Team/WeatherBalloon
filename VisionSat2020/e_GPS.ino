/*
    GPS

    GPS Functions:
      - captureGPS(): update GPS data by capturing 2 NMEA sentences
      - getTime(): retrieves time from GPS data
      - getLocation(): returns GPS data, "latitude, longitude, altitude", as a String
*/

String getLocation() {
  // Update GPS
  captureGPS();

  // Parse NMEA Data
  if (GPS.fix) {
    return String(GPS.latitudeDegrees ) + ", " + String(GPS.longitudeDegrees) + ", " + String(GPS.altitude) + ", " + getFeetPerSec();
  }

  // Return error
  return "Error! No Fix";
}


void captureGPS() {
  int sentenceCount = 0;
  while (sentenceCount <= 2) {
    int initTries = 5;
    //break out of infinite loop
    while (!GPS.newNMEAreceived() && initTries > 0) {
      GPS.read();
      initTries--;
    }
    GPS.parse(GPS.lastNMEA());
    sentenceCount += 1;
  }
}


String getTime() {
  //Turns the hours into standard time  16 % 12 =4
  int hours = (GPS.hour - 5) % 12;
  if (hours = 0) hours = 12;

  //String manlipulations for User to read easier
  String minutes = String(GPS.minute);
  String sec = String(GPS.seconds);
  if (GPS.minute < 10) minutes = "0" + minutes;
  if (GPS.seconds < 10) sec = "0" + sec;

  return String(hours) + ":" + minutes + ":" + sec;

}

// return the rate of ascent and descent in feet per seconds.
String getFeetPerSec() {
  int oldAltitude = GPS.altitude;
  delay(1000);
  int currentAltitude = GPS.altitude;
  return String (currentAltitude - oldAltitude);

}
