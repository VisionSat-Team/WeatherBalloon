/* Setup Tab
 *  Setup and Initialize all sensors */

void setup() { 
  
  // General START
  fileName = "setup"; 
  beaconDelay = 5000; 
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial2.begin(115200);
  Serial.println("setup started"); // Testing
  previousMillis = 0; 
  // General END 

  //TNC Start
  port1.begin(19200);
  //TNC END


  // Temperature START
  thermo.begin(MAX31865_3WIRE);
  // Temperature END 

  
  //  Altimeter START
  int numSamples = 20;  
  int meanPressure = 0;
  pressureSensor.setI2Caddr(MS5607_ADDRESS);
  if (pressureSensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(200);
//    setup();
  }
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  p_ref =  pressureSensor.GetPres();
  t_ref = lookUpTemperature();
  //  Altimeter END


  // Camera START
  delay(5000); 
  
  uint8_t temp;   //  temporary var

  Serial.println(F("ACK CMD ArduCAM Start! END"));

  pinMode(cameraCS, OUTPUT);    // set the CS as an output:
  digitalWrite(cameraCS, HIGH);

  SPI.begin();    // initialize SPI:
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  while(1) {
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    serial.
    if (temp != 0x55){
      Serial.println(F("ACK CMD SPI interface Error! END"));
      delay(1000); continue;
    } else {
      Serial.println(F("ACK CMD SPI interface OK. END"));
      break;
    }
  }

  //Change to JPEG capture mode and initialize the OV5642 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();

  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);    // VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);       //  resolution
  // Camera END


  //  SD Card START  
  SD.begin(SD_CS); 
  fileName = "setup";
  lastFileName = "setup";
  duplicate = 0;
  // SD Card END   

  
  // GPS START 
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PGCMD_ANTENNA);
  GPS.sendCommand(PMTK_FR_MODE);
  delay(1000);
  // GPS END

   
  Serial.println("setup finished"); // Testing

  delay(beaconDelay);
}
