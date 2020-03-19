/* Autor: Niklas Kamm, Julian Krieger, Pascal Gläß
 * Lizenz: GNU General Public License V3 | Siehe github.com/KriegersBlog/color_sorting_machine/LICENSE
 *                   
 * Info: DE VARIANTE
 *       Der Code ist für einen Arduino Uno R3 optimiert
 *       Das Programm steuert die Farbsortiermaschine  
 * Mehr Informationen: Siehe Dokumentation
 */

 /* Wichtige Information: 
 *  Es gibt 2 Methoden, um mit der Farberkennung umzugehen. Pascal hat sich hierbei für die mit festen Werten
 *  entschieden. Die Alternative ist, nur den höchsten Wert zu nehmen und dann davon auszugehen, es ist die Farbe.
 *  Wir haben beide Versionen getestet, beide lieferten die gleichen Ergebnisse.
 */
 
// Konstanten

const int motor[4] = {0b00000101, 0b00001001, 0b00001010, 0b00000110}; // Motoransteuerung

// Vergleichswerte für Farberkennung | Werte: R,G,B | Minimalster Wert, um als Farbe X erkannt zu werden
const int min_values[3] = {1,2,3};

// Positionen
const int one_step = 1; // Motorschritte um sich eine Position zu bewegen
const int position_start = -1; // Startposition
const int color_steps[4][2]= { {1,3} , {2,2} , {3,1} , {4,0} }; 
// Erste Ziffer M1, Zweite Ziffer M2 | Blöcke: Rot/Grün/Blau/Gold

// Pinbelegung
const byte motor1_pins[4] = {7, 5, 8, 6}; // IN3, IN1, IN4, IN2 | L1, L2, L3, L4
const byte motor2_pins[4] = {11, 9, 12, 10};
const byte leds[3] = {4,3,2}; // Rot, Grün, Blau
const byte light_barrier1 = A1; // Sensor: Obere Lichtschranke
const byte light_barrier2 = A0; // Sensor: Untere Lichtschranke
const byte color_sensor = A2; // Sensor: Farberkennung


//Variablen

int active_light_barrier;  // Wert: Pin der aktiven Lichtschranke
int position_color;  // Gespeicherte Schritt für die erkannte Farbe | M1
int color_values[3]; // Werte: Ergebnisse der Farberkennung
byte active_motorpins[4]; // Wert: Pins des aktiven Motors

//--------------------------ARDUINO METHODEN----------------------------------//

void setup() {

  // PINKONFIGURATION

  // Sensoren
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  // Ausgaben
  for(int i = 2; i < 13; i++)
    pinMode(i, OUTPUT);

  // Startposition herstellen
  motorcontrol("M2", position_start);
}

void loop() {
  motorcontrol("M2", one_step); // Von Start zu Gold
  motorcontrol("M1", position_start);
  delay(500); // Kugel reinfallen lassen
  motorcontrol("M1", one_step);
  position_color = color_recognition();
  motorcontrol("M2", one_step * color_steps[position_color][1]);
  motorcontrol("M1", one_step * color_steps[position_color][0]);
  motorcontrol("M2", position_start);
}

//---------------------------EIGENE METHODEN----------------------------------//
// FARBERKENNUNG | PASCAL GLÄß

int color_recognition(){

  for(int i = 0; i<3; i++){
    digitalWrite(leds[i], HIGH);
    delay(500);
    color_values[i] = analogRead(color_sensor);
    delay(500);
    digitalWrite(leds[i], LOW);
  }

  // Auf Farbe Rot testen
  if(color_values[0] >= min_values[0] || color_values[1] < min_values[1] || color_values[2] < min_values[2]){
    return 0;
  }
  // Auf Farbe Grün testen
  else if(color_values[0] < min_values[0] || color_values[1] >= min_values[1] || color_values[2] < min_values[2]){
    return 1;
  }
  // Auf Farbe Blau testen
  else if(color_values[0] < min_values[0] || color_values[1] < min_values[1] || color_values[2] >= min_values[2]){
    return 2;
  }
  // Auf Farbe Gold testen
  else if(color_values[0] >= min_values[0] || color_values[1] >= min_values[1] || color_values[2] >= min_values[2]){
    return 3;
  }
  // Falls keine Farbe erkannt wird | Fehler ausgeben | Farbe Gold wird gewählt (Aussortieren)
  else return 3;
}


void motorcontrol(String _motor, int _position){

  if(_motor == "M1"){
    for(int i = 0; i<4; i++)
      active_motorpins[i] = motor1_pins[i];
    active_light_barrier = light_barrier2;
  }
  else if(_motor == "M2"){
    for(int i = 0; i<4; i++)
      active_motorpins[i] = motor2_pins[i];
      active_light_barrier = light_barrier2;
  }

  if(_position == position_start)
    while(analogRead(active_light_barrier) <= 1){        // HIER WERT FINDEN, LICHTSCHRANKE
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
