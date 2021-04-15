void captureGPS() {
  //Caputures the 2 NMEA sentences needed to parse the GPS data in the Adafruit GPS libary
  int sentenceCount = 0;
  while (sentenceCount <= 2) {
    int initTries = 5;
// break out of infinite loop
    while (!GPS.newNMEAreceived() && initTries > 0){
      GPS.read();
      initTries--;
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
String getLocation() {
  /*
    The GPS fix informs if the GPS is recieving a signal from Satilites
    and can be seen on our GPS Sensor as a bluelight.
    If the blue light is not on there can be many reasons but I suggest first move locations.
    Without the fix the GPS can not recieve any data.
  */
  if (GPS.fix) {
    return String(GPS.latitudeDegrees ) + ", " + String(GPS.longitudeDegrees) + ", " + String(GPS.altitude) + ", " + String(getFeetPerSec); 
  }
  return " Error No Fix";
}

String getFeetPerSec(){
// return the rate of descent in feet per seconds.

  int oldAltitude = GPS.altitude;
  delay(1000);
  int currentAltitude = GPS.altitude;
  return String (currentAltitude - oldAltitude);
  
}
/*void balloonModeGPS(){
  
}*/
