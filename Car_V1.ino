#include <Servo.h> 
 
Servo frontWheels;  // Servo that controls the direction 

#define trigPin D4 // HC-SR04 Trig
#define echoPin D2 // HC-SR04 Echo
#define motor D6  // Pin attached to the Relay to allow 

int rotationAng = 20; //adjust the angles for your own car, this worked out for me.
int startingAng = 100;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motor, OUTPUT);
  
  frontWheels.attach(13); //13 is D7 (I have a Wemos D1 Mini)
  AdjustFrontWheels();

}

void loop() {

  long distance = ReadDistance();

  digitalWrite(motor, HIGH);
  
  if(distance < 30)
  {
    ToCloseDriveSlowly(distance);
  }
  if (distance < 50) 
  {
    TurnRight();
    delay(1000);
    AdjustFrontWheels();
  }
  else 
  {    
    delay(30);
  }

  delay(200);
}

void AdjustFrontWheels()
{
  frontWheels.write(startingAng);
}

void TurnRight()
{
    frontWheels.write(startingAng + rotationAng); 
}

void TurnLeft()
{
    frontWheels.write(startingAng - rotationAng); 
}

//Read the distance from the HC-SR04
// And returns it
int ReadDistance()
{
  long duration, distance;
  
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

   //for debug purposes
   //Serial.print(distance);
   //Serial.println(" cm");

  return distance;
}

//When the car is driving to slow it will try to deaccelerate
//It's kinda hard coded but it works out.
void ToCloseDriveSlowly(long distance)
{
  digitalWrite(motor, LOW);
  
  TurnRight();
  while(distance < 30)
  {
    delay(500);
    digitalWrite(motor, HIGH);
    delay(300);
    digitalWrite(motor, LOW);

    if(distance < 10 )
    {
      Stop(distance);
    }
    delay(100);
    distance = ReadDistance();
  }
}

//Stop the car while the distance is lower than 10.
// 10 might be to little if the car is gainning a lot of speed.
void Stop(long dist)
{
  while(dist <= 10)
  {
    delay(1000);
    dist = ReadDistance();
   
  }
}

