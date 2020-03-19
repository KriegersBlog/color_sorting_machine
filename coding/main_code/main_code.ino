/* Author: Niklas Kamm, Julian Krieger, Pascal Gläß
 * License: See github.com/KriegersBlog/color_sorting_machine
 *                                                                                      // LICHTSCHRANKE M2 wird gebraucht (schritte)
 * This code is optimized to work on a Arduino Uno R3 to manage
 * the whole color sorting process. For more information: See documentation
 */
// CONSTANT VARIABLES

const int motor[4] = {0b00000101, 0b00001001, 0b00001010, 0b00000110}; // Dreht Motor im Uhrzeigersinn

// colors
const int min_values[3] = {1,2,3}; // R,G,B  Minimalster Wert, um als Farbe erkannt zu werden

// positions
const int one_step = 341; // HIER WERT FÜR EINE POSITION EINGEBEN
const int position_start = -1;

const int color_steps[4][2]= { {1,3} , {2,2} , {3,1} , {4,0} };
// Anzahl Positionen, die ein Motor von seiner Position braucht: M1, M2  | ROT, GRUEN, BLAU, GOLD

// pins
const byte motor1_pins[4] = {7, 5, 8, 6}; // IN3, IN1, IN4, IN2
const byte motor2_pins[4] = {11, 9, 12, 10}; // L1, L2, L3, L4
const byte leds[3] = {4,3,2}; // Red, Green, Blue
const byte light_barrier1 = A1; // upper light barrier
const byte light_barrier2 = A0; // lower light barrier
const byte color_sensor = A2; // sensor for color recognition


//UNCONSTANT VARIABLES

int active_light_barrier;   // value: cached pin of the active light_barrier (dependent on active motor)
int position_color;  // value: cached steps for the determined color // only for M1
int color_values[3]; // value: cached input of A2
byte active_motorpins[4];


void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  for(int i = 2; i < 13; i++)
    pinMode(i, OUTPUT);

  motorcontrol("M2", position_start);
  Serial.begin(9600);
}

void loop() {
  motorcontrol("M1", one_step); // Von Start zu Gold
  motorcontrol("M2", position_start);
  delay(500); // Kugel reinfallen lassen
  motorcontrol("M2", one_step);
  position_color = color_recognition();
  Serial.print("Position: ");
  Serial.println(position_color);
  motorcontrol("M1", one_step * color_steps[position_color][1]);
  motorcontrol("M2", one_step * color_steps[position_color][0]);
  Serial.println("Bis hier geschafft, jetzt Unendlichkeit");
  motorcontrol("M1", position_start);
} 
/*
void loop(){
  motorcontrol("M2", position_start);
  Serial.println("Top");
} */

int color_recognition(){

  for(int i = 0; i<3; i++){
    digitalWrite(leds[i], HIGH);
    delay(500);
    color_values[i] = analogRead(color_sensor);
    delay(500);
    digitalWrite(leds[i], LOW);
  }

  // If color red gets recognized
  if(color_values[0] >= min_values[0] && color_values[1] < min_values[1] && color_values[2] < min_values[2]){
    return 0;
  }
  // If color green gets recognized
  else if(color_values[0] < min_values[0] && color_values[1] >= min_values[1] && color_values[2] < min_values[2]){
    return 1;
  }
  // If color blue gets recognized
  else if(color_values[0] < min_values[0] && color_values[1] < min_values[1] && color_values[2] >= min_values[2]){
    return 2;
  }
  // If all colors are recognized
  else if(color_values[0] >= min_values[0] && color_values[1] >= min_values[1] && color_values[2] >= min_values[2]){
    return 3;
  }
  // If no color gets recognized (maybe error)
  else return 1337;
}


void motorcontrol(String _motor, int _position){

  if(_motor == "M1"){
    for(int i = 0; i<4; i++)
      active_motorpins[i] = motor1_pins[i];
    active_light_barrier = light_barrier1;
  }
  else if(_motor == "M2"){
    for(int i = 0; i<4; i++)
      active_motorpins[i] = motor2_pins[i];
      active_light_barrier = light_barrier1;
  }

  if(_position == position_start)
    while(analogRead(active_light_barrier) >= 350){        // HIER WERT FINDEN, LICHTSCHRANKE
      move_motor();
    }
  else
  for(int i = 0; i < _position; i++)
    move_motor();

}

void move_motor(){
  for(int x = 0; x < 4; x++){
      digitalWrite(active_motorpins[0], motor[x] & 0b00000001);
      digitalWrite(active_motorpins[1], motor[x] & 0b00000010);
      digitalWrite(active_motorpins[2], motor[x] & 0b00000100);
      digitalWrite(active_motorpins[3], motor[x] & 0b00001000);
      delay(2);
      }
}
