#include <Arduino.h>
#include <LibRobus.h>

#define PAS_DE_JETON 10

//Programe pour détecter les jetons du joueur grace a des limite switch
//(on ne détecte pas les jetons que le robot va mettre seulement ceux que le joueur va mettre)

//Mettre les bon numéro de pin (tecniquement les bon)
const int pinSwitch0 = 25;
const int pinSwitch1 = 26;
const int pinSwitch2 = 27;
const int pinSwitch3 = 32;
const int pinSwitch4 = 33;
const int pinSwitch5 = 34; //Peut juste resevoir du input
const int pinSwitch6 = 35; //Peut juste resevoir du input

/// @brief Détermine si une switch a été activer
/// @param tableau Tableau contenant les pin des intérupteurs
/// @return Numéro de la switch actionner
int NumeroSwitchActionner(int tableau[7]) {
    for(int i = 0; i < 7; i++) {
        if(digitalRead(tableau[i]) == LOW) {
            return i;
        }
    }
    return PAS_DE_JETON;
}

/// @brief Attend qu'un intérupteur appuier soit relacher 
/// @param numPin Numéro de la pin de l'intérupteur qui a été appuier 
void SwitchRelacher(int numPin) {
    int etat = LOW;
    while(etat != HIGH) {
        etat = digitalRead(numPin);
    }
}

/// @brief Détecte s'il y a un jeton jusqu'a se qu'il y a un
/// @return La colonne que le jeton a été mis 
int DeterminerColonne() {
    int tableauPinSwitch[7] = {pinSwitch0, pinSwitch1, pinSwitch2, pinSwitch3, pinSwitch4, pinSwitch5, pinSwitch6}; //Tableau qui relie les switch à leur colonne corespondantes -> indice : numéro colonne (de 0 a 6)] et -> contenu : Numéro de pin de la switch de la colonne
    int numSwitchActionner = NumeroSwitchActionner(tableauPinSwitch);

    while(numSwitchActionner == PAS_DE_JETON) {
        numSwitchActionner = NumeroSwitchActionner(tableauPinSwitch);
    }

    SwitchRelacher(tableauPinSwitch[numSwitchActionner]);
    
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

void loop() {
    int  colonne = DeterminerColonne();
    Serial.print("Numéro de colonne (0 à 6): ");
    Serial.println(colonne); 
}