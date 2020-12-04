/************************* INFO & DOCUMENTATION *************************
   VisionSat2020 Weather Balloon Project
   Weather Balloon Payload Script

   Sensors:
    SD Card:      Adafruit MicroSD card breakout board
    Temperature:  Adafruit PT100 RTD and MAX31865 Temperature Sensor
    GPS:          Parallax SIM33EAU GPS Module
    Altimeter:    Parallax Altimeter Module MS5607
    Camera:       ArduCam OV5642 5MP Camera

   @@@@@: Indicates that this section is not yet complete
 ************************************************************************/
#include <pt100rtd.h>
#include <Wire.h>
#include <math.h>
#include <MS5xxx.h>
#include <Adafruit_MAX31865.h>
#include <Adafruit_GPS.h> //GPS 
#include <SoftwareSerial.h> 
SoftwareSerial serial(3, 2); 
Adafruit_GPS GPS(&serial); // GPS

// Using hardware SPI        :               CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(10, 11, 12, 13);

// The value of the Rref resistor. We are using the PT100 whose
// value is 430
#define RREF      430.0

// The 'nominal' 0-degrees-C resistance of the PT100 sensor
#define RNOMINAL  100.0

// init the Pt100 table lookup module from DrHaney's PT100RTD library
pt100rtd PT100 = pt100rtd() ;

#define MS5607_ADDRESS 0x76

char receivedChar;
bool newData = false;

MS5xxx pressureSensor(&Wire);
// Libraries @@@@@
#include <SPI.h>            // SPI 
#include <SD.h>             // SD Card 
#include <Adafruit_GPS.h>  // GPS
#include <SoftwareSerial.h> // GPS
#include <SoftwareSerial.h> // TNC 

// Timer Variables
unsigned long timerMilis = millis(); // Number of miliSeconds returned from arduino board
unsigned long beaconDelay;// the delay for the beacon 

// Sensor Data String declaration
String temperatureData; // Individual Sensor data
String gpsData;         // ^^
String altimeterData;   // ^^
String timeData;        // ^^
String SensorData;      // concatenated string of all sensor data
// @@@@@ Camera: String or Array?

// SD Card Variables
int SD_initCount;           // Used for Initializing SD Card
int SD_initCountLimit = 5;  // # of times to try initializing SD Card
boolean SD_init = true;     // True unless card Fails to initialize
String fileName;            // Name of File

// Chip Select Pins
const int chipSelect_SD = 10;

// TNC Variables
const byte zeroHex = (0x00);
// Header: Callsign and Start Characters
byte header[25] = {0xC0, zeroHex, 0xAE, 0x6A, 0xAA, 0x98, 0x40, 0x40, 0xF6, 0xAE, 0x6A, 0xAA,
                   0x98, 0x40, 0x40, 0x60, 0x86, 0xA2, 0x40, 0x40, 0x40, 0x40, 0x61, 0x03, 0xF0,
                  };
String TNC_message;   // Message Received from Ground Station
SoftwareSerial portTNC(10, 11); // RX, TX

/************************* SETUP *************************/
void setup() {
  // Open Serial Communications
  Serial.begin(9600);
   
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  Serial.begin(115200);
   
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
   
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
   
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
   
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
     
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  Serial.println(PMTK_Q_RELEASE); 
   
  thermo.begin(MAX31865_3WIRE);  // We are using a 3-wire RTD
  
   
  beaconDelay = 60000;
   
  numSamples = 20;
  meanPressure = 0;
  pressureSensor.setI2Caddr(MS5607_ADDRESS);
  if(pressureSensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(200);
    setup();}
   pressureSensor.ReadProm();
  pressureSensor.Readout();
  p_ref =  pressureSensor.GetPres();
  t_ref = lookUpTemperature();
  while (!Serial) {
    ;  // wait for serial port to connect
  }

  // Initialize TNC
  portTNC.begin(19200);// print to tnc

  // Initializing Sensors @@@@@
  // Initialize SD Card
  SD_initCount = 0; // Number of times that Card attempted to Initialize
  // If SD fails to initialize, keep trying SD_initCountLimit # of times
  while (!SD.begin(chipSelect_SD) && SD_initCount < SD_initCountLimit) {
    SD_initCount++; // Wait for SD Card to Initialize
  }
  // If card failed to initialize in while loop AND fails to initialize again
  if (!SD.begin(chipSelect_SD) && SD_initCount == SD_initCountLimit) {
    SD_init = false; // Card failed to initialize
  }

  /* Initialize Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 1024 prescaler
  TCCR1B |= (1 << CS10);    // ^^^
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  OCR1A = 0xF424;           // Max Output Compare = 4 seconds
  //(16MHz/1024prescale)*4 = 62,500 4 seconds
  TCNT1 = 0;                // Start Timer at 0
  interrupts();             // enable all interrupts
  */
}

/************************** MAIN LOOP **************************/
void loop() {
  // Poll the Timer_Flag
  if (timerMilis % beaconDelay == 0) {
    // Capture & Save Data
    CaptureData();
    SaveData();
    KeyUp(SensorData);

    // Reset Timer (secondsPast & TCNT1) to 0
    TCNT1 = 0;
    secondsPast = 0;
  }

  // Poll Serial Buffer for TNC Data
  if (portTNC.available() > 0) {
    //    Serial.println("Packet Recieved");
    TNC_message = portTNC.readStringUntil(59);  // Reads until ASCII#59 (;)
    TNC_message = TNC_message.substring(25);    // Starts reading after Header
    TNC_message.toLowerCase();            // convert TNC_message to lower case
    //    Serial.println(TNC_message);

    // Switch Case: allows types int & char @@@@@
    //    switch(TNC_message) {

    // Cut Wire
    //     case ' ':
    //      break;
    // Instant Capture
    // case "":
    //break;
    // Beacon: Send most recent Sensor Data
    // case "":
    //break;
    //    //Change delay: Set OCR1A
    // case "":
    //break;
    //}
     if(TNC_message.toLowerCase() == "cutdown") {
        cutDown();
        //You can include this later in the cutDown Fuction();
        KeyUp("Cutdown Command Recieved")
     }
     else if(TNC_message.toLowerCase() == "beacon"){
        /* 
           Sends the most recent data. 
           But it should be distinct 
        */
        KeyUp("Manual Beacon: " + SendData);
        
     }
     else if(TNC_message.substring(0,5).toLowerCase() == "delay")
        /* 
            Parses the TNC_message after the first 5 characters.
            The next characters should be the time for the delay in MilliSeconds 
            To change the delay variable "beaconDelay"
        */
        changeDelay(TNC_message.substring(5));
     }
     else if(TNC_message == "capture");
        //This our instant capture
        CaptureData();
     }  
     else 
        KeyUp("Command does not exist");
  }
}

/************************** FUNCTIONS **************************/
/* Interrupt Service Routine
ISR(TIMER1_COMPA_vect) { // timer compare interrupt service routine
  secondsPast += 4;
}
*/
void KeyUp(String SendMessage) {
  //Key up and put ax25 header.
  //portTNC leads into the TNC which ultimately transmits over the radio
  //first write the header (regular or forced) and then the message (see beacon() function)
  //0xC0 is the end character in ax.25
  portTNC.write(header, 25);  // Write Header
  portTNC.print(SendMessage); // Data to be returned
  portTNC.print(0xC0);        // C0 End of frame
}

// Sensor & Data Functions
void CaptureData() {
  //Travis
  //Fix the CaptureData and delete the global varibles except sensorData
  String temperatureData = String(GetTemperature());
  String gpsData = String(GetGPS());
  String altimeterData = (GetAltimeter());
  String timeData = (GetTime());

  SensorData = timeData + "," + temperatureData + "," + gpsData + "," + altimeterData;
}

/*
 * This method returns the temperature measured by the temperature sensor, in Farenheit.
 * Author: Emmanuel
*/
float GetTemperature() { 
  uint16_t rtd = thermo.readRTD();
  uint32_t dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF));
  dummy >>= 16 ;

  // This is the actual resistance of the RTD times 100
  uint16_t ohmsx100 = (uint16_t) (dummy & 0xFFFF);  

  return 32 + (PT100.celsius(ohmsx100) * (9 / 5.0f));
}




     
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
     
// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
     
{   
  thermo.begin(MAX31865_3WIRE);  // We are using a 3-wire RTD

  numSamples = 20;
  meanPressure = 0;
  pressureSensor.setI2Caddr(MS5607_ADDRESS);
  if(pressureSensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(200);
    setup();
  }
  
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  p_ref =  pressureSensor.GetPres();
  t_ref = lookUpTemperature();
}
     
     
     
uint32_t timer = millis();

{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
   
      void GetTime() {        // Updates String timeData
  // Read Sensor Data
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 1000) {
    timer = millis(); // reset the timer
/*  Serial.print("\nTime: ");
    Serial.print(GPS.hour -6, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); */
   
    // String Building: Save timeData to desired value
    // timeData in form of: hhmmss   
    timeData = String(GPS.hour -6, DEC) + ":" + String(GPS.minute, DEC) + ":" + String(GPS.seconds, DEC) + "." + String(GPS.milliseconds);
}

  string GetGPS() {         // Updates String GPSData
  // Read Sensor Data
     
     //If GPS module has a fix, line by line prints the GPS information
    if (GPS.fix) {
        // String Building: Save GPSData to desired value
  
  gpsData = String(GPS.latitude,4) + "N" + ";" + String(GPS.longitude,4) + "W";

    }
  }  }
}
double GetAltimeter() {   // Updates String altimeterData
  // Read Sensor Data
double R = 8.314472, m = 28.97, g = 9.80665;
       double temp = lookUpTemperature()
          Serial.println(temp*9/5-459.67);
  return log(p_ref / getPressure()) * (R * temp) / (m * g);
  // String Building: Save GPSData to desired value
  // altimeterData = @@@@@;
}
void checkFault(void) {
  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println('A'); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println('B'); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println('C'); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println('D'); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println('E'); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println('F'); 
    }
    thermo.clearFault();
  }
}
double getPressure() {
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  pressure = pressureSensor.GetPres();

  if (meanPressure == 0){
    // On the first run "prefill" the mean value as the current value
    meanPressure = pressure * numSamples;
  }
  else{
    meanPressure = meanPressure - (meanPressure / numSamples) + pressure;
    pressure = meanPressure / numSamples;
  }
  return pressure;
}

void GetPicture(){
   //Returns the picture 
   
}
void SaveData() {       // Save Sensor Data & Picture to SD Card 
  if (SD_init) { // If card successfully initialized in setup()
    // Save Sensor Data
    // Open Sensor .txt(Data_HourMinuteSecond): dhhmmss.txt
    fileName = "d" + timeData + ".txt";
    File CurrentFileSensor = SD.open(fileName, FILE_WRITE);
    CurrentFileSensor.println(SensorData);  // Save SensorData to Current Sensor .txt file
    CurrentFileSensor.close();              // Close .txt file

    // Save Picture @@@@@
    //    // Open Picure .txt File (Picture_HourMinuteSecond): p_hhmmss.txt
    //    File CurrentFilePicture = SD.open("SD.txt", FILE_WRITE);
    //    fileName = "p" + timeData;
    //
    //    if (SensorData) { // if file succeeds at opening
    //      // Save to flightLog .txt file
    //
    //      // Close flightLog .txt file
    //      CurrentFilePicture.close();
    //    }
  }
}
