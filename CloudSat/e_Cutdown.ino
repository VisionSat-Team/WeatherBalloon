/*
   Cutdown and Buzzers Functions
*/


//Cutdown balloon by sending power to pin 4 and severing connection from balloon to payload.
void cutDown()
{
  digitalWrite(cutdownPin, HIGH);
  cutDownStatus = true;
  delay(10000);
  digitalWrite(cutdownPin, LOW);
}


//Buzzes first set of buzzers for 1 second followed by the second set and repeats 5 times.
void buzz()
{
  for (int i = 5; i >= 1; i--)
  {
    digitalWrite(scream_1, HIGH);
    delay(1000);
    digitalWrite(scream_1, LOW);
    delay(1000);
    digitalWrite(scream_2, HIGH);
    delay(1000);
    digitalWrite(scream_2, LOW);
    delay(1000);
  }
}


void backupCutdown()
{
  //cutdown after 3 hours if the payload has yet to be cutdown.
  currentMillis = millis();
  if (currentMillis >= 10800000 && cutDownStatus == false)
  {
    cutDown();
  }

  //turn buzzers on after 3 hours or after cutdown
  if (currentMillis >= 10800000 || cutDownStatus == true)
  {
    buzz();
  }
}
