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
#define DROITE 1 // tourne à droite
#define GAUCHE 2 // tourne à gauche
#define MILIEU 510 // pont de résistance 1024 / 2 environ

const int sensibilite = 50; // écart admissible par rapport à la valeur initiale

int initial ; // état initial du capteur

int dir= -1; // direction 0 : tout droit

int vitesse = 1000; // 0 à 1023 PWM qui contrôle la vitesse des moteurs

unsigned long tempoLampe = 0;        // temporisation de 2s 
unsigned long intervalLampe = 2000; 

void setup() {
  Serial.begin(115200);
  // réglage des sorties
  pinMode(SpeedA, OUTPUT);
  pinMode(PinA, OUTPUT);
  pinMode(SpeedB, OUTPUT);
  pinMode(PinB, OUTPUT);
  initial = analogRead(capteur);
  bip(); // test moteur 
 //   digitalWrite(PinA, LOW);digitalWrite(PinB, LOW);// marche arriere

         if (initial> 100){   // lecture des valeurs initiales (on suppose que les capteurs sont de part et d'autre de la ligne)
             bip();
         } else{ while(1){ // capteur pas branché
           initial = analogRead(capteur);
            Serial.print("capteur pas branché "); Serial.println(initial-MILIEU); 
            delay(500);
          }
         }
     if(abs( initial-MILIEU) < (sensibilite/2)){
       bip();
     } else{
      initial = MILIEU;
     }
    delay(2000);
    vitesse = 800;// 700
}
void bip(){ // test moteur 
               analogWrite(SpeedA,vitesse);analogWrite(SpeedB,vitesse);
                           delay(200);
                analogWrite(SpeedA,0);analogWrite(SpeedB,0); 
                 delay(400);          
}
void loop() {
  int valeur ;
  valeur = analogRead(capteur);
  // temporisation de 2s pour moteur
    unsigned long currentMillis = millis();
   if(currentMillis - tempoLampe > intervalLampe) {
          Serial.println("on s'arrête");
    digitalWrite(SpeedA, 0);
    digitalWrite(SpeedB, 0);
   }
  if (abs(valeur - initial) < sensibilite)  {
    if(dir!= 0){ 
      Serial.println(valeur);Serial.println("tout droit");
      dir=0; tempoLampe=currentMillis;intervalLampe = 2000;
         analogWrite(SpeedA, vitesse);
    analogWrite(SpeedB, vitesse);
    }
  } else if ( valeur > initial ) {
      if(dir!= GAUCHE){ 
      Serial.print(valeur);Serial.println("on tourne à gauche");
    digitalWrite(SpeedA, 0);
    analogWrite(SpeedB, vitesse);
      dir=GAUCHE; tempoLampe=currentMillis;intervalLampe = 4000;
    }
  }   else if (valeur < initial) {
    if(dir!= DROITE){ 
     Serial.print(valeur);Serial.println("on tourne à droite");
    analogWrite(SpeedA, vitesse);
    digitalWrite(SpeedB, 0);
      dir=DROITE; tempoLampe=currentMillis;intervalLampe = 4000;
    }

  }

  delay(100);
}
