const byte led = 5; //Pin
const byte sensor = A5; //Pin

void setup() {
  pinMode(led, OUTPUT);
  pinMode(sensor, OUTPUT);
  Serial.begin(9600); //starting the serial monitor
  digitalWrite(led, HIGH); //led is on all the time

}

void loop() {
  Serial.println(analogRead(sensor)); //reads the sensor values
}
