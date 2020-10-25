/*
   Prototype Functions
   Descriptions of these functions and the entirety of this arduino code can be found in the documentation folder.
*/

void commandCheck();
void collectData();
void radioBeacon();
void KEY_UP(byte header[], String message);
String beacon(String title);
void cutdown();
void buzz();
void backupCutdown();
float getHumidity();
float getTemperature();
float getPressure();
float getExternalTemperature();
float getLatitude();
float getLongitude();
float getAltitude();
float getSpeed();
float getSatellites();
String getTime();
boolean isField();
bool canTakePicture();
void takePicture();
void resetCameraToggleCheck();
void setSDVariableTrue();
void setSDVariableFalse();
void readGPS();
void clearGPS();


/*
  Library Includes
  Including libraries and some initial sensor setup occurs below. All libraries used in this program can be downloaded
  from the Google drive folder. (See CloudSat 8 team member for download link)
*/

#include <String.h>

//Communications
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


//EEPROM
#include <EEPROM.h>


//Time
#include <math.h>


//GPS
#include <Adafruit_GPS.h> //name of GPS Library
#include <SoftwareSerial.h>


//BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


//RTD Sensor
#include <Adafruit_MAX31865.h>


//Create BME Object
Adafruit_BME280 bme;


//Define TNC ports
SoftwareSerial port1(30, 32); // RX, TX on TNC


//Create GPS library Object
Adafruit_GPS GPS(&Serial3);


//declaring the CS, DI, DO, CLK for RTD
Adafruit_MAX31865 maxtemp = Adafruit_MAX31865(10, 37, 38, 39);


//Resistance Thermal Detector

//the value of the Rref resistor.
#define RREF      430.0
//the 'nominal' 0-degrees-C resistance of the sensor??
#define RNOMINAL  100.0


//Altimeter include
#include "intersemaBaro.h"
Intersema::BaroPressure_MS5607B baro(true);


//Camera
#include <ArduCAM.h>
#include "memorysaver.h"

#if !(defined OV2640_MINI_2MP)
#error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif
#define SD_CS 11
const int SPI_CS = 4;
int count = 0;
#if defined (OV2640_MINI_2MP)
ArduCAM myCAM( OV2640, SPI_CS );
#endif
