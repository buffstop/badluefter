/*
  Swithes on/off a relay according to air humidity.
  
  Connections:
  1) VCC -> humidity resistor -> A4
  2) humidity resistor -> 160 Ohm resistor -> GND
  3) A3 -> relay switch detector 
  4) VCC -> relay VCC
  5) GND -> relay GND
*/

//see pins_energia.h for more LED definitions
const int analogHumidityResistorInPin = A4;  // Analog input pin that the huminity resistor is attached to
const int analogRelaySwitchInPin = A3;  // Analog output pin that the relay is attached to
int sensorValue = 0;        // last read value
unsigned int movingAverage = 0;   
unsigned int lastMovingAverage = 0;  
unsigned int stableCheckCounter = 0;  
unsigned int minStability = 15;  
int thresholdToTurnOn = 310;
int thresholdToTurnOff = 430;
boolean isLuefterOn = false;
unsigned int readInterval = 2500;
  
// the setup routine runs once when you press reset:
void setup() {   
  Serial.print("\nSETUP\n");    
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(analogRelaySwitchInPin, OUTPUT); 
  digitalWrite(analogRelaySwitchInPin, LOW);
  
}

void loop() {
  delay(readInterval);               // wait for a second
  handleNewValue(analogRead(analogHumidityResistorInPin)); 
  switchIfRequired();
  debugLog();
}

void switchIfRequired() {
  if (isStable()) {
    if (isLuefterOn && movingAverage < thresholdToTurnOff) {
      turnLuefterOff();
    } else if (!isLuefterOn && movingAverage >= thresholdToTurnOn) {
      turnLuefterOn();
    }
  }
}

void turnLuefterOn() {
  digitalWrite(analogRelaySwitchInPin, HIGH);
  Serial.print("\n\n#############\n!!! TURN ON !!!\n#############\n"); 
  isLuefterOn = true;
  stableCheckCounter = 0;
}

void turnLuefterOff() {
  digitalWrite(analogRelaySwitchInPin, LOW);
  Serial.print("\n\n#############\n!!! TURN OFF !!!\n#############\n"); 
  isLuefterOn = false;
  stableCheckCounter = 0;
}

boolean isStable() {
  return stableCheckCounter >= minStability;
}

void handleNewValue (int newValue){
  sensorValue = newValue;

  lastMovingAverage = movingAverage;
  movingAverage = (movingAverage + newValue) / 2;
  
  int threshold = 0;
  if (isLuefterOn) {
    threshold = thresholdToTurnOff;
  } else {
    threshold = thresholdToTurnOn;
  }
    
  if (lastMovingAverage >= threshold && movingAverage < threshold ) {
    stableCheckCounter = 0;
  } else if (lastMovingAverage < threshold && movingAverage >= threshold ) {
    stableCheckCounter = 0;
  } else {
    stableCheckCounter++;
  }
}

//void setStableState(int mvAvg, int avg

void debugLog () {
  // print the results to the serial monitor:
  Serial.print("\n############"); 
   
  Serial.print("\nsensorValue = ");                       
  Serial.print(sensorValue); 
 
  Serial.print("\nmovingAverage = ");                       
  Serial.print(movingAverage);  
  
  Serial.print("\nstableCheckCounter = ");                       
  Serial.print(stableCheckCounter); 
  
//  Serial.print("\nanalogRelaySwitchInPin = ");                       
//  Serial.print(analogRead(analogRelaySwitchInPin)); 
}
