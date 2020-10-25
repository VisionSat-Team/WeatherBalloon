/*
   Camera Functions
*/


//If any of these boolean variables return false, will not allow camera to take picture and save to SD card.
bool canTakePicture()
{
  return (SdCardAvailable && cameraAvailable && SpiInterfaceAvailable && commandCameraToggle);
}

void resetCameraToggleCheck()
{
  String variableCheck = "";
  File cameraCheck;

  cameraCheck = SD.open("CAMERA.txt", FILE_READ);
  if (cameraCheck)
  {
    while (cameraCheck.available())
    {
      variableCheck += cameraCheck.read();
      Serial.println("variableCheck: " + variableCheck);
    }
    cameraCheck.close();
  }
  else
  {
    Serial.println("Error with cameraCheck");
  }

  if (variableCheck == "116" || variableCheck == "84")
  {
    commandCameraToggle = true;
  }
  else
  {
    commandCameraToggle = false;
  }
}


void setSDVariableTrue()
{
  File setCamera;
  if (SD.exists("CAMERA.txt"))
  {
    SD.remove("CAMERA.txt");
  }
  setCamera = SD.open("CAMERA.txt", FILE_WRITE);
  if (setCamera)
  {
    setCamera.print("t");
    setCamera.close();
  }
}


void setSDVariableFalse()
{
  File setCamera;
  if (SD.exists("CAMERA.txt"))
  {
    SD.remove("CAMERA.txt");
  }
  setCamera = SD.open("CAMERA.txt", FILE_WRITE);
  if (setCamera)
  {
    setCamera.print("f");
    setCamera.close();
  }
}


//Takes one picture.
void takePicture()
{
  char str[8];
  byte buf[256];
  static int i = 0;
  static int k = 0;
  uint8_t temp = 0, temp_last = 0;
  uint32_t length = 0;
  bool is_header = false;
  File outFile;
  //Flush the FIFO.
  myCAM.flush_fifo();
  //Clear the capture done flag.
  myCAM.clear_fifo_flag();
  //Start capture.
  myCAM.start_capture();
  Serial.println();
  Serial.println(F("start Capture"));
  while (!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));
  Serial.println(F("Capture Done."));
  length = myCAM.read_fifo_length();
  Serial.print(F("The fifo length is :"));
  Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //384K
  {
    Serial.println(F("Over size."));
    return ;
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
    return ;
  }
  //Construct a file name.
  k = k + 1;
  itoa(picturesTaken, str, 10);
  strcat(str, ".jpg");
  //Open the new file.
  outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);
  if (!outFile) {
    Serial.println(F("File open faild"));
    return;
  }
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Read JPEG data from FIFO.
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      buf[i++] = temp;  //save the last  0XD9
      //Write the remain bytes in the buffer.
      myCAM.CS_HIGH();
      outFile.write(buf, i);
      //Close the file
      outFile.close();
      Serial.println(F("Image save OK."));
      is_header = false;
      i = 0;
    }
    if (is_header == true)
    {
      //Write image data to buffer if not full.
      if (i < 256)
        buf[i++] = temp;
      else
      {
        //Write 256 bytes image data to file.
        myCAM.CS_HIGH();
        outFile.write(buf, 256);
        i = 0;
        buf[i++] = temp;
        myCAM.CS_LOW();
        myCAM.set_fifo_burst();
      }
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buf[i++] = temp_last;
      buf[i++] = temp;
    }
  }
}
