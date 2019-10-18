const int TRIG_PIN = 3;
const int ECHO_PIN = 2;
const int ALERT_PIN = A4;

// proximity range for variable vibration
const int MAX_DISTANCE = 30; //cm
const int MIN_DISTANCE = 5; //cm, closer than this will result in max vibration
const long VIBRATION_FACTOR = 126 / (MAX_DISTANCE - MIN_DISTANCE);

const long SOUND_SPEED = 29.1; // division factor to get cm

long duration, distance, old_distance, vibration;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ALERT_PIN, OUTPUT);
  Serial.begin(9600);
  old_distance = MAX_DISTANCE;
}

void loop() {
  // clears trig pin for clear signal (at least 2 microseconds)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  // set trigPin on High for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(TRIG_PIN, LOW);

  // divide by 2 since echo time is bounce to object and back from object
  duration = pulseIn(ECHO_PIN, HIGH);
  duration = duration / 2;

  // distance = time * speed
  distance = duration / SOUND_SPEED;  // cm

  Serial.print("Distance: ");
  Serial.println(distance);

  if ((distance > MIN_DISTANCE) && (distance < MAX_DISTANCE) && (distance < old_distance)) {
    vibration = 130 + floor((MAX_DISTANCE - distance) * VIBRATION_FACTOR);
    Serial.print("Vibration level: ");
    Serial.println(vibration);
    analogWrite(ALERT_PIN, vibration);
  } else if (distance < MIN_DISTANCE && distance < old_distance) {
    digitalWrite(ALERT_PIN, HIGH); //turn onto max
  } else {
    digitalWrite(ALERT_PIN, LOW); // turn off
  }
  old_distance = distance;

  delay(250);
}
