/*
There's no actual getTime() function rather an array holding the time.
*/
#include <Adafruit_GPS.h>

#include "SoftwareSerial.h"

SoftwareSerial serial(3, 2);
Adafruit_GPS GPS(&serial);

/* time[0] = hours, time[1] = minutes, time[2] = seconds 
Always update the time array using updateTime() before use */
uint8_t time[3];

#define GPSECHO false
     
boolean usingInterrupt = false;

void setup() {
  Serial.begin(115200);
     
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
}

void updateTime() {
  GPS.parse(GPS.lastNMEA());
  time[0] = GPS.hour;
  time[1] = GPS.minute;
  time[2] = GPS.seconds;
}

void loop() {}
