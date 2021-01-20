/*
    MS5xxx.h - Library for accessing MS5xxx sensors via I2C
    Copyright (c) 2012 Roman Schmitz
    This file is part of arduino-ms5xxx.
    arduino-ms5xxx is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    arduino-ms5xxx is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with arduino-ms5xxx.  If not, see <http://www.gnu.org/licenses/>.
*/

double getAltitude( double pres, double pres_ref, double temp )
{
  double a1 = pow( ( pres_ref / pres ), ( 1 / 5.257 ) );
  double a = (a1 - 1) * ( temp + 273.15 ) / 0.0065f;
  return a;
}


void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
    //Serial.println(receivedChar);
  }
}

//void printJson()
//{
//  sensor.ReadProm();
//  sensor.Readout();
//  temperature = sensor.GetTemp() / 100.0f;
//  pressure = sensor.GetPres();
//  altitude = getAltitude( pressure, p_ref, temperature );
//
//  Serial.print("{\"temp\": ");
//  Serial.print(temperature);
//  Serial.print(", \"pres\": ");
//  Serial.print(pressure);
//  Serial.print(", \"alt\": ");
//  Serial.print(altitude);
//  Serial.println("}");
//}

void printSerial()
{
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  temperature = pressureSensor.GetTemp() / 100.0f;
  pressure = pressureSensor.GetPres();

  if (meanPressure == 0)
  {
    // On the first run "prefill" the mean value as the current value
    meanPressure = pressure * numSamples;
  }
  else
  {
    meanPressure = meanPressure - (meanPressure / numSamples) + pressure;
    pressure = meanPressure / numSamples;
  }
  altitude = getAltitude( pressure, p_ref, temperature );

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("  Pres: ");
  Serial.print(pressure);
  Serial.print("  Alt: ");
  Serial.print( altitude );
  Serial.println("");
}

void test_crc() {
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  Serial.print("CRC=0x");
  Serial.print(pressureSensor.Calc_CRC4(), HEX);
  Serial.print(" (should be 0x");
  Serial.print(pressureSensor.Read_CRC4(), HEX);
  Serial.print(")\n");
  Serial.print("Test Code CRC=0x");
  Serial.print(pressureSensor.CRCcodeTest(), HEX);
  Serial.println(" (should be 0xB)");
}


double getPressure() {
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  pressure = pressureSensor.GetPres();

  if (meanPressure == 0) {
    // On the first run "prefill" the mean value as the current value
    meanPressure = pressure * numSamples;
  }
  else {
    meanPressure = meanPressure - (meanPressure / numSamples) + pressure;
    pressure = meanPressure / numSamples;
  }
  return pressure;
}

double getAltimeter() {
  // Read Sensor Data
  double R = 8.314472, m = 28.97, g = 9.80665;
  double temp = lookUpTemperature();
  Serial.println(temp * 9 / 5 - 459.67);
  return log(p_ref / getPressure()) * (R * temp) / (m * g);
  // String Building: Save GPSData to desired value
  // altimeterData = @@@@@;
}
