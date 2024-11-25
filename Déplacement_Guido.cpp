//Logique du suiveur de ligne
#include <Arduino.h>
#include<LibRobus.h>


/* Branchement des pins
capteur devant
Capteur U1(gauche) pin A10
Capteur U2 (centre) Pin A11
Capteur U3 (droit) Pin A12
capteur deriere
U1 gauche pin A13
U2 centre pin A14
U3 droit pin A15
*/

int capteurgauche = A10;
int capteurcentre = A11;
int capteurdroit = A12;
int Rcapteurgauche = A0;
int Rcapteurcentre = A1;
int Rcapteurdroit = A2;
int capteurnoir = A3;
float ligne = 650;


bool depart = false; 
// etat des capteur gauche, centre et droit
float etatgauche;
float etatcentre;
float etatdroit;
float etatnoir;
int positionligne; //0=gauche et 1=droite
int etat; //variable de switch du suiveur de ligne
int boucleavance; //variable pour savoir si la ligne est trouvee
int tourne = 0; // variable pour placer la ligne
int position_c = 0;

void setup ()
{
  BoardInit();
  Serial.begin (9600);
  BoardInit();
  pinMode (capteurgauche, INPUT);
  pinMode (capteurcentre, INPUT);
  pinMode (capteurdroit, INPUT);
  pinMode (Rcapteurgauche, INPUT);
  pinMode (Rcapteurcentre, INPUT);
  pinMode (Rcapteurdroit, INPUT);
  pinMode (capteurnoir, INPUT);
  SERVO_Enable(SERVO_1);
}
void beep(int count){
  for(int i=0;i<count;i++){
    AX_BuzzerON();
    delay(100);
    AX_BuzzerOFF();
    delay(100); 
  }
}
void reload(){
    while (ROBUS_IsBumper (1) == 0)
    {
    SERVO_SetAngle(SERVO_1, 15);
    }
    SERVO_SetAngle(SERVO_1, 170);
}

void dispatch(){
SERVO_SetAngle(SERVO_1, 170);
delay(500);
SERVO_SetAngle(SERVO_1, 80);
delay(1000);
}
void avance_ish(){
    MOTOR_SetSpeed(0,0.2);
    MOTOR_SetSpeed(1,0.2);   
}
void recule_ish (){
    MOTOR_SetSpeed (0, -0.2);
    MOTOR_SetSpeed (1, -0.2);
}
void arret(){
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);   
}

void correctiongauche() {
    MOTOR_SetSpeed(0,0.2);
    MOTOR_SetSpeed(1,0.25); 
 
}

void correctiondroit(){
    MOTOR_SetSpeed(0,0.25);
    MOTOR_SetSpeed(1,0.2);
}
void Rcorrectiongauche (){
    MOTOR_SetSpeed (0, -0.20);
    MOTOR_SetSpeed (1, -0.15);
}
void Rcorrectiondroit (){
    MOTOR_SetSpeed (0, -0.15);
    MOTOR_SetSpeed (1, -0.20);
}
void retourdroit(){
   MOTOR_SetSpeed(0,0.25);
   MOTOR_SetSpeed(1,0.15);
   etatcentre = analogRead(capteurcentre);
   if (etatcentre >= ligne){
    MOTOR_SetSpeed(0,0.25);
    MOTOR_SetSpeed(1,0.15);
    if (etatcentre < ligne){
   }
}
}
void retourgauche(){
    MOTOR_SetSpeed(0,0.15);
    MOTOR_SetSpeed(1,0.25);
}


int DetecteurColonne(){
    etatnoir = analogRead(capteurnoir);
    if (etatnoir < ligne){
        return 2; //blanc
    }
   return 1; // noir
}


void chercheligne(){
    etatgauche = analogRead(capteurgauche);
    etatcentre = analogRead(capteurcentre);
    etatdroit = analogRead(capteurdroit);
    if (positionligne == 0){ //cherche a gauche
    retourgauche();
    }
    if (positionligne == 1){ //cherche a droite
    retourdroit();
    }
}
void placelignedroit(){
    avance_ish();
    delay (120);
    while (tourne == 1){
     etatgauche = analogRead(capteurgauche);
     etatcentre = analogRead(capteurcentre);
     etatdroit = analogRead(capteurdroit);
     MOTOR_SetSpeed(0,0.3);
     MOTOR_SetSpeed(1,-0.15);
     delay (50);
    if (etatcentre >= ligne && etatdroit < ligne){
       //delay (25);
       arret();
       tourne = 0;
}}
}
void placelignegauche(){
    avance_ish();
    delay (120);
    while (tourne == 1){
     etatgauche = analogRead(capteurgauche);
     etatcentre = analogRead(capteurcentre);
     etatdroit = analogRead(capteurdroit);
     MOTOR_SetSpeed(0,-0.15);
     MOTOR_SetSpeed(1,0.30);
     delay (50);
    if (etatcentre >= ligne && etatdroit < ligne){
       //delay (25);
       arret();
       tourne = 0;
}}
}
void suiveurligne(){
   
    etatgauche = analogRead(capteurgauche);
    etatcentre = analogRead(capteurcentre);
    etatdroit = analogRead(capteurdroit);
    Serial.print(etatgauche) ;
    Serial.print("\t");
    Serial.print(etatcentre);
    Serial.print("\t");
    Serial.print(etatdroit);
    Serial.print("\n");
    if (etatcentre >= ligne) { //ligne centre donc tourne un peu a droite
        etat = 1;
    }
    if (etatcentre < ligne) { //ligne a gauche donc tourne un peu a gauche
        etat = 2;
    }
    
    if (etatdroit < ligne && etatcentre < ligne  && etatgauche < ligne ){ //la ligne est perdue 
       etat = 3 ;
    } 
    if (etatdroit >= ligne){
       etat = 4;
    }
     if (etatgauche >= ligne){
       etat = 5;
    }
    if (etatdroit >= ligne && etatcentre >= ligne){ // la ligne tourne a droite
       etat = 6;
    }
    if (etatgauche >= ligne && etatcentre >= ligne){ // la ligne tourne a gauche
       etat = 7;
       
    }
     
    
        
    switch (etat)
    {
    case 1:
        correctiondroit();
        positionligne = 0;
        break;
    case 2:
        correctiongauche();
        positionligne = 1;
        break;
    case 3:
        chercheligne();
        break;
    case 4:
        retourdroit();
        positionligne = 0;
        break;
    case 5:
        retourgauche();
        positionligne = 1;
        break;
    case 6 :
        tourne =1;
        placelignedroit();
        break;
    case 7 :
        tourne =1;
        placelignegauche();
        break;
    
    default:
        MOTOR_SetSpeed(0,0);
        MOTOR_SetSpeed(1,0);
        break;
    
    }
}
void trouvegauche(){
    while (etatdroit < ligne || etatcentre < ligne){
        etatgauche = analogRead(capteurgauche);
        etatcentre = analogRead(capteurcentre);
        etatdroit = analogRead(capteurdroit);
        MOTOR_SetSpeed(0,-0.25);
        MOTOR_SetSpeed(1,0.2);
    }
    arret();
}
void trouvedroit(){
    while ( etatgauche < ligne || etatcentre < ligne){
        etatgauche = analogRead(capteurgauche);
        etatcentre = analogRead(capteurcentre);
        etatdroit = analogRead(capteurdroit);
        MOTOR_SetSpeed(0,0.2);
        MOTOR_SetSpeed(1,-0.25);
    }
    arret();
}

void trouveligne(){
    etatgauche = analogRead(capteurgauche);
    etatcentre = analogRead(capteurcentre);
    etatdroit = analogRead(capteurdroit);
    while (etatgauche < ligne && etatcentre < ligne && etatdroit < ligne){
        avance_ish();
        boucleavance = 1;
    etatgauche = analogRead(capteurgauche);
    etatcentre = analogRead(capteurcentre);
    etatdroit = analogRead(capteurdroit);
    }
    if (boucleavance == 1){
        arret();
        delay (50);
        while (etatgauche < ligne || etatdroit < ligne){
            etatgauche = analogRead(capteurgauche);
            etatcentre = analogRead(capteurcentre);
            etatdroit = analogRead(capteurdroit);
            if (etatgauche >= ligne && etatdroit < ligne){
                trouvegauche();
                delay (50);
            }
            if (etatgauche < ligne && etatdroit >= ligne){
                trouvedroit();
                delay (50);
            }
            
        }
        if (etatgauche >= ligne && etatdroit >= ligne){
                boucleavance = 0;
                tourne = 1;
                placelignedroit();
                delay (100);
            }
    }
}
void avancemillieu (){
    etatgauche = analogRead(capteurgauche);
    etatcentre = analogRead(capteurcentre);
    etatdroit = analogRead(capteurdroit);
    while (etatgauche < ligne || etatcentre < ligne || etatdroit < ligne){
        etatgauche = analogRead(capteurgauche);
        etatcentre = analogRead(capteurcentre);
        etatdroit = analogRead(capteurdroit);
        suiveurligne();
    }
     arret();
     
}
void suiveurlignereculon (){
    etatgauche = analogRead(Rcapteurgauche);
    etatcentre = analogRead(Rcapteurcentre);
    etatdroit = analogRead(Rcapteurdroit);
    if (etatcentre >= ligne) { //ligne centre donc tourne un peu a gauche
        etat = 1;
    }
    if (etatcentre < ligne) { //ligne a gauche donc tourne un peu a droite
        etat = 2;
    }
    
    
    switch (etat)
    {
    case 1:
        Rcorrectiongauche();
        break;
    case 2:
        Rcorrectiondroit();
        break;
    
    default:
        MOTOR_SetSpeed(0,0);
        MOTOR_SetSpeed(1,0);
        break;
    
}  
}
void avancecolonne (int colonne){
    int couleur = DetecteurColonne();
    while (position_c != colonne){
        if (colonne > position_c){
            couleur = DetecteurColonne();
            while ((couleur == 1) && (position_c < colonne)){
                couleur = DetecteurColonne();
                suiveurligne();        
            }
        delay(100);
        couleur = DetecteurColonne();
        while (couleur == 2){
            couleur = DetecteurColonne();
            suiveurligne();
        }
        delay(50);
        position_c++;
    }
    
    if (colonne < position_c){
        couleur = DetecteurColonne();
        while ((couleur == 1) && (position_c > colonne)){
            couleur = DetecteurColonne();
            suiveurlignereculon();
        }
        delay(100); 
        couleur = DetecteurColonne();
        while (couleur == 2){
            couleur = DetecteurColonne();
            suiveurlignereculon();
        }
        position_c --;
        delay(50);
              
        }
    }
    if (colonne == position_c){
        arret();
        beep (position_c);
    }
    
}



void loop (){
    etatgauche = analogRead(capteurgauche);
    etatcentre = analogRead(capteurcentre);
    etatdroit = analogRead(capteurdroit);
    int couleur = DetecteurColonne();
    

    Serial.print(etatgauche) ;
    Serial.print("\t");
    Serial.print(etatcentre);
    Serial.print("\t");
    Serial.print(etatdroit);
    Serial.print("\t");
    Serial.print(couleur);
    Serial.print("\t");
    Serial.print(position_c);
    Serial.print("\n");
    //delay(1000);
    
    if(ROBUS_IsBumper (3) == true) {
      depart=true;

    }
    if(ROBUS_IsBumper(2) == 1){
        reload();
    }
    if (depart == true) {
        avancecolonne(1);
   

   
   
    depart = false;
     }
     //delay(100);
}
