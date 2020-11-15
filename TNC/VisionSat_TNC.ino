// Visio
#include <SoftwareSerial.h>
const byte zeroHex = (0x00);
// Header: Callsign and Start Characters 
byte header[25] = {0xC0, zeroHex, 0xAE,0x6A,0xAA,0x98,0x40,0x40,0xF6,0xAE,0x6A,0xAA,0x98,0x40,0x40,0x60,0x86,0xA2,0x40,0x40,0x40,0x40,0x61,0x03,0xF0,};
char in;  // Testing
// Message Received from Ground Station 
String message; 
// Command to execute 
String command;

SoftwareSerial port1(10,11); // RX, TX


void setup() {
  // put your setup code here, to run once:
  // Serial.print("HELLO  ");
  Serial.begin(9600);// print to console
  port1.begin(19200);// print to tnc
  Serial.println("demo to write to tnc  by Nick");
}

void loop()
{
  
  if (port1.available() > 0) {
    Serial.println("Packet Recieved");
    message = port1.readStringUntil(59); // Reads until ASCII#59 (;)
    message = message.substring(25);// Starts reading after Header
    message.toLowerCase();
    Serial.println(message);

    
    // Parse command out of message
     

    // Switch Case 
    //switch(command)

    // Cut Wire

    // Instant Capture 

    // Beacon: Send most recent Sensor Data 

    // Change delay 
    
    
  }
  //port1.flush(); // Empty Serial Buffer 
  //KEY_UP();
  //delay(100);

}

void KEY_UP ()
{  
//Key up and put ax25 header.
//port1 leads into the TNC which ultimately transmits over the radio
//first write the header (regular or forced) and then the message (see beacon() function)
//0xC0 is the end character in ax.25

  port1.write(header, 25);

  port1.print("Hello" );//Data to be returned
  Serial.println("Hello");
  port1.write(0xC0);//c0.. End of frame

  delay(2000);
  delay(6);


}
