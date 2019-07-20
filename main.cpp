#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include "PulseSensorPlayground.h"     // Includes the PulseSensorPlayground Library.

int m = analogRead(A4);     // myoware muscle sensor
int h = analogRead(A3);     // heart rate
int X = analogRead(A0);     // X-axis of Accelorometer
int Y = analogRead(A2);     // Y-axis of Accelorometer
int Z = analogRead(A1);     // Z-axis of Accelorometer



unsigned long previousMillis;       
unsigned long startSeizeMillis;
unsigned long seizeDuration;

bool stabilized = false;
bool seizing = false;
bool currentJerk = false; 
bool data = false;                //data collected post-seizure
bool tensionCalculated = false;   

int jerks = -1;
int prevBPM;
int bpmCounter = 0;
int bpmDiff; 
int tension;
int tensionAvg;
int tensionTotal = 0;
int minT = 645;           //for tension
int maxT = 0;             //for tension
int sample_count = 10;    //for tension
int manyJerks = 50;
int fewJerks = 10;
int highTension = 300;

int mled = 47;            // myoware
int hled = 35;            // heart rate
int aled = 40;            // accelorometer

const int mthresh = 200;  //muscle is tense
const int hthresh = 550;  // heartbeat
const int athresh = 150;  // movement detected

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

   

    int myBPM = pulseSensor.getBeatsPerMinute();// Calls function on our pulseSensor 

  
    if (pulseSensor.sawStartOfBeat() && !stabilized) {            // Constantly test to see if "a beathappened".
 
    if (myBPM >= 60 && myBPM <= 100){
        Serial.println("STABILIZED");
        prevBPM = myBPM;
        previousMillis = millis();
        stabilized = true;
        }
        
     Serial.println(myBPM);
 
}
  
//conditional runs when pulse sensor detects a stabilized heart beat  
if (stabilized){
  
    //tracking seizure 
    unsigned long currentMillis = millis();


  
    //calculating BMP diff every 7 seconds
    if ( currentMillis - previousMillis >= 7000  && currentMillis - previousMillis <= 7015 && !seizing) {
        bpmDiff = myBPM - prevBPM;
        previousMillis = currentMillis;
     
       //seize threshold = 50+bmp jump
          if (bpmDiff >= 50){
              jerks = 0;
              seizing = true;
              }
  
          prevBPM = myBPM;
          startSeizeMillis = currentMillis;
        }

      //calculating seizure duration
      if (seizing){
          data=false;
          Serial.println("SEIZEING");
  
          //seize jerk threshold: 650 
          if(analogRead(A0) >= 650 && !currentJerk) {
             currentJerk = true;
             jerks+=1; 
            }

           //recording jerk  
           if (currentJerk && analogRead(A0) < 650){
             currentJerk = false;
            }
    
    
          //muscle sensor tracks tension
          //collects sample_count number of tension values
          //discards max and min values and stores average
   
          if (!tensionCalculated){
              for (int i=0; i < sample_count; i++){
                  tension = analogRead(A4);
              if (tension > maxT){
                  maxT = tension;
               }
              if (tension < minT){
                  minT = tension;
               }
              tensionTotal += tension;
            }
           tensionTotal -= minT;
           tensionTotal -= maxT;
           tensionCalculated = true;
         }
  
        //stops seizing
        if (myBPM <= 110){
          seizeDuration = currentMillis - startSeizeMillis;
         
          Serial.print("STOPPED SEIZING");
          seizing = false;
         }
       }
    

      //printing duration, number of jerks, and average tension after a seizure had occured
      if (!seizing && jerks!= -1 && !data){
        Serial.print("DATA: ");
        Serial.print("SEIZE DURATION:");
        Serial.println(seizeDuration);
        Serial.print("JERKS: ");
        Serial.println(jerks);
        Serial.print("TENSION AVG: ");
        tensionAvg = int(tensionTotal / (sample_count-2));
        Serial.println(tensionAvg);
        data = true;     //prints once
        tensionCalculated = false;


         if (jerks < manyJerks && tensionAvg > highTension){                           //low jerks, high tension
              Serial.println("THIS SEIZURE WAS TONIC");
          }
         if (jerks > manyJerks && tensionAvg < highTension){                           //high jerks, low tension
              Serial.println("THIS SEIZURE WAS CLONIC");
          }
         if (jerks < manyJerks && jerks > fewJerks && tensionAvg < highTension){      //some jerks, low tension
              Serial.println("THIS SEIZURE WAS MYOCLONIC");
          }
         if (jerks < manyJerks && tensionAvg < highTension){                           //low jerks, low tension
              Serial.println("THIS SEIZURE WAS ATONIC");
          }
         if (jerks > manyJerks && tensionAvg > highTension){                           //high jerks, high tension
              Serial.println("THIS SEIZURE WAS TONIC-CLONIC");
          } 
      }
    }

 
  }

/*

WHILE HEART RATE IS HIGH:

Tonic seizure: stiff — high tension
Clonic seizure: repeating jerking motions 
Myoclonic seizure: inconsistent jerks and twitches 
Atonic seizure: loose, not tension, no jerks 
Tonic-clonic seizure: stiffness and repeated jerks

Partial: elevated heart rate but symptoms are inconsistent
 */
 
