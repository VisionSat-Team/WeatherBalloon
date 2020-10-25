/*
   BME280 Functions
*/

//ConvertPressureTemperature


//  BME280  //

/*
//Returns temperature inside the box. (Farenheit)
float getTemperature()
{
  return baro.readTemperature();
}

//Returns Pressure. (Pascals)
float getPressure()
{
  return baro.readPressure();
}

int getBackupAltitude()
{
  int alt = baro.getHeightCentiMeters();
  return alt;
  //return (float)(alt) / 30.48;
}
*/

//Returns temperature inside the box. (Farenheit)
float getTemperature()
{
  return bme.readTemperature();
}

//Returns Pressure. (Pascals)
float getPressure()
{
  return bme.readPressure();
}

//Returns humidity. (Percentage)
float getHumidity()
{
  return bme.readHumidity();
}
