void captureGroundStation()
{
  
  if (port1.available() > 0) {
    Serial.println("Packet Recieved");
    tncMessage = port1.readStringUntil(59); // Reads until ASCII#59 (;)
    tncMessage = tncMessage.substring(25);// Starts reading after Header
    tncMessage.toLowerCase();
    Serial.println(tncMessage);
      
      if(tncMessage.equals("cutdown")) {
        //cutDown();
        keyUp("Cutdown Command Recieved");
     }
     else if(tncMessage.equals("beacon")){
         
          //Sends the most recent data. 
          // But it should be distinct 
        
        keyUp("Manual Beacon: " + sensorData);
        
     }
     else if(tncMessage.substring(0,5).equals("delay")){
         
        //    Parses the TNC_message after the first 5 characters.
        //    The next characters should be the time for the delay in MilliSeconds 
        //    To change the delay variable "beaconDelay"
        
        String changeDelay(tncMessage.substring(5));
     }
     else if(tncMessage.equals("capture")){
        //This our instant capture
        String CaptureData();
     }  
     else {
        keyUp("Command does not exist");
    }
  }
  

}
void keyUp (String messageToGround)
{  
//Key up and put ax25 header.
//port1 leads into the TNC which ultimately transmits over the radio
//first write the header (regular or forced) and then the message (see beacon() function)
//0xC0 is the end character in ax.25

  port1.write(header, 25);

  port1.print(messageToGround);//Data to be returned
  Serial.println(messageToGround);
  port1.write(0xC0);//c0.. End of frame

  delay(2000);
  delay(6);


}
