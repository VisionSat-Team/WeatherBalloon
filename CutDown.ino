const int cutDownPin = 16; //****** Needs to Be chosen *********
boolean isCutDown = false; 

void setup() {
  pinMode(cutDownPin, OUTPUT); 
  digitalWrite(cutDownPin, LOW); 
}

void loop() {}

/* 
 *  Cut Down Function
 */
void cutDown() { 
  digitalWrite(cutDownPin, HIGH); 
  delay(10000); 
  isCutDown = true; 
  digitalWrite(cutDownPin, LOW); 
}
