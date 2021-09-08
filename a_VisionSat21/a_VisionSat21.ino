#include <MS5611.h>
#include <MS5xxx.h>
#include <math.h>
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <pt100rtd.h>
#include <Adafruit_MAX31865.h>
#include <SD.h>
#include <memorysaver.h>
#include <MicroNMEA.h>  // Get current time from GPS 
#include <SoftwareSerial.h> 
#include <Adafruit_GPS.h>

boolean setCam();
 void ResetEEPROM();
 String captureData();
 void keyUp(String messageToGround);
 String newFileName();
 void saveData (String sensorData, String fileName);
 void takePicture(String picFileName);
 void captureGroundStation();
 float toFarenheit();
 float lookUpTemperature();
 String getLocation();
 void captureGPS();
 String getTime();
 String getPressure(); 
 String getFeetPerSec();
 void burnWire();

