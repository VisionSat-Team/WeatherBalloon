/*
   GPS Functions
*/


//  GPS  //

//Reads in two NMEA sentences.
void readGPS()
{
  //create two strings to parse the two types of NMEA Sentences.
  String NMEA1;
  String NMEA2;

  //clear corrupted data.
  clearGPS();

  while (!GPS.newNMEAreceived())
  {
    readNMEA = GPS.read();
  }

  GPS.parse(GPS.lastNMEA());
  NMEA1 = GPS.lastNMEA();

  //immediately read next sentence.

  while (!GPS.newNMEAreceived())
  {
    readNMEA = GPS.read();
  }

  GPS.parse(GPS.lastNMEA());
  NMEA2 = GPS.lastNMEA();
}


//Clears NMEA strings of invalid data.
void  clearGPS()
{
  //read and parse old data to ensure non-corrupt sentences are saved.
  //Also accounts for delays in the main loop.

  while (!GPS.newNMEAreceived())
  {
    readNMEA = GPS.read();
  }

  GPS.parse(GPS.lastNMEA());

  while (!GPS.newNMEAreceived())
  {
    readNMEA = GPS.read();
  }

  GPS.parse(GPS.lastNMEA());

  while (!GPS.newNMEAreceived())
  {
    readNMEA = GPS.read();
  }

  GPS.parse(GPS.lastNMEA());
}


//Returns altitude. (Feet)
float getAltitude()
{
  //no fix check
  if (GPS.fix == 0)
  {
    return 0;
  }

  return GPS.altitude * 3.2;
}


//Returns latitude. (Decimal Degrees)
float getLatitude()
{
  //no fix check
  if (GPS.fix == 0)
  {
    return 0;
  }
  return GPS.latitudeDegrees;
}


//Returns longitude. (Decimal Degrees)
float getLongitude()
{
  //no fix check
  if (GPS.fix == 0)
  {
    return 0;
  }
  return GPS.longitudeDegrees;
}


//Returns speed. (Knots)
float getSpeed()
{
  //no fix check
  if (GPS.fix == 0)
  {
    return 0;
  }
  return GPS.speed;
}


//Returns time. (UTC - 5)
String getTime()
{
  //define variables
  String output;
  int hour = GPS.hour - 5;
  int minute = GPS.minute;
  int second = GPS.seconds;

  //define output string
  output += hour;
  output += ":";
  output += minute;
  output += ":";
  output += second;

  return output;
}


//Returns number of satellites GPS is currently locked onto.
float getSatellites()
{
  return GPS.satellites;
}
