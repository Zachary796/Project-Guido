#include <Arduino.h>
#include <LibRobus.h>

/*Programe pour détecter les jetons du joueur grace a des limite switch
(on ne détecte pas les jetons que le robot va mettre seulement ceux que le joueur va mettre)*/

/*
limite switch en normally open donc LOW -> appuyer et HIGH -> noraml
*/

#define PAS_DE_JETON 10


const int pinSwitch0 = 40;
const int pinSwitch1 = 41;
const int pinSwitch2 = 42;
const int pinSwitch3 = 43;
const int pinSwitch4 = 44;
const int pinSwitch5 = 45;
const int pinSwitch6 = 46;

// ********** pas sur à 100% que lorsque switch est high -> appuyer et low -> pas appyer **************

/// @brief Détermine si une switch a été activer
/// @param tableau Tableau contenant les pin des intérupteurs
/// @return Numéro de la switch actionner
int NumeroSwitchActionner(int tableau[7]) {
    for(int i = 0; i < 7; i++) {
        if(digitalRead(tableau[i]) == LOW) {
            //Serial.print("Swicht actionner : ");
            return i;
        }
    }
    //Serial.print(PAS_DE_JETON);
    return PAS_DE_JETON;
}

/// @brief Attend qu'un intérupteur appuier soit relacher 
/// @param numPin Numéro de la pin de l'intérupteur qui a été appuier 
void SwitchRelacher(int numPin) {
    //Si appeler fonction lors intérupteur pas apuiller rien faire 
    //Fonctionne si vérifier pin LOW avant 
    
    int etat = LOW;
    while(etat != HIGH) {
        etat = digitalRead(numPin);
    }
    //Serial.print("  Relacher");
}

/// @brief Détecte s'il y a un jeton jusqu'a se qu'il y a un
/// @return La colonne que le jeton a été mis 
int DeterminerColonne() {
    int tableauPinSwitch[7] = {pinSwitch0, pinSwitch1, pinSwitch2, pinSwitch3, pinSwitch4, pinSwitch5, pinSwitch6}; //Tableau qui relie les switch à leur colonne corespondantes -> indice : numéro colonne (de 0 a 6)] et -> contenu : Numéro de pin de la switch de la colonne
    int numSwitchActionner = NumeroSwitchActionner(tableauPinSwitch);

    while(numSwitchActionner == PAS_DE_JETON) {
        numSwitchActionner = NumeroSwitchActionner(tableauPinSwitch);
    }
    //Serial.print("  Num switch : ");
    //Serial.print(numSwitchActionner);
    //ajouter condition pour être sur 
    SwitchRelacher(tableauPinSwitch[numSwitchActionner]);
    //Serial.println("    Plus actionner");
    
    return numSwitchActionner;
}

void setup() {
    pinMode(pinSwitch0, INPUT);
    pinMode(pinSwitch1, INPUT);
    pinMode(pinSwitch2, INPUT);
    pinMode(pinSwitch3, INPUT);
    pinMode(pinSwitch4, INPUT);
    pinMode(pinSwitch5, INPUT);
    pinMode(pinSwitch6, INPUT);
    Serial.begin(9600);
}

//int tableauPinSwitch[7] = {pinSwitch0};

//int tableauPinSwitch[7] = {pinSwitch0, pinSwitch1, pinSwitch2, pinSwitch3, pinSwitch4, pinSwitch5, pinSwitch6};
void loop() {
    
    //int numSwitchActionner = NumeroSwitchActionner(tableauPinSwitch);
    //Serial.println(numSwitchActionner);
    
    /* 
    if(digitalRead(pinSwitch0) == LOW) {
        SwitchRelacher(pinSwitch0);
    }
    */

    int numSwitchActionner = DeterminerColonne();
    Serial.println(numSwitchActionner);


   
}
/* 
    if(digitalRead(pinSwitch0) == HIGH) {
        Serial.println("ouvert");
    }

    if(digitalRead(pinSwitch0) == LOW) {
        Serial.println("fermer");
    }
*/




/*
Code pour utiliser limite switch sans libRobus :

1- (https://arduinogetstarted.com/tutorials/arduino-limit-switch)
#include <ezButton.h>

ezButton limitSwitch(7);  // create ezButton object that attach to pin 7;

void setup() {
  Serial.begin(9600);
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  limitSwitch.loop(); // MUST call the loop() function first

  if(limitSwitch.isPressed())
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");

  if(limitSwitch.isReleased())
    Serial.println("The limit switch: TOUCHED -> UNTOUCHED");

  int state = limitSwitch.getState();
  if(state == HIGH)
    Serial.println("The limit switch: UNTOUCHED");
  else
    Serial.println("The limit switch: TOUCHED");
}

2- (https://www.circuits-diy.com/limit-switch-using-arduino-uno/)
2.1 - Connection : switch besoin d'être connecter avec 2 fil -> digital pin et GND
2.2 - Setup() : Pin -> input
2.3 - Pour lire valeur : digitalRead()
2.4 - Utilisation du résulat du digitalRead() : "Check the state of the switch, if the switch is closed the state will be LOW and 
        if the switch is open the state will be HIGH"
    -> Si retoure HIGH -> ouvert donc appuyer
    -> Si retoure LOW -> fermer donc pas appuyer

Info sur fonction digitalRead() : https://reference.arduino.cc/reference/en/language/functions/digital-io/digitalread/
Valeur returns : HIGH or LOW 

*/