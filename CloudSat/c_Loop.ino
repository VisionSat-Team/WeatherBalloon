/*
   Main Loop
*/


void loop() {

  //Radio data beacon back to ground on some interval. (15, 30, 45, 60 seconds -> 30 seconds default)
  radioBeacon();

  //
  Serial.println(beacon(title));
  //

  //Every commandCheck() is placed in order to constantly be checking for commands.
  commandCheck();

  //Radio tweets back to ground station IGATE.
  radioTweet();

  //Every commandCheck() is placed in order to constantly be checking for commands.
  commandCheck();

  //Automatically cutdown balloon after set amount of time.
  backupCutdown();

  //Every commandCheck() is placed in order to constantly be checking for commands.
  commandCheck();

  delay(1000);

  //Every commandCheck() is placed in order to constantly be checking for commands.
  commandCheck();

  //Collect data from sensors.
  collectData();

  if(Serial.read() == 114)
  {
    Serial.println("RESETING EEPROM...");
    EEPROM.write(0, 0);
    Serial.println("WAITING 30 SECONDS");
    delay(30000);
    }
}
