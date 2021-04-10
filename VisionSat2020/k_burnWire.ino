/* 
 *  BurnWire 
 *  
 *  Burns wire connecting balloon to payload
 */
 
void burnWire() { 
  digitalWrite(burnWirePin, HIGH); 
  delay(10000); 
  isCutDown = true; 
  digitalWrite(burnWirePin, LOW); 
}
