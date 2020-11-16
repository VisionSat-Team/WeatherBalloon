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

// Libraries @@@@@
#include <SPI.h>            // SPI 
#include <SD.h>             // SD Card 
#include <MicroNMEA.h>      // GPS
#include <SoftwareSerial.h> // TNC 

// Timer Variables
int secondsPast; // Number of Seconds since last read

// Sensor Data String declaration
String temperatureData; // Individual Sensor data
String GPSData;         // ^^
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

  // Initialize Timer1
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
}

/************************** MAIN LOOP **************************/
void loop() {
  // Poll the Timer_Flag
  if (secondsPast >= 60) {
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
  }
}

/************************** FUNCTIONS **************************/
// Interrupt Service Routine
ISR(TIMER1_COMPA_vect) { // timer compare interrupt service routine
  secondsPast += 4;
}

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
  GetTemperature();
  GetGPS();
  GetAltimeter();
  GetTime();

  SensorData = timeData + "," + temperatureData + "," + GPSData + "," + altimeterData;
}
void GetTemperature() { // Updates String temperatureData
  // Read Sensor Data

  // String Building: Save temperatureData to desired value
  // temperatureData = @@@@@;
}
void GetGPS() {         // Updates String GPSData
  // Read Sensor Data

  // String Building: Save GPSData to desired value
  // GPSData = @@@@@;
}
void GetAltimeter() {   // Updates String altimeterData
  // Read Sensor Data

  // String Building: Save GPSData to desired value
  // altimeterData = @@@@@;
}
void GetTime() {        // Updates String timeData
  // Read Sensor Data

  // String Building: Save timeData to desired value
  // timeData in form of: hhmmss
  // timeData = @@@@@;
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
