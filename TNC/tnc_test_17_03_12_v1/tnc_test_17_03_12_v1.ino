
#include <SoftwareSerial.h>
byte a = (0x1);

//SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  // Serial.print("HELLO  ");
  Serial.begin(19200);
  //  mySerial.begin(192000);
}

void loop()
{
//    Serial.println(Serial.read()); 

  Serial.println(Serial.readStringUntil(59));  

//  KEY_UP();
  // Serial.print(a);
  //Serial.write(192);
  //Serial.write(00);
  //Serial.write(192);
  delay(1000);

}
void KEY_UP ()
{
  //Serial.println("192");//=C0
  Serial.write(192);//c0
  Serial.write(0x00);//00
  // Serial.write(0x03);

  //delay(2000);
  //Serial.println(0);
  // Serial.write(a);
  // delay(20);
  //Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  /* Serial.write(0xae);
   Serial.write(0x6a);
   Serial.write(0xaa);
   Serial.write(0x98);
   Serial.write(0x40);
   Serial.write(0x40);
   Serial.write(0xe0);
   Serial.write(0xae);
   Serial.write(0x6a);
   Serial.write(0xaa);
   Serial.write(0x98);
   Serial.write(0x40);
   Serial.write(0x40);
   Serial.write(0x7f);
   Serial.write(0x03);
   Serial.write(0xf0);
   Serial.println("#Hello world --------------------------------------------*");// this is wwhere your data goes
   //delay(20);
   //Serial.println("192");
   Serial.write(0x10);
   Serial.write(0x13);
   Serial.write(0x03);
   Serial.write(0xf0);
   Serial.write(0xc0); //=C0
   */
  Serial.write(134);//c
  Serial.write(162);//q
  Serial.write(64);//space
  Serial.write(64);//space
  Serial.write(64);//space
  Serial.write(44);//space
  Serial.write(96);//space

  Serial.write(150);//k
  Serial.write(106);//5
  Serial.write(162);//q
  Serial.write(176);//x
  Serial.write(148);//j
  Serial.write(64);//space
  Serial.write(118);//?
  Serial.write(164);//
  Serial.write(166);//
  
  Serial.write(96);
  Serial.write(146);//92
  Serial.write(166);//a6
  Serial.write(166);//a6
  Serial.write(97);//a1
  Serial.print("  Hello World TEMPI23,TEMPO-50!");
  Serial.write(3);//03
  Serial.write(240);//f0
  Serial.write(13);//cr



  Serial.write(192);
  delay(20);
  //Serial.println("a");
  //Serial.print("a");
  Serial.println("  ");
  
  delay (6);
  return;

}
