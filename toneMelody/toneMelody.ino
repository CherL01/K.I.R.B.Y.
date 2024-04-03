/*
  Melody
  - 8 ohm speaker on digital pin 8 & ground

  includes "ezBuzzer" library

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
*/

#include "pitches.h"
#include <ezBuzzer.h>

const int musicPin = 8;

ezBuzzer buzzer(musicPin);

int noteIndex;

// Dealing Melody: KIRBY Dreamland Theme song
int dealMelody[] = {
  NOTE_E5, 0, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_B4, 0,
  NOTE_A5, 0, NOTE_E5, 0, NOTE_C5, NOTE_B4, NOTE_A4, 0, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_E4, 0,
  NOTE_A5, 0, NOTE_E5, 0, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_A4, 0,
  NOTE_A5, 0, NOTE_E5, 0, NOTE_C5, NOTE_B4, NOTE_A4, 0, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_E4, 0, 
  NOTE_A5, 0, NOTE_E5, 0, NOTE_B4, NOTE_D5, NOTE_E5, NOTE_A4, NOTE_B4, NOTE_D5, NOTE_B4, NOTE_G4, NOTE_A4, 0
};

// Dealing melody note durations: 4 = quarter note, 8 = eighth note, etc.:
int dealNoteDurations[] = {
  8, 16384, 16, 16, 16, 16, 8, 16, 16, 16, 16, 8, 8,          
  8, 6, 8, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 6, 8, 6, 8, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 6, 8, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 6, 8, 6, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4 
};

// End Melody: KIRBY Victory song
int endMelody[] = {
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_G5, NOTE_E5, 0, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_B5, NOTE_C6, 0, NOTE_E5, 0, 
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_G5, NOTE_E5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C6, 0  

};

// End melody note durations: 4 = quarter note, 8 = eighth note, etc.:
int endNoteDurations[] = {
  8, 8, 8, 8, 8, 8, 4, 8, 4, 8, 8, 8, 8, 8, 8, 8, 4, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 4, 8, 4, 8, 4, 8, 4, 8, 3, 8, 4
};

int noteLength;

void setup() {
  Serial.begin(9600);
  //musicEnd();
  

}

void loop() {
  //musicEnd();
  delay(200);
  musicDeal();
}

void musicDeal() //play music during dealing
{
//  for (noteIndex = 0; noteIndex < sizeof(dealMelody); noteIndex++) 
//  {
//
//    // to calculate the note duration, take one second divided by the note type.
//    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//    int noteDuration = 1000 / dealNoteDurations[noteIndex];
//    tone(musicPin, dealMelody[noteIndex], noteDuration);
//
//    // to distinguish the notes, set a minimum time between them.
//    // the note's duration + 30% seems to work well:
//    int pauseBetweenNotes = noteDuration * 1.30;
//    delay(pauseBetweenNotes);
//    // stop the tone playing:
//    noTone(musicPin);
//  }
  
  buzzer.loop();
  noteLength = sizeof(dealNoteDurations) /sizeof(int);

  if (buzzer.getState() == BUZZER_IDLE)
  {
    buzzer.playMelody(dealMelody, dealNoteDurations, noteLength);
  }
}

void musicEnd() //play music when done dealing
{
  for (noteIndex = 0; noteIndex < sizeof(endMelody); noteIndex++) 
  {
    
    int noteDuration = 1000 / endNoteDurations[noteIndex];
    tone(musicPin, endMelody[noteIndex], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(musicPin);
  }

//  buzzer.loop(); //try without looping or use the previous code for 1 iteration of melody
//  noteLength = sizeof(endNoteDurations) /sizeof(int);
//
//  if (buzzer.getState() == BUZZER_IDLE)
//  {
//    buzzer.playMelody(endMelody, endNoteDurations, noteLength);
//  }
}
