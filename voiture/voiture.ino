/*
   Robot suiveur de ligne à base esp 8266 + bouclier L293D
   
   Pus d'informations:
   https://electroniqueamateur.blogspot.com/2020/04/robot-suiveur-de-ligne-version-arduino.html

*/

// définition des broches

#define capteur A0

#define PinA 0 // broche enable du L298N pour le premier moteur
#define PinB 2 //  broche enable du L298N pour le deuxième moteur
#define SpeedA 5 // Premier moteur
#define SpeedB 4 // Deuxième moteur


const int sensibilite = 60; // écart admissible par rapport à la valeur initiale

int initial; // état initial du capteur de droite
// int initialGauche; // état initial du capteur de gauche

int vitesse = 1000; // 0 à 1023 PWM qui contrôle la vitesse des moteurs

unsigned long tempoLampe = 0;        // temporisation de 2s 
const long intervalLampe = 500; 

void setup() {
  Serial.begin(115200);
  // réglage des sorties
  pinMode(SpeedA, OUTPUT);
  pinMode(PinA, OUTPUT);
  pinMode(SpeedB, OUTPUT);
  pinMode(PinB, OUTPUT);

// test moteur 
  digitalWrite(PinA, HIGH);digitalWrite(PinB, HIGH);
  analogWrite(SpeedA,vitesse);analogWrite(SpeedB,vitesse);
delay(500);

 // on se donne le temps de placer le robot à son point de départ
  analogWrite(SpeedA,0);
analogWrite(SpeedB,0);
  // lecture des valeurs initiales (on suppose que les capteurs sont de part et d'autre de la ligne)
  initial = analogRead(capteur);
  Serial.print("Valeur initiale ");
    Serial.println(initial); 
    initial = 520;
    delay(2000);
    vitesse = 700;// 700
}

void loop() {

  // temporisation de 2s pour moteur
    unsigned long currentMillis = millis();
   if(currentMillis - tempoLampe > intervalLampe) {
          Serial.println("on s'arrête");
    digitalWrite(SpeedA, 0);
    digitalWrite(SpeedB, 0);
        tempoLampe=currentMillis;
   }
  int valeur ;
  valeur = analogRead(capteur);

  if (abs(valeur - initial) < sensibilite)  {
          Serial.println(valeur);
   analogWrite(SpeedA, vitesse);
    analogWrite(SpeedB, vitesse);
  } else if ( valeur > initial ) {
     tempoLampe=currentMillis;
      Serial.print(valeur);Serial.println("on tourne à gauche");
    digitalWrite(SpeedA, 0);
    analogWrite(SpeedB, vitesse);
  }   else if (valeur < initial) {
          Serial.print(valeur);Serial.println("on tourne à droite");
         tempoLampe=currentMillis;
    analogWrite(SpeedA, vitesse);
    digitalWrite(SpeedB, 0);
  }

  delay(100);
}
