/*  
 *    GPS Set to Balloon Mode
 *    Setting Ublox 7 GPS module to ballon mode with UBX commads 
 */

#include "Adafruit_GPS.h"
Adafruit_GPS GPS(&Serial3);


void setup() {
  Serial.begin(9600);
  GPS.begin(9600);

  Serial.println("GPS TEST START");

  //Send a SetNav string to command the GPS to set it in "Airborne Mode"
  unsigned char setNav[] = {
    0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00,
    0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC};
  for (int i = 0; i < 44; i++) {
    Serial3.write(setNav[i]);
  }

  //Send a CheckNav string to command the GPS to return the current settings. Used to check if the GPS is in "Airborne Mode".
  unsigned char checkNav[] = {0xB5, 0x62, 0x06, 0x24, 0x00, 0x00, 0x2A, 0x84};
  for (int i = 0; i < sizeof(checkNav); i++) {
    Serial3.write(checkNav[i]);
  }

  Serial.println("Checking for ACK...");
  delay(1000);
  while (Serial3.available() > 0)
  {
    Serial.print(Serial3.read(), HEX); // Prints Bytes 
    //    Serial.print(GPS.read()); // Prints characters
    Serial.print(" ");
  }
  Serial.println();
}


uint32_t timer = millis();
void loop()
{
  // Print out characters as they are read 
  char c = GPS.read();
  if (c) Serial.print(c);

  if (GPS.newNMEAreceived()) {
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
}
