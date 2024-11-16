#include <Arduino.h>
#include <LibRobus.h>
//La fonction dispatch récupère et envoie un jeton, il est uniquement nécessaire de changer ce qui va caller la fonction en tant que tel et le input qui va sortir le robot de sa phase reload.
//La fonction reload va mettre le réservoir en position reload jusqu'à se que le bumper (1) entre une valeur true. 
//L'angle pour pouvoir sortir un jeton est 170, l'angle recharger la portion distributeur du robot est 80 et l'angle pour recharger le réservoir est 15.

void reload(){
    while (ROBUS_IsBumper (1) == 0)
    {
    SERVO_SetAngle(SERVO_1, 15);
    }
    SERVO_SetAngle(SERVO_1, 80);
}

void dispatch(){
SERVO_SetAngle(SERVO_1, 80);
delay(2500);
SERVO_SetAngle(SERVO_1, 170);
}

void setup(){
    Serial.begin(9600);
    BoardInit();
    SERVO_Enable(SERVO_1);
}

void loop(){
    if(ROBUS_IsBumper(3) == 1){
        dispatch();
    }
    if(ROBUS_IsBumper(2) == 1){
        reload();
    }
}
