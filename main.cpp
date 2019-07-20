#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include "PulseSensorPlayground.h"     // Includes the PulseSensorPlayground Library.

//int t = analogRead(A5); // temperature
int m = analogRead(A4); // myoware
int h = analogRead(A3); // heart rate
int X = analogRead(A0); 
int Y = analogRead(A2);
int Z = analogRead(A1);

int count = 0;
int jpm ;
unsigned long time1 = 0; // store the initial time
unsigned long time2; // store the current time



int mled = 47; // myoware
int hled = 35; // heart rate
int aled = 40; // accelorometer

const int mthresh = 200;  //muscle is tense
const int hthresh = 550;  // heartbeat
const int athresh = 150; // movement detected

PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(9600); 

  pulseSensor.analogInput(3);
  pulseSensor.setThreshold(hthresh);

  if (pulseSensor.begin()){
    Serial.print("working!");
    }
  

  pinMode(mled, OUTPUT);
  pinMode(hled, OUTPUT);
  pinMode(aled, OUTPUT);

}

void loop() {


  if(analogRead(A4) > mthresh)
    { digitalWrite(mled, HIGH);}
  else
    { digitalWrite(mled, LOW);} 


int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
  // "myBPM" hold this BPM value now.

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: "
 Serial.println(myBPM);                        // Print the value inside of myBPM.
}

  delay(20);                    // considered best practice in a simple sketch.



/*
  if(analogRead(A3) > hthresh)
    { digitalWrite(hled,HIGH);} 
  else 
    { digitalWrite(hled,LOW);}
*/

  if(count == 0) 
    {time1 = millis();}

  time2 = millis();

  if(analogRead(A0) > 650) {
    count++; }

  if(time2>=time1+10000) {
    jpm = count * 6;

    if(jpm >= 2000){
      digitalWrite(aled, HIGH);
      jpm=0;

    }
    else {digitalWrite(aled, LOW);}

    time2 = 0;
    count = 0;

  }

}

/*

WHILE HEART RATE IS HIGH:

Tonic seizure: stiff — high tension
Clonic seizure: repeating jerking motions 
Myoclonic seizure: inconsistent jerks and twitches 
Atonic seizure: loose, not tension
Tonic-clonic seizure: stiffness and repeated jerks

Partial: elevated heart rate but symptoms are inconsistent
 */


