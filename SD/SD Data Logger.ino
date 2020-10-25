/* VisionSat 2020 
 * SD Write File 
 * 
 * Wiring (Chip -> Arduino): 
 *  5V -> 5V
 *  GRND -> GRND 
 *  CLK -> 13 (SCK)
 *  DO -> 12 (MISO)
 *  DI -> 11 (MOSI)
 *  CS -> 10
*/

// Libraries 
#include <SPI.h>
#include <SD.h>
#include <MicroNMEA.h>  // Get current time from GPS 

const int chipSelect = 10; // Chip Select for SD Card

// Sensor Data String declaration
String SensorData; 
String temperatureData; 
String GPSData; 
String altimeterData; 

// Testing Variables 
String dummyTest; //Test Variable 
int testCount = 0;

void setup() {
  // Open serial communications and wait for port to open
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed to initialize");
    while (1); // Wait here forever
  }
  Serial.println("Card initialized.");
}

void loop() {
  /* Sensor Data read and String Creation 
    temperatureData = **getter**; 
    GPSData = **getter**; 
    altimeterData = **getter**; 
    
    SensorData = temperatureData + " " + GPSData + " " + altimeter; 
   */

  /* File names are not case sensitive.
   * Files will fail to open if name is too long, so keep them as short as possible. 
   * If no file exists with given name, a file is created. */ 
  File dataFile = SD.open("vs_dl.txt", FILE_WRITE); 

  if (dataFile) {
    /* //Test Code 
    dummyTest = "sup baw";
    dataFile.println(dummyTest); // + " " + testCount
    Serial.println(dummyTest);
    
    // Time Stamp (To be addes using GPS data)
    
    */


    // Real Project code
//    dataFile.println(SensorData); 

    dataFile.close();   // close each loop to make sure data saves
  }
  else {    // if the file isn't open, pop up an error:
    Serial.println("error opening VS_DL.txt");
  }
   
  delay(1000); 
  
  testCount++;
  if (testCount >= 3) { 
    Serial.println("STOP"); 
    while(1); 
  }
}
