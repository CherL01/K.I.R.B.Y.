#include <Wire.h>
#include <Servo.h>

// Define pins

// Define servo motor
Servo platformServo;

// Define DC motor connections
int baseMotorPin;     // int?
int baseMotorIn1;     // no int
int baseMotorIn2;     // no int
int baseEncA;         // need int
int baseEncB;         // no int

int cardMotorPin;     // int?
int cardMotorIn1;     // no int -- also idk if this even has an encoder
int cardMotorIn2;     // no int
int cardEncA;         // need int
int cardEncB;         // no int

// Define functions
int dealCards(int cardsPerHand, int numPlayers);
void dealOneCard(void);
int rotateBase(float rotDegrees);

void setup() {
  // put your setup code here, to run once:

  // Initialize Serial communication
  Serial.begin(9600);



  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}


int dealCards(int cardsPerHand, int numPlayers) {
  // Input: number of cards to deal, number of players
  // call rotateBase - calculate degrees needed to spin based on numPlayers
    // ex. 6 players: 360/6 = 60 deg --> spin 60 deg before dealing every card
  // call dealOneCard
    // stop when numPlayers all have cardsPerHand
}

void dealOneCard() {
  // spin the card motor to shoot one card out
}

int rotateBase(float rotDegrees) {
  // rotate base motor by rotDegrees (use encoder values to determine the degree spun)
}
