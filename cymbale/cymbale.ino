
// interface MIDI
// http://projectgus.github.io/hairless-midiserial/

const int analog_pins[] = {A0, A1, A2, A3, A4};
const int threshold = 10 ;  // 25 = threshold value to decide when the detected sound is a knock or not
const int boton1 = 2;
const int boton2 =  3;
#define NBDRUMS 0 // nnombre de batteries 

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int buttonState1 = 0;      
int buttonState2 = 0;       // buttons used for the pedals or modifiers
int cooldown[6] = {0, 0, 0, 0, 0, 0};    // iterations until reading the piezo again (to avoid noise)
bool prev1 = 0;             
bool prev2 = 0;              // previous button state, used to detect flanks   
bool modif1 = false;
bool modif2 = false;
int vel;
int pico;
bool debug = false; // test : commentaires sur prise USB
//bool debug = true; 

void setup() {
  Serial.begin(115200);
 if(debug)      Serial.println("drums midi");
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
}

void loop() {
  
   // Serial.println(analogRead(A0)); delay(100); 
   
  for (int i = 0; i <= NBDRUMS; i++) {
    vel = maximo(i) * 0.124;

    //BOTONES   //BUTTONS
    buttonState1 = digitalRead(boton1);
    buttonState2 = digitalRead(boton2);
    if (buttonState1 == 1 && prev1 == 0) {
      modif1 = true;
    }
    if (buttonState1 == 0 && prev1 == 1) {     
      modif1 = false;                     // this button would be the hi-hats pedal, modifying the sound (note) it makes when pressing it
    }
    if (buttonState2 == 1 && prev2 == 0) {     
      notaON(7, 100);                           // button used for extra sound
    }
    if (buttonState2 == 0 && prev2 == 1) {

    }


    //SENSORES
    if (cooldown[i] == 0) {
      if (vel > threshold && i == 4) {
        notaON(i, 100);
        cooldown[i] = 90;                                           
      }
      else if (vel > threshold && i == 3 && modif1 == true) {
        notaON(i + 2, 100);                                             
        cooldown[i] = 60;                                               //button modifies the index, shifts it two half-tones upwards
      }
      else if (vel > threshold) {
        notaON(i, 100);
        cooldown[i] = 50;
      }
    } else {
      cooldown[i]--;
    }
    prev1 = buttonState1;
    prev2 = buttonState2;
  }

}

void notaON(int i, int vitesse) {  
 if(debug) {
  
   Serial.println(vel);
 }
 else {
    Serial.write(153);                            // this function sends through serial the 3 bytes that compromises a MIDI message
  Serial.write(i + 60);
  Serial.write(vitesse);
 }
//  noteIsOn[i] = true; 
}

int maximo(int i) {                             
  int lectura = analogRead(analog_pins[i]);     // this function determines the peak value of the sensor i
  pico = 0;
  do {
    if (pico < lectura) {
      pico = lectura;
    };
    lectura = analogRead(analog_pins[i]);
  } while (sensorReading > pico );
  return (pico);
}
