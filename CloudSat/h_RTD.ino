/*
   Resistance Thermal Detector Functions
*/


//  PT100 //

//Returns temperature outside of the box. (Farenheit)
float getExternalTemperature()
{
  //reading the resistance.
  uint16_t rtd = maxtemp.readRTD();
  float ratio = rtd;
  ratio /= 32767;

  uint8_t fault = maxtemp.readFault();
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
    maxtemp.clearFault();
  }

  return maxtemp.temperature(RNOMINAL, RREF) * 1.8 + 32;
}
