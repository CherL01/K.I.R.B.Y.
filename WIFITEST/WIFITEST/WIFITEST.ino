#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "WiFiS3.h"
#include "index.h"


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


//---USER INPUT PARAMETERS
int cardSpeed = 0; // set card speed
int numPlayers = 0; // set card speed
String gameType = "None"; // set game type

//---LED---
ArduinoLEDMatrix matrix;

const uint32_t defaults[] = { //default LED
  0xcdfcdfcc,
  0x4fc4fc4c,
  0xc4cdfcdf,
  66
};

const uint32_t on[] = { //running LED
  0x00000000,
  0x00000000,
  0x00000000,
  66
};

const uint32_t setting[] = { //setting LED
  0x00001111,
  0x00001111,
  0x00001111,
  66
};


void setup() {
  Serial.begin(9600); // initialize serial communication
  matrix.begin(); //initialize LED matrix

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
  
  //Start web server on port 80
  server.begin();
  printWifiStatus(); // you're connected now, so print out the status
  //---/SETTING UP WIFI---//
}


void loop() {
  webServer();
  
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
//---END WIFI/HTML FUNCTIONS---
