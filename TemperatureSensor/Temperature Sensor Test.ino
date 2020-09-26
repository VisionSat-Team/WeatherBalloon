#include <pt100rtd.h>
#include <Adafruit_MAX31865.h>

// Using software SPI        :               CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(10, 11, 12, 13);

// The value of the Rref resistor. We are using the PT100 whose
// value is 430
#define RREF      430.0

// The 'nominal' 0-degrees-C resistance of the PT100 sensor
#define RNOMINAL  100.0

// init the Pt100 table lookup module from DrHaney's PT100RTD library
pt100rtd PT100 = pt100rtd() ;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  Serial.println("Adafruit Temperature, LookUp Temperature");

  thermo.begin(MAX31865_3WIRE);  // We are using a 3-wire RTD
}

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

void loop() {
  float adafruitTemp, lookupTemp;
    
  adafruitTemp = thermo.temperature(RNOMINAL, RREF); // Adafruit's custom library calculated temperature

  lookupTemp = lookUpTemperature(); // DrHaney's LookUp Table temperature

  // Maintaining CSV format
  Serial.print(adafruitTemp);  Serial.print(" , ");  Serial.print(lookupTemp);

  checkFault();  // Checks for faults
  
  Serial.println();
  delay(1000);
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
