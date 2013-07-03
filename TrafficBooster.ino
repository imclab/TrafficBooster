/**
 * TrafficBooster / July 2013
 * http://github.com/01010101/TrafficBooster
 *
 * Original idea by Jerome Saint-Clair aka 01010101 
 * http://saint-clair.net
 * 
 * A project by Jerome Saint-Clair and Antoine Bonnet 
 * For the Graffiti Research Lab France
 * http://graffitiresearchlab.fr
 * 
 */

int potPin = A0;
int lightPin = A1;
int switchPin = 2;     // switch is connected to pin 2

int relayPin = 9;
int greenPin = 10;
int redPin = 11;
int orangePin = 12;

int lastButtonState = 0;
int lastLightVal = 0;

boolean lastLightState = false; // What mode is the light in?
boolean lightOn = false;
boolean testMode = true;

boolean debug = false;

boolean lightStateChanged = false;
int lastPotVal = 0;


void setup() {
  pinMode(switchPin, INPUT);    // Set the switch pin as input
  pinMode(potPin, INPUT);
  pinMode(lightPin, INPUT);

  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(orangePin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  Serial.begin(9600);           // Set up serial communication at 9600bps
  
}

void loop(){

  // Potentiometer
  int potVal = analogRead(potPin);
  if (abs(lastPotVal-potVal) > 5) {
    Serial.print("Potentiometer value changed to: "); 
    Serial.println(potVal);
  }
  lastPotVal = potVal;

  // Light
  int lightVal = analogRead(lightPin);

  // Switch button
  int val1 = digitalRead(switchPin);      // read input value and store it in val
  delay(10);                         // 10 milliseconds is a good amount of time
  int val2 = digitalRead(switchPin);     // read the input again to check for bounces

  if (val1 == val2) {
    if(lastButtonState == 1 && val1 == 0 ) { // make sure we got 2 consistant readings and that the button state changed!
      testMode = !testMode;
      if (testMode) {
       Serial.println("Switched to Test mode"); 
      }
      else {
        Serial.println("Switched to Horn mode"); 
      }
    }
    lastButtonState = val1;   
  }

  // Show state
  if(testMode) { 
    digitalWrite(orangePin, HIGH);
  }
  else {
    digitalWrite(orangePin, LOW);
  }


  // Check for light state change from ON to OFF
  if (lightOn) {
    if (lightVal > potVal) { // Changed values and greater than threshold
      lightStateChanged = true;
      lightOn = false; 
      lastLightVal = lightVal;
      // Debug
      if (debug && lightStateChanged) {
        Serial.print("State changed");
        Serial.print(lastLightVal);
        Serial.print('/');
        Serial.println(lightVal);
      }
      // Light red LED ON
      digitalWrite(redPin, HIGH);
      // Light green LED ON 
      digitalWrite(greenPin, LOW);
    }
  }

  else { // !lightOn // // Check for light state change from OFF to ON
    if (lightVal < potVal) {
      lightStateChanged = true;
      lightOn = true; 
      lastLightVal = lightVal;      
      // Debug
      if (debug && lightStateChanged) {
        Serial.print("State changed (ON)");
        Serial.print(lastLightVal);
        Serial.print('/');
        Serial.println(lightVal);
      }
      // Light red LED OFF 
      digitalWrite(redPin, LOW);
      // Light green LED ON 
      digitalWrite(greenPin, HIGH);
    }
  }


  // Now do whatever the lightMode indicates
  if (lightOn && lightStateChanged && !testMode) { // light on
    // Play
    if (debug) {
      Serial.println("Horn begin");
    }
    digitalWrite(relayPin, HIGH);
    delay(1500);
    digitalWrite(relayPin, LOW);
    if (debug) {
      Serial.println("Horn end");        
    }
    lightStateChanged = false;
  }
}

