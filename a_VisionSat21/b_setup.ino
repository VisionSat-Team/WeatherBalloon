/*
    setup

    Set up sensors, initialize global variables, etc.
*/

void setup() {

  // General START
   beaconDelay[0] = 30;
  beaconDelay[1] = 45;
  beaconDelay[2] = 60; // 60 second delay between beacons
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
  //Send a SetNav string to command the GPS to set it in "Airborne Mode" to be used above 100,000 feet.
  //-------------------------------------------------------------------------------------------------------------------------------------------------
  unsigned char setNav[] = {
    0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00,
    0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC
  };

  //  Set uBlox MAX series to Airborne Mode.
  for (int i = 0; i < 44; i++) {
    Serial3.write(setNav[i]);
  }
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
  // SD Card END


  //TNC Start
  port1.begin(19200);
  //TNC END
  
  //APRS Start
  int aprsReadPin = 38; 
  int powerRelayPin = 39;
  pinMode(aprsReadPin, INPUT);
  pinMode(powerRelayPin, OUTPUT);
  //APRS END

  // Cut Down START
  pinMode(burnWirePin, OUTPUT);
  digitalWrite(burnWirePin, LOW);
  // Cut Down END


  //Testing
  ResetEEPROM();
  beaconDelay[0] = 30;
  beaconDelay[1] = 45;
  beaconDelay[2] = 60;
  whichDelay = 0;
  Serial.println("*** End of setup ***");
  Serial.println(); 
}
