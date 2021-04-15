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
    return String(GPS.latitudeDegrees ) + ", " + String(GPS.longitudeDegrees) + ", " + String(GPS.altitude);
  }

  // Return error
  return "Error! No Fix";
}


void captureGPS() {
  int sentenceCount = 0;
  while (sentenceCount <= 2) {
    while (!GPS.newNMEAreceived())
    {
      GPS.read();
      //Serial.println(".");
    }
    GPS.parse(GPS.lastNMEA());
    sentenceCount += 1;
  }
}


String getTime() {
  //Turns the hours into standard time
  int hours = GPS.hour - 6;
  if (hours > 12) hours -= 12;
  if (hours <= 0) hours += 12;

  //String manlipulations for User to read easier
  String minutes = String(GPS.minute);
  String sec = String(GPS.seconds);
  if (GPS.minute < 10) minutes = "0" + minutes;
  if (GPS.seconds < 10) sec = "0" + sec;

  return String(hours) + ":" + minutes + ":" + sec;

}