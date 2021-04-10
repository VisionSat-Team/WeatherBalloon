/* 
 *  temperature 
 *  
 *  Temperature Functions; 
 *    - lookUpTemperature(): converts resistance on the sensor to 
 *    - toFarenheit(): converts Celsius to Farenheit 
 *    - checkFault(): 
 */

/*
Quoting (author)DrHaney's GitHub, this library:
...converts a Pt100 temperature sensor resistance into degrees Celsius
using a lookup table taken from empirical data in the DIN 43760 / IEC 751 document.
This library's conversion accuracy is authoritative such that other purely computational
methods may be validated against it.
*/


// This method calculates and returns the temperature looked up by DrHaney's library.
float lookUpTemperature(){
  uint16_t rtd = thermo.readRTD();
  uint32_t dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  
  // This is the actual resistance of the RTD times 100
  uint16_t ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;  
  return PT100.celsius(ohmsx100);
}


float toFarenheit(){
  return lookUpTemperature()*9/5+32;
}
