/*
   Camera

   Camera Functions:
    - takePicture(String picFileName): 
        - begins process of taking picture and saving it
        - receives String picFileName, which is the name for the picture that is going to be saved 
    - read_fifo_burst(ArduCAM myCAM, String picFileName): 
        - traverses the FIFO buffer and saves the bytes to the text file
        - receives: 
          - an ArduCAM object, for reading the FIFO buffer
          - receives String picFileName, which is the name for the picture that is going to be saved 
    - setCam(): resets camera registers so that another picture can be taken
        - returns if the reset was successful 
*/


void takePicture(String picFileName) {
  if (setCam() && SDAvailable) {
    uint8_t temp = 0xff, temp_last = 0;
    bool is_header = false;

    myCAM.flush_fifo();
    myCAM.clear_fifo_flag();

    //Start capture
    myCAM.start_capture();

    // Waiting for capture to be done
    Serial.print("waiting");
    while (!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)) {
      delay(10);
      Serial.print('.');
    }

    Serial.println(F("Capture Done")); delay(50);

    read_fifo_burst(myCAM, picFileName);

    //Clear the capture done flag
    myCAM.clear_fifo_flag();
  }
}


void read_fifo_burst(ArduCAM myCAM, String picFileName) {
  uint8_t temp = 0, temp_last = 0;
  uint32_t length = 0;
  uint32_t realLength = 0;
  length = myCAM.read_fifo_length();
  Serial.print("Fifo Length: "); Serial.println(length, DEC);

  // Error States
  if (length >= MAX_FIFO_SIZE) { // MAX_FIFO_SIZE 512KB for the OV5642
    Serial.println(F("Over size. ")); return;
  }
  if (length == 0 ) {
    Serial.println(F("Size is 0. ")); return;
  }

  // Prepare for capture
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();//Set fifo burst mode
  temp = SPI.transfer(0x00);
  length--;

  // Create File to be saved
  File picFile = SD.open(picFileName, FILE_WRITE);

  while ( length-- ) {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    if (is_header == true) {
      // Testing: Print Bytes out as the FIFO buffer is read
      Serial.print(','); Serial.print(temp, DEC);

      // Print Bytes to the picFile as they are read from the buffer
      picFile.print(','); picFile.print(temp, DEC);
      realLength++;
    }

    // End of image
    else if ((temp == 0xD8) && (temp_last == 0xFF)) {
      is_header = true;
      Serial.println(F("IMG END"));
      Serial.print(temp_last, DEC); Serial.print(',');
      Serial.print(temp, DEC);
      realLength = realLength + 2;
    }

    if ((temp == 0xD9) && (temp_last == 0xFF)) {
      break; // exit while loop
    }
    delayMicroseconds(15);
  }

  // Close the picture file
  picFile.close();

  // Testing
  //  Serial.println();
  //  Serial.print("Real Length: "); Serial.println(realLength, DEC);

  myCAM.CS_HIGH();
  is_header = false;
}


boolean setCam() {
  boolean spi, module, camReady = false;

  uint8_t temp;   //  temporary var
  uint8_t vid, pid;

  // initialize SPI:
  SPI.begin();
  Wire.begin();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  //Serial.println(temp);
  if (temp != 0x55) {
    Serial.println(F("SPI interface Error! END"));
    delay(1000);
  }
  else {
    spi = true;
  }

  //Check if the camera module type is OV5642
  myCAM.wrSensorReg16_8(0xff, 0x01);
  myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
  if ((vid != 0x56) || (pid != 0x42)) {
    Serial.println(F("Can't find OV5642 module! END"));
    delay(1000);
  }
  else {
    module = true;
  }
  if (spi && module == true) {
    Serial.println("Camera is set");
    camReady = true;
  }

  //Change to JPEG capture mode
  myCAM.set_format(JPEG);

  // Initialize Camera
  myCAM.InitCAM();
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);    // VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);       //  resolution

  return camReady;
}
