void takePicture() {
  // put your main code here, to run repeatedly:
  uint8_t temp = 0xff, temp_last = 0;
  bool is_header = false;
  
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  
  //Start capture
  myCAM.start_capture();
  
  // Waiting for capture to be done (polling)
  Serial.print("waiting");
  while (!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)) {
    delay(10);
    Serial.print('.');
  }
    
  Serial.println(F("ACK CMD CAM Capture Done. END")); delay(50);
  
  read_fifo_burst(myCAM);
  
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
    
}

uint8_t read_fifo_burst(ArduCAM myCAM) {
  uint8_t temp = 0, temp_last = 0;
  
  uint32_t length = 0;
  uint32_t realLength = 0;
  length = myCAM.read_fifo_length();
  Serial.print("Fifo Length: "); Serial.println(length, DEC);

  // 512KB for the OV5642
  if (length >= MAX_FIFO_SIZE) {
    Serial.println(F("ACK CMD Over size. END"));
    return 0;
  }
  if (length == 0 ) {
    Serial.println(F("ACK CMD Size is 0. END"));
    return 0;
  }
  
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();//Set fifo burst mode
  temp = SPI.transfer(0x00);
  length--;
  
  while ( length-- ) {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    if (is_header == true){
      Serial.print(','); Serial.print(temp, DEC); 
      realLength++;
    }
    else if ((temp == 0xD8) && (temp_last == 0xFF)) {
      is_header = true;
      Serial.println(F("ACK IMG END"));
      Serial.print(temp_last, DEC); Serial.print(',');
      Serial.print(temp, DEC);
      realLength = realLength + 2;
    }
    if ((temp == 0xD9) && (temp_last == 0xFF)) {
      break;
    } 
    delayMicroseconds(15);
  }

  Serial.println();
  Serial.print("Real Length: "); Serial.println(realLength, DEC);
  
  myCAM.CS_HIGH();
  is_header = false;
  return 1;
}
