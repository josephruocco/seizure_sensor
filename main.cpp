#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include "PulseSensorPlayground.h"     // Includes the PulseSensorPlayground Library.

//int t = analogRead(A5); // temperature
int m = analogRead(A4); // myoware
int h = analogRead(A3); // heart rate
int X = analogRead(A0); 
int Y = analogRead(A2);
int Z = analogRead(A1);

int jerks = 0;
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

int bpmCounter = 0;
boolean stabilized = false;

void loop() {

   

int myBPM = pulseSensor.getBeatsPerMinute();// Calls function on our pulseSensor 


	
if (pulseSensor.sawStartOfBeat() && !stabilized) {            // Constantly test to see if "a beathappened".
 //Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 //Serial.print("BPM: ");                        // Print phrase "BPM: "
if (myBPM >= 60 && myBPM <= 100){
	Serial.println("STABILIZED");
	stabilized = true;
}	
 Serial.println(myBPM);                        // Print the value inside of myBPM.
}

  delay(20);                    // considered best practice in a simple sketch.
 
	
//conditional runs when pulse sensor detects a stabilized heart beat	
if (stabilized){
	
	
 //muscle sensor tracks tension
		if(analogRead(A4) > mthresh)	
		{ digitalWrite(mled, HIGH);
		  Serial.println("TENSE");}
  	  else
		{ digitalWrite(mled, LOW);} //end muscle sensor
		
	//counts jerks in the x axis of the accelerometer	
	 if(jerks == 0) 
    	{jtime1 = millis();}

    jtime2 = millis();

  	if(analogRead(A0) > 650) {
    jerks++; }

  	if(jtime2>=jtime1+10000) {
    Serial.println("LAST 10 SEC: " + jerks + " jerks.");
	jerks = 0;
	}
  } // end jerk sensor	
		


	
}//end stabilized


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
