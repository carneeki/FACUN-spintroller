#include <Streaming.h> // for stdio style << operators
#include "config.h"

volatile unsigned int state = 0; // finite state machine tracker

String buf = ""; // buffer for reading from serial

unsigned int division; // current division counter
unsigned int totalDivisions; // number of total divisions to cut
unsigned int stepsPerDiv; // number of steps per division
unsigned int stepsPerRevolution; // number of steps per revolution

void setup()
{
  Serial.begin(serialSpeed);
  Serial << endl << endl;
  Serial << "Initialising FUCAN-spintroller..." << endl;

  /* Steps per revolution is the calculated number of steps required to rotate
   * the workpiece one revolution.
   */
  stepsPerRevolution = microsteps * pulley * worm;

  Serial << "Microsteps:   [" << microsteps << "]" << endl;
  Serial << "Pulley ratio: [" << pulley << "]" << endl;
  Serial << "Worm set to:  [" << worm << "]" << endl;
  Serial << endl;

  // Initialise all the output pins
  Serial << "Setting up output pins." << endl;
  pinMode(pinStep, OUTPUT);
  digitalWrite(pinStep, invertStep^false );

  pinMode(pinDir, OUTPUT);
  digitalWrite(pinDir, invertDir^false);

  pinMode(pinEn, OUTPUT);
  digitalWrite(pinEn, invertEn^false);

  pinMode(pinStart, INPUT);

  // zero out the divisions to cut
  division = 0;

  Serial << "Initialisation complete. System ready." << endl;
}

void loop()
{
  switch(state)
  {
    // display message to run cutter
    case 1:
      if(division == totalDivisions)
      {
        Serial << "Operation complete." << endl;
        state = 0;
        return;
      }
      digitalWrite(pinEn, invertEn^true);
      Serial << "Run cutter for division " << division
                                    << "/" << totalDivisions << endl;
      Serial << "Press CYCLE START when ready to rotate." << endl;
      state = 2;
      return;
    break;

    // awaiting CYCLESTART
    case 2:
      // if button pushed, continue to state 3
      delay(5);
      if(pinStart == HIGH)
        state = 3;
      return;
    break;

    // rotate workpiece
    case 3:
      division++;
      if(division <= totalDivisions)
      {
        Serial << "Rotating to division " << division << "/"
                                          << totalDivisions << endl;
        for(int i = 0; i < stepsPerDiv; i++)
        {
          digitalWrite(pinStep, invertStep^true);
          delayMicroseconds(stepPeriod);
          digitalWrite(pinStep, invertStep^false);
          delayMicroseconds(stepPeriod);
        }
      }
      state = 1;
      return;
    break;

    // unknown, unset, or 0 state for FSM
    default:
      totalDivisions = getDivisions();
      if(totalDivisions < 3)
      {
        Serial << "ERROR! Divisions must be 3 or greater." << endl;
        return;
      }

      division = 1;
      stepsPerDiv = stepsPerRevolution / totalDivisions;

      state = 1;
      return;
    break;
  }
}

unsigned int getDivisions()
{
  buf = "";

  Serial << "Enter number of divisions (3 or greater): ";
  while(Serial.available() > 0)
  {
    int inChar = Serial.read();
    if(isDigit(inChar))
    {
      buf += (char) inChar;
      Serial << inChar;
    }

    if(inChar == '\n')
      return (unsigned int) buf.toInt();
  }
}
