#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



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
const int analogRelaySwitchOutPin = A5;  // Analog output pin that the relay is attached to
int sensorValue = 0;        // last read value
unsigned int movingAverage = 0;    
int thresholdToTurnOn = 64;
int thresholdToTurnOff = 58;
boolean isLuefterOn = false;
unsigned int readInterval = 2500;

void switchIfRequired();

#define DHTPIN            11       // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // Sensor type DHT 11 
//DHT_Unified dht(DHTPIN, DHTTYPE, 15);
DHT_Unified dht(DHTPIN, DHTTYPE);
  
// the setup routine runs once when you press reset:

void setup() {
  Serial.begin(9600); 
  pinMode(analogRelaySwitchOutPin, OUTPUT); 
  
  // Initialize device.
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details. 
  //delayMS = 2000 //sensor.min_delay / 1000; 
  turnLuefterOff();
}

void loop() { 
  delay(readInterval);
  sensors_event_t event;  
  /*
  dht.temperature().getEvent(&event); //Read temperature value 
  if (isnan(event.temperature)) { // If temperature value is not a number 
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature : ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  } 
  */
  dht.humidity().getEvent(&event);  /* Read humidity value */
  if (isnan(event.relative_humidity)) { /* If humidity value is not a number */
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity : ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    handleNewValue(event.relative_humidity);
    switchIfRequired();
  }        
}

void switchIfRequired() {
    if (isLuefterOn && movingAverage <= thresholdToTurnOff) {
      turnLuefterOff();
    } else if (!isLuefterOn && movingAverage >= thresholdToTurnOn) {
      turnLuefterOn();
    } else {
       Serial.print("\n Nothing to switch");
      
    }
}

void turnLuefterOn() {
  digitalWrite(analogRelaySwitchOutPin, HIGH);
  Serial.print("\n\n#############\n!!! TURN ON !!!\n#############\n"); 
  isLuefterOn = true;
}

void turnLuefterOff() {
  digitalWrite(analogRelaySwitchOutPin, LOW);
  Serial.print("\n\n#############\n!!! TURN OFF !!!\n#############\n"); 
  isLuefterOn = false;
}

/*
boolean isStable() {
  return stableCheckCounter >= minStability;
}
*/

void handleNewValue (int newValue){
  sensorValue = newValue;

  unsigned int lastMovingAverage = movingAverage;  
  movingAverage = (lastMovingAverage + newValue) / 2;

  Serial.print("\nsensorValue : ");
  Serial.print(sensorValue);

    Serial.print("\tmovingAverage : ");
    Serial.print(movingAverage);
}

