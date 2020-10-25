/*
   Command Parsing
*/


//Checks for commands sent to ballon.
void commandCheck()
{
  //Reset variables.
  command = "";
  commandParsed = "";

  //Check serial buffer for commands.
  while (Serial1.available() > 0)
  {
    in = Serial1.read();
    command = command + in + " ";
  }


  //Run through loop and check for 4 then 2 which translates to the '*' character.
  //Then creates commandParsed string which is the remaining characters that can be checked.
  for (int i = 0; i < command.length(); i++)
  {
    if (command[i] == '4')
    {
      if (command[i + 1] == '2')
      {
        for (int j = 3; j < command.length() - i - 1; j++)
        {
          commandParsed += command[i + j];
        }
      }
    }
  }

  //Cross check commandParsed to see if there is a command to be done.

  //If the ground command says "*cut#", initiate cut down.
  if (commandParsed.equals("99 117 116 35 192"))
  {
    delay(500);
    KEY_UP(header, ack);
    delay(1000);

    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }

    cutDown();
    delay(1000);
  }

  //If the ground command says "*bea#", initiate a beacon.
  if (commandParsed.equals("98 101 97 35 192"))
  {
    delay(500);
    KEY_UP(header, beacon(titleForced));
    delay(1000);

    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }
  }

  //If the ground command says "*int1#", changes beacon interval.
  if (commandParsed.equals("105 110 116 49 35 192"))
  {
    delay(500);
    KEY_UP(header, ack);
    delay(1000);

    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }

    beaconInterval = 1;
    delay(1000);
  }

  //If the ground command says "*int2#", changes beacon interval.
  if (commandParsed.equals("105 110 116 50 35 192"))
  {
    delay(500);
    KEY_UP(header, ack);
    delay(1000);

    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }

    beaconInterval = 2;
    delay(1000);
  }

  //If the ground command says "*int3#", changes beacon interval.
  if (commandParsed.equals("105 110 116 51 35 192"))
  {
    delay(500);
    KEY_UP(header, ack);
    delay(1000);

    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }

    beaconInterval = 3;
    delay(1000);
  }

  //If the ground command says "*int4#", changes beacon interval.
  if (commandParsed.equals("105 110 116 52 35 192"))
  {
    delay(500);
    KEY_UP(header, ack);
    delay(1000);

    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }

    beaconInterval = 4;
    delay(1000);
  }

  //If the ground command says "*cam#", toggles camera variable off and discontinues picture taking.
  if (commandParsed.equals("99 97 109 35 192"))
  {
    delay(500);
    KEY_UP(header, ack);
    delay(1000);


    //clear buffer so that ground commands are sent to the balloon clearly.
    for (int i = 0; i < 250; i++)
    {
      Serial1.read();
    }

    if (commandCameraToggle)
    {
      commandCameraToggle = false;
      setSDVariableFalse();
    }
    else
    {
      commandCameraToggle = true;
      setSDVariableTrue();
    }
  }
}
