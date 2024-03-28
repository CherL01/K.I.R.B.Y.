/*
  Melody

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 8 & ground

  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
*/

#include "pitches.h"
#include <ezBuzzer.h>

const int musicPin = 8;

ezBuzzer buzzer(musicPin);

int noteIndex;

// Dealing melody
int dealMelody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

// Dealing melody note durations: 4 = quarter note, 8 = eighth note, etc.:
int dealNoteDurations[] = {
  4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4,
  4, 4, 2, 4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4,
  4, 4, 2, 4, 4, 4, 4, 4, 4, 2
};

// End of melody 
int endMelody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// End melody note durations: 4 = quarter note, 8 = eighth note, etc.:
int endNoteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int noteLength;

void setup() {
  Serial.begin(9600);
  

}

void loop() {
  musicEnd();
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
//  for (noteIndex = 0; noteIndex < sizeof(endMelody); noteIndex++) 
//  {
//
//    // to calculate the note duration, take one second divided by the note type.
//    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//    int noteDuration = 1000 / endNoteDurations[noteIndex];
//    tone(musicPin, endMelody[noteIndex], noteDuration);
//
//    // to distinguish the notes, set a minimum time between them.
//    // the note's duration + 30% seems to work well:
//    int pauseBetweenNotes = noteDuration * 1.30;
//    delay(pauseBetweenNotes);
//    // stop the tone playing:
//    noTone(musicPin);
//  }

  buzzer.loop();
  noteLength = sizeof(endNoteDurations) /sizeof(int);

  if (buzzer.getState() == BUZZER_IDLE)
  {
    buzzer.playMelody(endMelody, endNoteDurations, noteLength);
  }
}
