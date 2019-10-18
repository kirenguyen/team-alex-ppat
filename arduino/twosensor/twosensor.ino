#define trigPin 8
#define echoPin 9

#define trigPin2 7
#define echoPin2 6

#define alertPin 2

//const int MAX_DISTANCE = 8; //inches
const int MAX_DISTANCE = 20; //centimeters

//const int MIN_DISTANCE = 1; //inches   TODO: closer than this will still result in a standard alert without taking into account trajectory
const int MIN_DISTANCE = 5; //centimeters   TODO: closer than this will still result in a standard alert without taking into account trajectory


int inches, inches2, cm, cm2, oldDistance, oldDistance2;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(alertPin, OUTPUT);
  oldDistance = MAX_DISTANCE;
  oldDistance2 = MAX_DISTANCE;
}
 
void loop() {
  inches = getDistanceInches(trigPin, echoPin);
  inches2 = getDistanceInches(trigPin2, echoPin2);
  cm = getDistanceCentimeters(trigPin, echoPin);
  cm2 = getDistanceCentimeters(trigPin2, echoPin2);

//  Serial.print("inches 1: ");
//  Serial.println(inches); 
//  Serial.print("inches 2: ");
//  Serial.println(inches2);
  Serial.print("cm 1: ");
  Serial.println(cm); 
  Serial.print("cm 2: ");
  Serial.println(cm2);
  
  //checkForAlert(inches, inches2);
  checkForAlert(cm, cm2);

//  update old distances to most recent distance
  //oldDistance = inches;
  //oldDistance2 = inches2;
  oldDistance = cm;
  oldDistance2 = cm2;
  
  delay(250);
}

void checkForAlert(int distance, int distance2) {
  // Checks if any of the sensors are in the alert range and gives varying length alerts for alarm
  
  if ((distance < MAX_DISTANCE) && (distance < oldDistance)){
    tone(alertPin, 800, 500);
  }
  else if ((distance2 < MAX_DISTANCE) && (distance2 < oldDistance2)){
    tone(alertPin, 800, 500);
  } 
  else if ((distance < MIN_DISTANCE) || (distance2 < MIN_DISTANCE)){
    // very very close
    tone(alertPin, 1100, 1000);
  } else {
    noTone(alertPin);
  }
}


int getDistanceInches(int trig, int echo) {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Bounce time must be immediately taken after sensor trigger, otherwise errors occur
  
  int bounceTime = pulseIn(echo, HIGH);
  int distance = (bounceTime / 2) / 74;   // Divide by 74 or multiply by 0.0135 for inches
  return distance;
}

int getDistanceCentimeters(int trig, int echo) {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Bounce time must be immediately taken after sensor trigger, otherwise errors occur
  
  int bounceTime = pulseIn(echo, HIGH);
  int distance = (bounceTime / 2) / 29.1;   // Divide by 74 or multiply by 0.0135 for inches
  return distance;
}
