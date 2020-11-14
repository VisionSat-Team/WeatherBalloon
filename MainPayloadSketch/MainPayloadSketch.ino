/* VisionSat2020 Weather Balloon Project
   Main Payload Script    */

// Libraries *****
#include <SPI.h>
#include <SD.h>
#include <MicroNMEA.h>  // Get current time from GPS 

// Interrupt Pins *****
const int interrupt0Pin = 2; // interrupt0: Cut Wire
const int interrupt1Pin = 3; // interrupt1: Timer Interrupt, Capture Data

// Sensor Data String declaration
String temperatureData; // Individual Sensor data
String GPSData;         // ^^
String altimeterData;   // ^^
String timeData;        // ^^ 
String SensorData;      // concatenated string of all sensor data 

// Timer Flag: After 1 minute has passed, Interrupt sets goCapture to true 
volatile boolean Timer_Flag = false;

void setup() {
  //Declaring Interrupts
  attachInterrupt(digitalPinToInterrupt(interrupt0Pin), CutWire, HIGH);
  attachInterrupt(digitalPinToInterrupt(interrupt1Pin), Timer_Interrupt, HIGH);
  // Initializing Sensors *****
}

void loop() {
  // Poll the Timer_Flag 
  if (Timer_Flag) {
    CaptureData(); 
    SaveData(); 
    
    Timer_Flag = false; 
  }
}

// Interrupt Service Routines 
void Timer_Interrupt () { // 1 Minute has passed, set Timer_Flag to True
  Timer_Flag = true; 
}

void CutWire() {        // Command from ground station: Cut Wire, Detach Payload
  // Send balloon cut command 
}

// Sensor & Data Functions 
void CaptureData() {
  GetTemperature();
  GetGPS();
  GetAltimeter();
  GetTime(); 
  
  SensorData = timeData + "," + temperatureData + "," + GPSData + "," + altimeterData;
}
void GetTemperature() { 
  // Read Sensor Data

  // String Building: Save temperatureData to desired value
  // temperatureData = ******; 
}
void GetGPS() { 
  // Read Sensor Data

  // String Building: Save GPSData to desired value
  // GPSData = ******; 
}
void GetAltimeter() { 
  // Read Sensor Data

  // String Building: Save GPSData to desired value
  // altimeterData = ******; 
}
void GetTime() { 
  // Read Sensor Data

  // String Building: Save timeData to desired value
  // timeData = ******; 
}
void SaveData() { 
  
}
