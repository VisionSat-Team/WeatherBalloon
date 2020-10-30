#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

// set pin 7 as the slave select for the digital pot:
const int CS = 7;
bool is_header = false;
ArduCAM myCAM(OV5642, CS);
uint8_t read_fifo_burst(ArduCAM myCAM);

void setup() {
  delay(5000);
  
  //temporary var
  uint8_t temp;
  
  Serial.begin(115200);
  
  Serial.println(F("ACK CMD ArduCAM Start! END"));
  
  // set the CS as an output:
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  
  // initialize SPI:
  SPI.begin();

  //Check if the ArduCAM SPI bus is OK
  while(1) {
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    
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
  delay(1000); 
  
  takePicture();
  Serial.println();
}

void loop() {
}

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
