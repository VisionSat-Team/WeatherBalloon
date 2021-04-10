/*
    altimeter

    Altimeter Functions:
      - getPressure(): returns pressure as a string 
*/

String getPressure()
{
  // Update Altimeter Data
  pressureSensor.ReadProm();
  pressureSensor.Readout();

  // Read & Return Pressure
  return String(pressureSensor.GetPres()); 
}
