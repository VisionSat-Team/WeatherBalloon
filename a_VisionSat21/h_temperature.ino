/*
Quoting (author)DrHaney's GitHub, this library:
...converts a Pt100 temperature sensor resistance into degrees Celsius
using a lookup table taken from empirical data in the DIN 43760 / IEC 751 document.
This library's conversion accuracy is authoritative such that other purely computational
methods may be validated against it.
*/

void checkFault(void) {
  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    thermo.clearFault();
  }
}

// This method calculates and returns the temperature looked up by DrHaney's library.
float lookUpTemperature(){

  uint16_t rtd = thermo.readRTD();
  uint32_t dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  
  // This is the actual resistance of the RTD times 100
  uint16_t ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;  
  return PT100.celsius(ohmsx100) ;
}
