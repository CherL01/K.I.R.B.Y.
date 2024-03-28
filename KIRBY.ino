#include <Wire.h>
#include <Servo.h>

// Define pins

// Define servo motor
Servo PlatformServo;

// Define DC motor connections
int baseMotorPin=10;     // int
int baseMotorIn1=11;     // no int
int baseMotorIn2=12;     // no int
int baseEncA=2;         // need int
int baseEncB=9;         // no int

int cardMotorPin;     // int?
int cardMotorIn1;     // no int -- also idk if this even has an encoder
int cardMotorIn2;     // no int
int cardEncA;         // need int
int cardEncB;         // no int

volatile long baseMotorCount = 0;
int baseMotorSpeed = 60;           // Speed of motor 0-255
float degEncRatio = 1.8;     // 360 deg / 200 enc -> 1.8 deg per enc val
int encValError = 6;

// Define functions
void InitMotors(void);
void InitInterrupts(void);
void DisableMotors(void);
int DealCards(int cardsPerHand, int numPlayers);
int cardsPerHand, numPlayers=4, cardSpeed;
void DealOneCard(void);
int RotateBase(float rotDegrees);
int MovePlatform(float rotDegrees);
int SendLCD(int cardsDealt, int numPlayers, char gameType);

void TurnCW(void);
void TurnCCW(void);

String gameType;

void setup() {
  // Initialize motors and interrupts
  InitMotors();
  InitInterrupts();

  // Initialize Serial communication
  Serial.begin(9600);
  Wire.begin();             // join i2c bus

  Serial.setTimeout(50);
  delay(100);
}

void loop() {
  // get input from Harry's website page
  // inputs: cardSpeed, numPlayers, gameType (poker, go fish, big 2, etc)

  // testing
  if (Serial.available()) {
    gameType = Serial.readString();

    if (gameType.charAt(0) == 'p') {
      // Poker: 2 cards per player, x players
      Serial.println("Dealing poker");
      DealCards(2, numPlayers);
    } else if (gameType.charAt(0) == 'u') {
      // Uno: 7 cards per player, x players
      DealCards(7, numPlayers);
    } else if (gameType.charAt(0) == 'b') {
      if (gameType.charAt(1) == 'i') {
        // Big two: 13 cards per player, 4 players
        DealCards(13, 4);
      } else if (gameType.charAt(1) == 'l') {
        // Blackjack: 2 cards per player, x players (including dealer)
        DealCards(2, numPlayers);
        // **to do** rotate to each player who wants a card hit, skip players that stand
      }
      
    } else if (gameType.charAt(0) == 'x'){
      gameType.remove(0,1);
      SendLCD(3,4,"p");
    } else if (gameType.charAt(0) == 'z'){
      gameType.remove(0,1);
      MovePlatform(gameType.toInt());
    } else {
      Serial.println("Invalid gamemode. Please try again...");
    }
    
  }
  Serial.println(gameType);
  delay(20);
  Serial.println(baseMotorCount);
}

void InitMotors(void){
  // DC motor setup
  pinMode(baseMotorPin, OUTPUT);
  pinMode(baseMotorIn1, OUTPUT);
  pinMode(baseMotorIn2, OUTPUT);
//  pinMode(cardMotorPin, OUTPUT);
//  pinMode(cardMotorIn1, OUTPUT);
//  pinMode(cardMotorIn2, OUTPUT);

  // Servo motor setup
//  platformServo.write(180);     // set default position?
  PlatformServo.attach(5);       // need pwm pins
}

void InitInterrupts(void){
  pinMode(baseEncA, INPUT);
  pinMode(baseEncB, INPUT);
  attachInterrupt(digitalPinToInterrupt(baseEncA), EncoderEvent, CHANGE);
}

void EncoderEvent() {
  if (digitalRead(baseEncA) == HIGH) {
    if (digitalRead(baseEncB) == LOW) {
      baseMotorCount--;
    } else {
      baseMotorCount++;
    }
  } else {
    if (digitalRead(baseEncB) == LOW) {
      baseMotorCount++;
    } else {
      baseMotorCount--;
    }
  }
}

int SendLCD(int cardsDealt, int numPlayers, char gameType, bool sameGame){
  // Update the LCD as cards are being dealt. If there is a new game, also update name of game 
  Wire.beginTransmission(4);          // transmit to device address #4
  
  if (sameGame) {
    Wire.write("Cards dealt: ");        // sends 13(?) bytes
    Wire.write(cardsDealt);             // sends 1 byte

  } else {
    Wire.write("Game: ");
    if (gameType.charAt(0) == 'p') {
      Wire.write("POKER");
    } else if (gameType.charAt(0) == 'u') {
      Wire.write("UNO");
    } else if (gameType.charAt(0) == 'b') {
      Wire.write("BIG TWO");
    } else if (gameType.charAt(0) == 'l') {
      Wire.write("BLACKJACK");
    }

    Wire.write("Players: ");
    Wire.write(numPlayers);
  }

  Wire.endTransmission();
  delay(500);
}

int DealCards(int cardsPerHand, int numPlayers) {
  // Input: number of cards to deal, number of players
  // call RotateBase - calculate degrees needed to spin based on numPlayers
    // ex. 6 players: 360/6 = 60 deg --> spin 60 deg before dealing every card
  // call DealOneCard
    // stop when numPlayers all have cardsPerHand
  for (int i=0; i<cardsPerHand; i++){
    for (int j=0; j<numPlayers; j++) {
      RotateBase(360/numPlayers);
      DealOneCard();
      delay(400);
//      Serial.print("card(s) dealt: ");
//      Serial.println(j);
    } 
  }
}

void DealOneCard(void) {
  // spin the card motor to shoot one card out, delay, then turn off.
//  digitalWrite(cardMotorIn1, HIGH);
//  digitalWrite(cardMotorIn2, LOW);
//  analogWrite(cardMotorPin, baseMotorSpeed);
  
}

int RotateBase(float rotDegrees) {
  // rotate base motor by rotDegrees (use encoder values to determine the degree spun)
  float encValChange = (rotDegrees/degEncRatio);
  int prevCount = baseMotorCount;
  Serial.println(prevCount);
  Serial.print("Rotating ");
  Serial.println(rotDegrees);
  Serial.print("Encoder value change: ");
  Serial.println(round(encValChange));
  if (rotDegrees > 0.0) {
    while(baseMotorCount<prevCount+round(encValChange)-encValError) {
      TurnCW();
    }
    DisableMotors();
  } else {
    while(baseMotorCount>prevCount+round(encValChange)-encValError) {
      TurnCCW();
    }
    DisableMotors();
  }
  delay(10);
  Serial.println(baseMotorCount);
}

void TurnCW(void){
  digitalWrite(baseMotorIn1, HIGH);
  digitalWrite(baseMotorIn2, LOW);
  analogWrite(baseMotorPin, baseMotorSpeed);
}

void TurnCCW(void){
  digitalWrite(baseMotorIn1, LOW);
  digitalWrite(baseMotorIn2, HIGH);
  analogWrite(baseMotorPin, baseMotorSpeed);
}

void DisableMotors(void){
  digitalWrite(baseMotorIn1, LOW);
  digitalWrite(baseMotorIn2, LOW);
}

int MovePlatform(float rotDegrees){
  for (int pos=0; pos<=180; pos+=1){
    PlatformServo.write(pos);
    delay(15);
    Serial.print("Servo pos: ");
    Serial.println(pos);
  }
  for (int pos=180; pos>=0; pos-=1){
    PlatformServo.write(pos);
    delay(15);
    Serial.print("Servo pos: ");
    Serial.println(pos);
  }
  delay(2000);
  return rotDegrees;
}
