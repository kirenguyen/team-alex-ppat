int trigPin = 8;    // Trigger
int echoPin = 9;    // Echo
int trigPin2 = 7;    // Trigger
int echoPin2 = 6;    // Echo

int ledPin = A3;
long duration, cm, inches, inches2;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(8, OUTPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
    if (inches<12) {
    // turn LED on:
    analogWrite(ledPin, 255);//from 0 to 255
    tone(3,1000);
    digitalWrite(8, HIGH);
  } else {
    // turn LED off:
    analogWrite(ledPin, 0);//from 0 to 255
    digitalWrite(8, LOW);
    noTone(3);
  }
  
  //delay(250);


  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin2, INPUT);
  duration = pulseIn(echoPin2, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches2 = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches2);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
    if (inches2<12) {
    // turn LED on:
    analogWrite(ledPin, 255);//from 0 to 255
    tone(3,1000);
    digitalWrite(8, HIGH);
  } else {
    // turn LED off:
    analogWrite(ledPin, 0);//from 0 to 255
    digitalWrite(8, LOW);
    noTone(3);
  }
  
  delay(250);
}

