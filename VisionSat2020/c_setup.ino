/*
    setup

    Set up sensors, initialize global variables, etc.
*/

void setup() {

  // General START
  beaconDelay = 60000; // 60 second delay between beacons
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial2.begin(115200);
  previousMillis = 0;
  Serial.println(); 
  // Start Communication
  Wire.begin();
  SPI.begin();
  // Slow down SPI transfer rate (for Camera)
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  // Temporary variable for repeatedly attempting to initialize sentences
  int initTries = 5;
  // General END


  // GPS START
  // Try initializing sensor
  initTries = 5;
  while (!GPS.begin(9600) && initTries > 0) {
    initTries--;
  }
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PGCMD_ANTENNA);
  GPS.sendCommand(PMTK_FR_MODE);
  delay(1000);
  // GPS END


  //  Altimeter START
  pressureSensor.setI2Caddr(MS5607_ADDRESS);
  // Try initializing sensor
  initTries = 5;
  while (!pressureSensor.connect() && initTries > 0) {
    initTries--;
  }
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  p_ref =  pressureSensor.GetPres();
  //  Altimeter END

  // Temperature START
  thermo.begin(MAX31865_3WIRE);
  // Temperature END


  // Camera START
  pinMode(cameraCS, OUTPUT);
  // set the CS as an output:
  digitalWrite(cameraCS, HIGH);

  // Try initializing sensor
  initTries = 5;
  while (!setCam() && initTries > 0) {
    initTries--;
  }
  // Camera END


  //  SD Card START
  initTries = 5;
  while (!SDAvailable && initTries > 0) {
    SDAvailable = SD.begin(SD_CS);
    initTries--;
  }
  if (SDAvailable) { // *****
    Serial.println("SD init true");
  }
  // SD Card END


  //TNC Start
  port1.begin(19200);
  //TNC END


  // Cut Down START
  pinMode(burnWirePin, OUTPUT);
  digitalWrite(burnWirePin, LOW);
  // Cut Down END


  // Safe wait, get ready to go into main loop
  //  delay(5000);

  //Testing
  ResetEEPROM();
  beaconDelay = 1000;
  Serial.println("  end of setup");
}