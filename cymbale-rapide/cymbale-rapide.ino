
// interface MIDI
// http://projectgus.github.io/hairless-midiserial/

const int analog_pins[] = {A0, A1, A2, A3, A4,A5};
const int threshold = 50 ;  // threshold value to decide when the detected sound is a knock or not
const int threshold_max = 100 ; // 
const int boton1 = 2; // hi-hats pedal
const int boton2 =  3; // extra sound
const int gnd =  4; // masse pour résistance 1Mo
const int gnd1 =  13; // masse pour résistance 470 Ko 
const int debug_pin =  12; // masse si debug 
#define NBDRUMS 4 // nombre de batteries - 1

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int buttonState1 = 0;      
int buttonState2 = 0;       // buttons used for the pedals or modifiers
int cooldown[6] = {0, 0, 0, 0, 0, 0};    // iterations until reading the piezo again (to avoid noise)
bool noteIsOn[6] = {false, false, false, false, false, false};
bool prev1 = 0;             
bool prev2 = 0;              // previous button state, used to detect flanks   
bool modif1 = false;
bool modif2 = false;
int vel;
int pico;
bool debug = false; // mode debug avec commentaires 

void setup() {

  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
    pinMode(debug_pin, INPUT_PULLUP);
     pinMode(gnd, OUTPUT);    pinMode(gnd1, OUTPUT);
    digitalWrite(gnd, LOW); // masse pour résistance 1Mo
    digitalWrite(gnd1, LOW);// masse pour résistance 470 Ko 
    if(digitalRead(debug_pin) == 0) debug= true;
      Serial.begin(115200);
 if(debug)      Serial.println("drums midi");
// Serial.println(digitalRead(debug_pin));
}

void loop() {
  for (int i = 0; i <= NBDRUMS; i++) {
    vel = analogRead(analog_pins[i]);     // this function determines the peak value of the sensor i
   
        if (vel > threshold_max) {

          if (cooldown[i] == 0) {
              noteIsOn[i] = false ; // 
              if (i == 4) {
              cooldown[i] = 90;                                           
          } else if ( i == 3) {                                           
              cooldown[i] = 60;                                               //button modifies the index, shifts it two half-tones upwards
          } else { cooldown[i] = 50; }
        } else {
            cooldown[i]--;
        }
    
           if(debug) Serial.println(vel);
           if( noteIsOn[i] == false) {
          notaON(i, 100);
           }
      } else if (vel < threshold) noteIsOn[i] = false;
}
 delay(1);
}

void notaON(int i, int vitesse) {  
 if(debug) {
  Serial.println("--------------------------------");
   Serial.println(i);
   Serial.println(vel);
 }
 else {
    byte buf[3];
    buf[0] = 153;
    buf[1] = i + 60;
    buf[2] =vitesse;
    Serial.write(buf,3);
 }
  noteIsOn[i] = true; 
  delay(100);
}
