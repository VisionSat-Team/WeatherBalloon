long timerMillis;



long beaconDelay = 5000;

String sensorData= "kkugbi";

int centralStandardTime;

// TNC START
int zeroHex = 0x00;
byte header[25] = {0xC0, zeroHex, 0xAE, 0x6A, 0xAA, 0x98, 0x40, 0x40, 0xF6, 0xAE, 0x6A, 0xAA,
                   0x98, 0x40, 0x40, 0x60, 0x86, 0xA2, 0x40, 0x40, 0x40, 0x40, 0x61, 0x03, 0xF0};

String tncMessage;
// TNC END 

#define USE_BLUETOOTH false  // set to false to cummunicate via seria port
#define MS5607_ADDRESS 0x76

MS5xxx pressureSensor(&Wire);

// Set the reference pressure to the current pressure to get relative altitude changes
double pressure, temperature, p_ref, t_ref, altitude, meanPressure, numSamples;

char receivedChar;
bool newData = false;

// Using software SPI        :               CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(10, 11, 12, 13);

// The value of the Rref resistor. We are using the PT100 whose
// value is 430
#define RREF      430.0

// The 'nominal' 0-degrees-C resistance of the PT100 sensor
#define RNOMINAL  100.0

// init the Pt100 table lookup module from DrHaney's PT100RTD library
pt100rtd PT100 = pt100rtd() ;



SoftwareSerial serial(3, 2);
Adafruit_GPS GPS(&serial);



// Camera Variables START
const int cameraCS = 7; 
ArduCAM myCAM(OV5642, cameraCS);
bool is_header = false;
uint8_t read_fifo_burst(ArduCAM myCAM);
// Camera Variable END 


// SD Card START 
boolean SD_init; 
const int SD_CS = 53; 
int SD_initCount; 
// SD Card END 
