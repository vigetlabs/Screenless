#include "SLbuzzer.h"

SLbuzzer::SLbuzzer() {}

void SLbuzzer::initialize() {
}

void SLbuzzer::updateState() {
  if (_currentSoundEffect == SONG) { _determineSongState(); }
}

void SLbuzzer::show() {
  //this is what makes the tone
  if (_currentSoundEffect == NONE) {
    noTone(BUZZERPIN);
  } else {
    tone(BUZZERPIN, _currentTone,0);
    /* //DEBUGGING
    Serial.print("Playing Sound: ");
    Serial.println(_currentTone);
    */
  }
}

void SLbuzzer::createSong(String song) {
  _songString = song;
  //_songString.toCharArray(_inputStr,200);
  _songBPM = _songBPM; //defaulting to the default set in SLbuzzer.h. But, this could be set on createSong.
  _currentSoundEffect = SONG;
  _songCurrentlyPlaying = true;
  _songStartTime = millis();
  _songLength = _getSongLengthMS(song);

  Serial.print("Creating Song, length: ");
  Serial.println(_songLength);
}

//private

void SLbuzzer::_determineSongState() {
  if(_songCurrentlyPlaying) {
    if (millis() < (_songStartTime + _songLength)) {
      //cache the tone so you can avoid some extra function calls and math
      String previousNote = _currentNote;
      String currentNote = _getCurrentNote();
      //if (previousNote != currentNote) {
        _currentTone = _noteToTone(currentNote);         //get the new tone
      //}
      //_currentTone = _noteToTone(_getCurrentNote());
      /* //DEBUGGING
      Serial.print("Current Tone: ");
      Serial.println(_currentTone);
      */
    } else {
      _songCurrentlyPlaying = false;
      _currentTone = 1;
      _currentNote = "C4"; // just some default
    }
  } else {
    _currentSoundEffect = NONE;
  }
}

int SLbuzzer::_getSongLengthMS(String song){
  //returns the length of the song in MS;
  char inputStr[200];
  song.toCharArray(inputStr,200);
  int length = 0;

  //get first record
  char *note = strtok(inputStr,",");
  char *duration = strtok(NULL,",");
  length = length + (1000/atoi(duration));

  //loop through the song. As we move through it, figure out what note we should be currently playing
  while(duration != NULL){
    note = strtok(NULL,",");
    duration = strtok(NULL,", \n");
    if (note != NULL) {
      length = length + (1000/atoi(duration));
    }
    /*
    Serial.print(" looping note: ");
    Serial.print(note);
    Serial.print(" looping duration: ");
    Serial.print(atoi(duration));
    Serial.print(" current Length: ");
    Serial.println(length);
    */
  }
  return length; //in ms (override with 1600 atm)
}

int SLbuzzer::_noteToTone(String note){
  //in goes a note
  //map that note to a frequency -- this would be less expensive.
  // less expensive still, figure out if the note has changed.

  int noteNum = 0;
  int octave = 5;
  int freq = 256;
  char octavo[5];

  String tempString = note.substring(1,2);
  tempString.toCharArray(octavo,5);
  octave = atoi(octavo);

  switch(note.charAt(0)){
          case 'C':
              noteNum = 0;
              break;
          case 'D':
              noteNum = 2;
              break;
          case 'E':
              noteNum = 4;
              break;
          case 'F':
              noteNum = 5;
              break;
          case 'G':
              noteNum = 7;
              break;
          case 'A':
              noteNum = 9;
              break;
          case 'B':
              noteNum = 11;
              break;
          case 'R':          // Reset note
              octave = -1;
              break;
          default:
              break;
      }

  freq = float(256*pow(1.05946,(12.0*(octave-4)+noteNum)));
  /* //DEBUGGING
  Serial.print("Note: ");
  Serial.print(note);
  Serial.print(" noteNum: ");
  Serial.print(noteNum);
  Serial.print(" octave: ");
  Serial.print(octave);
  Serial.print(" freq: ");
  Serial.println(freq);
  */
  return freq;
}

String SLbuzzer::_getCurrentNote(){
  //figure out how far along you are in the song (ms)
  int timeElapsed = millis() - _songStartTime;
  int currentSongTime = 0;

  char inputStr[200];
  _songString.toCharArray(inputStr,200);
  int length = 0;

  //get first record
  char *note = strtok(inputStr,",");
  char *duration = strtok(NULL,",");
  length = length + (1000/atoi(duration));

  if (duration == NULL) { Serial.println("FAILED DURATION"); }

  //loop through the song. As we move through it, figure out what note should be currently playing
  while(duration != NULL){
    /* //DEBUGGING
    Serial.print(" looping note: ");
    Serial.print(note);
    Serial.print(" looping duration: ");
    Serial.print(atoi(duration));
    Serial.print(" current Length: ");
    Serial.print(length);
    Serial.print(" time elapsed: ");
    Serial.print((millis() - _songStartTime));
    Serial.print(" start time: ");
    Serial.print(_songStartTime);
    Serial.print(" millis: ");
    Serial.println(millis());
    */
    if (note != NULL) {
      length = length + (1000/atoi(duration));
      //length = length + (60*60) //60 bpm 3600 bpsecond or 3600000 bpms
    }
    if ((millis() - _songStartTime) <= length) {
      _currentNote = note;
      break;
    }
    //increase by one for the next loop
    note = strtok(NULL,",");
    duration = strtok(NULL,", \n");
  }
  /* //DEBUGGING
  Serial.print("Current Note: ");
  Serial.println(_currentNote);
  */

  return _currentNote;
}
