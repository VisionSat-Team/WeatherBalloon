/*
   Setup
*/


void setup() {

  picturesTaken = EEPROM.read(0);

  //set buzzer pins.
  pinMode(scream_1, OUTPUT);
  pinMode(scream_2, OUTPUT);


  //set cutdown pin.
  pinMode(cutdownPin, OUTPUT);


  //set baud rate  !!!MORE INFO NEEDED.
  Serial.begin(9600);
  Serial1.begin(19200);

  //Hardware Serial3, defines the baud rate used by GPS module.
  GPS.begin(9600);

  //port1 is used to write data to the TNC and ultimately be sent via Radio to the ground station.
  port1.begin(19200);



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
  //-------------------------------------------------------------------------------------------------------------------------------------------------



  //Send a CheckNav string to command the GPS to return the current settings. Used to check if the GPS is in "Airborne Mode".
  //-------------------------------------------------------------------------------------------------------------------------------------------------
  unsigned char checkNav[] = {0xB5, 0x62, 0x06, 0x24, 0x00, 0x00, 0x2A, 0x84};

  for (int i = 0; i < sizeof(checkNav); i++) {
    Serial3.write(checkNav[i]);
  }

  delay(500);

  while (Serial3.available() > 0)
  {
    Serial.println(Serial3.read());
  }
  //-------------------------------------------------------------------------------------------------------------------------------------------------


  //Set GPS to have an update rate of 10 hertz and only request RMC and GGA sentences.
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);


  //BME280 Setup
  if (!bme.begin()) {
    Serial.println("Couldn't find the BME280 sensor");
  }


  //RTD Setup
  //If you visually inspect the RTD in the CloudSat 8 payload, it uses 3
  //leads, thus "MAX31865_3WIRE" is neccessary for setup. 2 and 4 lead
  //versions of this sensor are available.
  maxtemp.begin(MAX31865_3WIRE); //set to 2WIRE or 4WIRE as necessary


  //Initialize altimeter
  baro.init();


  //Camera Setup
  uint8_t vid, pid;
  uint8_t temp;
  Wire.begin();
  SD.begin();
  Serial.println(F("ArduCAM Start!"));
  //set the CS as an output:
  pinMode(SPI_CS, OUTPUT);
  digitalWrite(SPI_CS, HIGH);
  //initialze SPI:
  SPI.begin();

  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);

  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);

  if (temp != 0x55) {
    Serial.println(F("SPI interface Error!"));
    SpiInterfaceAvailable = false;
    delay(1000);
  } else {
    Serial.println(F("SPI interface OK."));
    SpiInterfaceAvailable = true;
  }

  //Initialize SD Card
  if (!SD.begin(SD_CS)) {
    Serial.println(F("SD Card Error!"));
    SdCardAvailable = false;
    delay(1000);
  } else {
    Serial.println(F("SD Card detected."));
    SdCardAvailable = true;
  }

#if defined (OV2640_MINI_2MP)

  //Check if the camera module type is OV2640
  myCAM.wrSensorReg8_8(0xff, 0x01);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))) {
    Serial.println(F("Can't find OV2640 module!"));
    cameraAvailable = false;
    delay(1000);
  }
  else {
    Serial.println(F("OV2640 detected."));
    cameraAvailable = true;
  }

#endif
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
#if defined (OV2640_MINI_2MP)
  myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);
#endif
  delay(1000);


  //Create and add first line to dataLog file
  File dataLog;
  dataLog = SD.open("dataLog.csv", FILE_WRITE);
  if (dataLog)
  {
    Serial.print("Writing to dataLog.txt...");
    dataLog.println("CallSign ,Payload ,Latitude ,Longitude ,Altitude ,Rate of Ascent ,Speed ,Satellites ,UTCtime ,Pictures ,Humidity ,Pressure ,InternalTemp ,ExternalTemp ,Field ,CutDownStatus ,CameraToggle ,End");
    dataLog.close();
    Serial.println("Done");
  }
  else
  {
    Serial.println("Error with dataLog");
  }


  //Check SD Card CameraToggle variable
  resetCameraToggleCheck();
}
