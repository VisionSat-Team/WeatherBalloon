/*
   Global Variables
*/


//  Communication   //

//Replaces 0x00 because arduino is weird
const byte zeroHex = (0x00);

//Used to place incoming integers in "command" string to be read
int in;

//String for final command
String command = "";

//Parsed command
String commandParsed = "";

//Controls beacon interval
int beaconInterval = 0;

//AX.25 header used in KEY_UP functions
byte header[25] = {0xC0, zeroHex, 0xAE, 0x6A, 0xAA, 0x98, 0x40, 0x40, 0xE0, 0xAE, 0x6A, 0xAA,
                   0x98, 0x40, 0x40, 0x76, 0xAE, 0x92, 0x88, 0x8A, 0x64, 0x40, 0x65, 0x03, 0xF0
                  };

//AX.25 header used in KEY_UP_IGATE function
byte headerIGATE[25] = {0xC0, zeroHex, 0x96, 0x8C, 0x6A, 0x84, 0x9C, 0x98, 0xEE, 0xAE, 0x6A, 0xAA,
                        0x98, 0x40, 0x40, 0x76, 0xAE, 0x92, 0x88, 0x8A, 0x64, 0x40, 0x65, 0x03, 0xF0
                       };

//Title for data string used in beacon() function
const String title = "W5UL-11,CloudSat8++";

//Title for data string used for FORCED beacons
const String titleForced = "W5UL-11,CloudSat8++FORCED";

//Message of acknowledgement
const String ack = "Acknowledged";


//  General  //

//Has balloon been cutdown?
bool cutDownStatus = 0;

//Are buzzers on?
float buzzers = 0;

//Buzzer pin for 1st set of buzzers
const int scream_1 = 48;

//Buzzer pin for 2nd set of buzzers
const int scream_2 = 49;

//Cutdown pin
const int cutdownPin = 46;


//  Timing  //

//Time since start
long int currentMillis = 0;

//Previous time (milliseconds), used to calculate rate of ascent
long int previousTime = 0;

//Time at which the last picture was taken (milliseconds)
long int lastPicTaken = 0;

//Time at which payload last beaconed (milliseconds)
long int lastBeacon = 0;

//Time at which payload last tweeted (milliseconds)
long int lastTweet = 0;


//  GPS  //

//Create char to read the NMEA characters as they come in
char readNMEA;

//Previous altitude (Feet)
float previousAltitude = 0;

//Current altitude of balloon (Feet)
float currentAltitude = 0;

//The rate at which the balloon is climbing
float ascentRate = 0;

//Current latitude (Decimal Degrees)
float latitude = 0;

//Current longitude (Decimal Degrees)
float longitude = 0;

//Current speed of balloon (Knots)
float currentSpeed = 0;

//Time recorded from GPS module (UTC - 5)
String timeUTC = "";

//Number of satellites currently locked onto GPS module
int satellites = 0;


//  BME  //

//Current humidity (Percentage)
float humidity = 0;

//Current temperature (Fahrenheit)
float temperature = 0;

//Current pressure (Pascals)
float pressure = 0;


//  Hall Effect  //

//HallEffectSensor pin
const int hallSensor = 42;

//Number of positive readings for hall effect sensor
int positiveReadings = 0;

//Field y/n
bool field = false;


// Camera  //

//Current number of pictures taken
int picturesTaken = 0;

//Flag that allows the camera to take and save pictures
bool cameraAvailable = true;

//Flag that allows the camera to take and save pictures
bool SdCardAvailable = true;

//Flag that allows the camera to take and save pictures
bool SpiInterfaceAvailable = true;

//Flag that allows the camera to take and save pictures
bool commandCameraToggle = true;


//RTD

//External temperature from RTD
float externalTemp = 0;


//Parallax altimeter

//Backup altitude reading
float secondAltitude = 0;


//Message to be tweeted
String tweet = "*TWEET    Temperature = " + String(temperature, 2) + " Degrees Fahrenheit." + " Altitude = " + String(currentAltitude, 2) + " Feet." + " Pressure = " + String(pressure, 2) + " Pascals.";
