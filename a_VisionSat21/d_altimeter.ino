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

double altitudeFormula( double pres, double pres_ref, double temp )
{
  double a1 = pow( ( pres_ref / pres ), ( 1 / 5.257 ) );
  double a = (a1 - 1) * ( temp + 273.15 ) / 0.0065f;
  return a;
}




double getAltitude()
{
  pressureSensor.ReadProm();
  pressureSensor.Readout();
  temperature = lookUpTemperature();
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
  altitude = altitudeFormula( pressure, p_ref, temperature );
  
  return altitude;
}
