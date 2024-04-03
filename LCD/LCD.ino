/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 https://docs.arduino.cc/learn/electronics/lcd-displays

*/

#include <Wire.h>
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 4, en = 6, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte a = 0;

void setup() {
  Wire.begin(4);                  // join i2c bus with address 4
  Wire.onReceive(receiveEvent);   // register event
  Serial.begin(9600);             // start serial for output
  
  lcd.begin(20, 4);
  
  /* ---Tutorial sample code--- 
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  */
  lcd.print("Hi! I am KIRBY. <3");
}

void loop() {
  /* test display
  lcd.setCursor(0,1);
  lcd.print(millis()/1000);
  
  lcd.setCursor(0,2);
  lcd.print("hello world");
  Serial.println("hello world");
  delay(5000);
  lcd.setCursor(0,2);
  lcd.print("goodbye world");
  Serial.println("goodbye world");
  delay(5000);
  */
  
  /* ---Tutorial sample code---
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  */
}

void receiveEvent(int num) {
  String message="";
  char firstChar="";

  // Holds first character - determines which row to print on LCD
  if (Wire.available()) {
    firstChar = Wire.read();
    message.concat(String(firstChar));
    Serial.print(firstChar);
  }
  // while loop: receives Cards dealt, Game, or Players
  while (Wire.available() > 1) {
    char c = Wire.read();
    message.concat(String(c));
    Serial.print(c);
  }
  
  // depending on first char received (C, G or P) do something different
  if (firstChar == 'C') {
    int x = Wire.read();
    message.concat(String(x));
    Serial.println(x);

    lcd.setCursor(0,3);
    lcd.print(message);
    
  } else if (firstChar == 'G') {
    char y = Wire.read();
    message.concat(String(y));
    Serial.println(y);
    
    lcd.setCursor(0,1);
    lcd.print(message);
  } else if (firstChar == 'P') {
    int x = Wire.read();
    message.concat(String(x));
    Serial.println(x);
    
    lcd.setCursor(0,2);
    lcd.print(message);
  }
  

}

// SendLCD(int cardsDealt, int numPlayers, String gameType, bool sameGame)
// https://docs.arduino.cc/learn/communication/wire/
