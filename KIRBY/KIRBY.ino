#include <Wire.h>
#include <Servo.h>
#include "index.h"
#include "ArduinoGraphics.h"
#include <WiFiS3.h>
#include "pitches.h"
#include <ezBuzzer.h>

//---WIFI PARAMETERS---
char ssid[] = "KIRBY"; //Enter your WIFI SSID
char pass[] = "123456789!";   //Enter your WIFI password
int keyIndex = 0;      // your network key index number (needed only for WEP)
String header; // read index from html page
String page = DEFAULT_PAGE; //set html page to default

int status = WL_IDLE_STATUS; //Wifi Status
WiFiServer server(80); //initialize wifi server

//---TIMEOUT VARIABLES---
unsigned long currentTime = millis(); //current time
unsigned long previousTime = 0; //previous time

const long timeoutTime = 2000; //define timeout time


// Define pins

// Define servo motor
Servo PlatformServo;

// Define DC motor connections
int baseMotorPin=10;     // pwm
int baseMotorIn1=11;     // no pwm
int baseMotorIn2=12;     // no pwm
int baseEncA=2;         // need interrupt pin
int baseEncB=9;         // no pwm

int cardMotorPin=6;     // pwm
int cardMotorIn1=7;     // no pwm -------------- direction pin (needed)
int cardMotorIn2=8;     // no pwm -------------- direction pin (needed)
int cardEncA=4;         // need interrupt pin -- enc values not really needed - needed if turning motor on and off too inconsistent
int cardEncB=5;         // no pwm -------------- enc values not really needed

volatile long baseMotorCount = 0;
int baseMotorSpeed = 60;           // Speed of motor 0-255
int cardMotorSpeed = 80;           // Speed of motor 0-255
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
int SendLCD(int cardsDealt, int numPlayers, String gameType, bool sameGame);

void checkGame(String gameType);

int eStopLCD();

void TurnCW(void);
void TurnCCW(void);

void webServer();
void printWifiStatus();

String gameType = "None";

void setup() {
  // Initialize motors and interrupts
  InitMotors();
  InitInterrupts();

    // Initialize Serial communication
  Serial.begin(9600);
  Wire.begin();             // join i2c bus

  //---SETTING UP WIFI---//
  
  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  // Check for latest Wifi firmware version
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  printWifiStatus();
  

  Serial.setTimeout(50);
  delay(100);
}

void loop() {
  // get input from Harry's website page - need to figure this out
  // inputs: cardSpeed, numPlayers, gameType (poker, go fish, big 2, etc)
  webServer();
  // testing
  /*
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
      // Big two or Blackjack
      if (gameType.charAt(1) == 'i') {
        // Big two: 13 cards per player, 4 players
        DealCards(13, 4);
        
      } else if (gameType.charAt(1) == 'l') {
        // Blackjack: 2 cards per player, x players (including dealer)
        DealCards(2, numPlayers);
        // **to do** rotate to each player who wants a card hit, skip players that stand
      }

    // test prompts
    } else if (gameType.charAt(0) == 'x'){
      gameType.remove(0,1);
      SendLCD(gameType.toInt(),4,"b",false);
    } else if (gameType.charAt(0) == 'y'){
      gameType.remove(0,1);
      SendLCD(gameType.toInt(),4,"b",true);
    } else if (gameType.charAt(0) == 'z'){
      gameType.remove(0,1);
      MovePlatform(gameType.toInt());
    } else {
      Serial.println("Invalid gamemode. Please try again...");
    }
    
  }*/
  delay(20);
  //Serial.println(gameType);
  // Serial.println(baseMotorCount);
}

void InitMotors(void){
  // DC motor setup
  pinMode(baseMotorPin, OUTPUT);
  pinMode(baseMotorIn1, OUTPUT);
  pinMode(baseMotorIn2, OUTPUT);
  pinMode(cardMotorPin, OUTPUT);
  pinMode(cardMotorIn1, OUTPUT);
  pinMode(cardMotorIn2, OUTPUT);

  // Servo motor setup
  PlatformServo.write(180);     // set default position?
  PlatformServo.attach(3);       // need pwm pin
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

int SendLCD(int cardsDealt, int numPlayers, String gameType, bool sameGame){
  // Update the LCD as cards are being dealt. If there is a new game, also update name of game 
  // Input: cardsDealt, numPlayers, gameType, sameGame
  // Output: LCD gets text that is sent between beginTransmission to endTransmission

  Wire.beginTransmission(4);          // transmit to device address #4
  
  if (sameGame) {
    Wire.write("Cards dealt: ");        // sends 13(?) bytes
    Wire.write(byte(cardsDealt));             // sends 1 byte

  } else {
    Wire.write("Game: ");
    if (gameType.indexOf("poker") == 0) {
      Wire.write("POKER");
    } else if (gameType.indexOf("uno") == 0) {
      Wire.write("UNO");
    } else if (gameType.indexOf("blackJack") == 0) {
      Wire.write("BLACKJACK");
    } else if (gameType.indexOf("bigtwo") == 0) {
      Wire.write("BIG 2");
    }
    Wire.endTransmission();
    
    Wire.beginTransmission(4);
    Wire.write("Players: ");
    Wire.write(byte(numPlayers));
    Wire.endTransmission();
    
    Wire.beginTransmission(4);
    Wire.write("Cards dealt: ");
    Wire.write(byte(cardsDealt));
  }

  Wire.endTransmission();
  delay(500);
}

int eStopLCD()
{
  Wire.beginTransmission(4);
  Wire.write("Stopped Dealing");

  Wire.endTransmission();
  delay(500);
}

int DealCards(int cardsPerHand, int numPlayers) {
  // Input: number of cards to deal, number of players
  // Iterate for each card needed to be dealt
  for (int i=0; i<cardsPerHand; i++){
    //SendLCD(i,numPlayers,gameType,true);

    // Iterate for each player
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
  digitalWrite(cardMotorIn1, HIGH);
  digitalWrite(cardMotorIn2, LOW);
  analogWrite(cardMotorPin, baseMotorSpeed);
  delay(50);        // Changeable value - time the motor turns to deal 1 card. Should be relatively short.
  DisableMotors();
}

int RotateBase(float rotDegrees) {
  // rotate base motor by rotDegrees (use encoder values to determine the degree spun)
  float encValChange = (rotDegrees/degEncRatio);
  int prevCount = baseMotorCount;

  // test
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
  digitalWrite(cardMotorIn1, LOW);
  digitalWrite(cardMotorIn2, LOW);
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

void checkGame(String gameType) //games: uno, blackJack, crazy8, poker
{
  if (gameType.indexOf("uno") == 0)
  {
    Serial.println("Dealing uno");
  }
}

//---Wifi Functions---//

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Now open this URL on your browser --> http://");
  Serial.print(ip);
  Serial.print("/default");
}

//---WIFI/HTML FUNCTIONS---
void webServer() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,

    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        Serial.println("NEW ACTION PROMPTED!");          // print a message out in the serial port
        header = client.readStringUntil('\n');          // read a byte & add to header
        Serial.println(header);
        break;
      }
    }

    if (header.indexOf("GET /on") >=0) // running state, display settings with emergency button
    {
      Serial.println("Running!");
      page = String(ON_PAGE);
      
      Serial.println(cardSpeed);
      Serial.println(numPlayers);
      page.replace("CARD_SPEED", String(cardSpeed));
      page.replace("NUM_OF_PLAYERS", String(numPlayers));
      page.replace("GAME_TYPE", gameType);

      //----------------------Deal cards HERE
    }
    
    else if (header.indexOf("GET /default") >=0)
    {
      Serial.println("Default Page!"); // starting page to set up speed, etc
      page = String(DEFAULT_PAGE);

      cardSpeed = 5; //default cardspeed and numplayers
      numPlayers = 2;
      gameType = "UNO";
      //----------------------Starting Page HERE
      
    }
    else if (header.indexOf("GET /speed") >= 0) //access index of header to get the speed/#of players/game type values.
    {
      //initiate variables to access speed, #of players, and game type
      int speedStart = header.indexOf("/speed");
      int playerStart = header.indexOf("/players");
      int gameStart = header.indexOf("/game");
      int httpStart = header.indexOf(" HTTP/");

      //iterate thru header to get card dealing speed
      Serial.print("This is speed(6): ");
      if (isDigit(header.charAt(speedStart+6))) 
      {
        String temp = "";
        for (int i=speedStart+6; i<playerStart; i++)
        {
          temp += String(header.charAt(i));
        }
        cardSpeed = temp.toInt();
      }
      Serial.println(cardSpeed);

      
      //iterate thru header to get # of players
      Serial.print("This is number of players(3): ");
      if (isDigit(header.charAt(playerStart+8))) 
      {
        String temp = "";
        for (int i=playerStart+8; i<gameStart; i++)
        {
          temp += String(header.charAt(i));
        }
        numPlayers = temp.toInt();
      }
      
      Serial.println(numPlayers);

      //iterate thru header to get game type
      Serial.print("This is game type: ");
      if (header.charAt(gameStart+5)) 
      {
        String temp = "";
        for (int i=gameStart+5; i<httpStart; i++)
        {
          temp += String(header.charAt(i));
        }
        gameType = temp;
      }
      
      Serial.println(gameType);
      
      page = String(START_PAGE);
      //----------------------Settting Page HERE
    }

    
    while (client.connected()) {
      if (client.available()) {
        header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (header.equals("\r"))  // the end of HTTP request
          break;
      }
    }

    

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body

    //print out page based on user input
    String html = String(HTML_CONTENT1); //first part of html
    client.println(html);

    client.println(page); //page content bnased on user input
    
    html = String(HTML_CONTENT2); //last part of html
    client.println(html);
    
    client.flush();

    delay(10);

    client.stop();
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("END OF ACTION...");
    Serial.println("");
  }
}

