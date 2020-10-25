/*
   Hall Effect Functions
*/


//  Hall Effect  //

//Reads high in case of magnetic field.
boolean isField()
{
  return digitalRead(hallSensor) == HIGH;
}
