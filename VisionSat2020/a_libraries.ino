/* 
 *  Libraries
 *  
 *  Libraries & uses 
 */

// GPS Sensor
#include <Adafruit_GPS.h>
// Parses Messages from GPS 
#include <MicroNMEA.h> 

// Altimter Sensor 
#include <MS5xxx.h>

// Temperature Sensor
#include <Adafruit_MAX31865.h>
// Resistance-to-Temperature (Celsius 
#include <pt100rtd.h>

// ArduCAM
#include <ArduCAM.h>
// Used in ArduCAM library to pick which module to use
#include <memorysaver.h>

// SD Card 
#include <SD.h>

// Reading & Writing to/from EEPROM: for tracking number of Pictures Taken 
#include <EEPROM.h>

// Adds extra serial port. Used for TNC. 
#include <SoftwareSerial.h> 

// General Use 
#include <math.h>

// I2C communication 
#include <Wire.h>

// SPI communication 
#include <SPI.h>
