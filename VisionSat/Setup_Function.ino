/* Setup Tab
 *  Setup for all sensors */

void setup() { 
  sensorData = ""; 
  beaconDelay = 60000; 
  prev = micros(); 
  currentDataSaved = true; 
  Serial.begin(115200);

  //  Altimeter Module Initialization START
  numSamples = 20;  meanPressure = 0;
  pressureSensor.setI2Caddr(MS5607_ADDRESS);
  if (pressureSensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(200);
    setup();
  }
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  p_ref =  pressureSensor.GetPres();
  t_ref = lookUpTemperature();
  //  Altimeter Module Initialization END

  // Camera Initialization START
  uint8_t temp;   //  temporary var

  Serial.println(F("ACK CMD ArduCAM Start! END"));

  pinMode(cameraCS, OUTPUT);    // set the CS as an output:
  digitalWrite(cameraCS, HIGH);

  SPI.begin();    // initialize SPI:

  while (1) { //  Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);

    if (temp != 0x55) {
      Serial.println(F("ACK CMD SPI interface Error! END"));
      delay(1000); continue;
    } 
    else {
      Serial.println(F("ACK CMD SPI interface OK. END"));
      break;
    }
  }

  //Change to JPEG capture mode and initialize the OV5642 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();

  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);    // VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);       //  resolution
  // Camera Initialization END

  //  Initialize SD Card START
  SD_initCount = 0; // Number of times that Card attempted to Initialize
  SD_init = true;   // Assume that card succeeded to initialize 
  // If SD fails to initialize, keep trying 5 times
  while (!SD.begin(chipSelect_SD) && SD_initCount < 5) {
    SD_initCount++; // Wait for SD Card to Initialize
  }
  // If card failed to initialize in while loop AND fails to initialize again
  if (!SD.begin(chipSelect_SD) && SD_initCount == SD_initCountLimit) {
    SD_init = false; // Card failed to initialize
  }
  //  Initialize SD Card END
  
  // Initialzie GPS START 
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  // Initialize GPS END

  /*  Initialise Temperature sensor START */
  thermo.begin(MAX31865_3WIRE);
  /*  Initialise Temperature sensor END */
  
}
