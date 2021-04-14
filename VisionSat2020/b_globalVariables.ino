/* 
 *  globalVariables 
 *  
 *  Global Variable Declaration 
 */

// General START
long beaconDelay;
long previousMillis; // Last Beacon Time
// General END


// GPS START 
//SoftwareSerial serial(12, 11); // RX, TX
Adafruit_GPS GPS(&Serial3);
int centralStandardTime;
#define PMTK_FR_MODE "$PMTK886,3*2B"
// GPS END 


// Altimeter START 
#define USE_BLUETOOTH false  // set to false to communicate via seria port
#define MS5607_ADDRESS 0x76
MS5xxx pressureSensor(&Wire);
// Set the reference pressure to the current pressure to get relative altitude changes
double p_ref, meanPressure, numSamples;
char receivedChar;
bool newData = false;
// Altimeter END 


// Temperature START 
// Using Hardware SPI (Sensor --> Mega Pin #) cs --> 10, SDI --> MOSI (51), SDO --> MISO (50), CLK --> SCK (52)
Adafruit_MAX31865 thermo = Adafruit_MAX31865(9);
// The value of the Rref resistor. We are using the PT100 whose value is 430
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the PT100 sensor
#define RNOMINAL  100.0
// init the Pt100 table lookup module from DrHaney's PT100RTD library
pt100rtd PT100 = pt100rtd();
// Temperature END 


// Camera Variables START
const int cameraCS = 7; 
ArduCAM myCAM(OV5642, cameraCS);
bool is_header = false;
uint8_t read_fifo_burst(ArduCAM myCAM, String picFileName);
// Camera Variable END 


// SD Card START 
const int SD_CS = 53; 
const int dataSaveCountAddress = 0; 
const int dataSaveRolloverAddress = 1; 
boolean SDAvailable = false; 
// SD Card END 


// TNC START
int zeroHex = 0x00;
byte header[25] = {0xC0, zeroHex, 0xAE, 0x6A, 0xAA, 0x98, 0x40, 0x40, 0xF6, 0xAE, 0x6A, 0xAA,
                   0x98, 0x40, 0x40, 0x60, 0x86, 0xA2, 0x40, 0x40, 0x40, 0x40, 0x61, 0x03, 0xF0};
String tncMessage;
SoftwareSerial port1(10,11); // RX, TX
// TNC END 


// Cut Down START
const int burnWirePin = 27; 
boolean isCutDown = false; 
// Cut Down END
