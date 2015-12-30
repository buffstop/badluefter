/*
  Swithes on/off a relay according to air humidity.
  
  Connections:
  1) VCC -> humidity resistor -> A4
  2) humidity resistor -> 160 Ohm resistor -> GND
  3) TODO: relay A3 -> relay ?????
  4) VCC + GND -> relay VCC + GND
*/

//see pins_energia.h for more LED definitions
const int analogHumidityResistorInPin = A4;  // Analog input pin that the huminity resistor is attached to
const int analogRelaySwitchInPin = A3;  // Analog output pin that the relay is attached to
int sensorValue = 0;        // last read value
unsigned int movingAverage = 0;   
unsigned int lastMovingAverage = 0;  
unsigned int stableCheckCounter = 0;  
unsigned int minStability = 15;  
int threshold = 1;
boolean isLuefterOn = false;
  
// the setup routine runs once when you press reset:
void setup() {   
  Serial.print("\nSETUP\n");    
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(analogRelaySwitchInPin, OUTPUT); 
}

// the run loop
void loop() {
  digitalWrite(analogRelaySwitchInPin, HIGH);
  
  delay(1000);               // wait for a second
  digitalWrite(analogRelaySwitchInPin, LOW);
  // read the analog in value:
  handleNewValue(analogRead(analogHumidityResistorInPin)); 
//  analogRead(analogRelaySwitchInPin);
  switchIfRequired();
  debugLog();
}

void switchIfRequired() {
  if (isStable()) {
    if (isLuefterOn && movingAverage < threshold) {
      turnLuefterOff();
    } else if (!isLuefterOn && movingAverage >= threshold) {
      turnLuefterOn();
    }
  }
}

void turnLuefterOn() {
  //TODO:
  Serial.print("\n\n#############\n!!! TURN ON !!!\n#############\n"); 
  isLuefterOn = true;
  stableCheckCounter = true;
}

void turnLuefterOff() {
  //TODO:
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
