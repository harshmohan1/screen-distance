#include <SD.h>
#include <SPI.h>

int chipSelect = 4;
File mySensorData;

int trigPin = 8;                                                    //ultrasonic-sensor trigger pin is connected to IO 8
int ledPin = 3; 
int echoPin = 9;                                                    //ultrasonic-sensor echo pin is connected to IO 9
float roundTrip;                                                    //round-trip-time (RTT)
float distance2target;
float speedOfSound = 343;                                           //speed of sound at 20°C in m/s 


void setup() {

  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(10,OUTPUT);                                               //reserve IO 10 as an output - this is required by the SD.h library
  SD.begin(chipSelect);                                             //initialize SD-card

}

void loop() {
  
  //the ultrasonic-sensor starts its measurements upon arrival of a trigger pulse
  //its response to the microcontroller is a pulse whose length corresponds to the RTT
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin,LOW);
  delayMicroseconds(15);
  
  roundTrip = pulseIn(echoPin,HIGH);                                //measure RTT in microseconds
  roundTrip = roundTrip/1000000;                                    //RTT in seconds
  distance2target = speedOfSound * roundTrip/2;                     //distance to target in m 
  distance2target = distance2target * 100;                          //distance to target in cm
  if (distance2target < 40.00) {  // Check if distance is below threshold
   digitalWrite(ledPin, HIGH);  // Turn on LED
  } else {
    digitalWrite(ledPin, LOW);  // Turn off LED
    }


  mySensorData = SD.open("HCSR_final.txt", FILE_WRITE);
  if(mySensorData) {
    Serial.print("Distance to target: ");
    Serial.print(distance2target);
    Serial.println(" cm");

    delay(1000);

    mySensorData.print(distance2target);
    mySensorData.println(",");
    mySensorData.close();
  }
  
  }
