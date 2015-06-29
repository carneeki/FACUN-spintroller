#include <Streaming.h> // for stdio style << operators
#include "config.h"

volatile unsigned int state = 0; // finite state machine tracker

String buf = ""; // buffer for reading from serial

unsigned long division=0; // current division counter
unsigned long totalDivisions=0; // number of total divisions to cut
unsigned long stepsPerDiv=0;   // number of steps per division

void setup()
{
  Serial.begin(serialSpeed);
  Serial << endl << endl;
  Serial << "Initialising FUCAN-spintroller..." << endl;

  // Initialise all the output pins
  Serial << "Setting up output pins...";
  pinMode(pinStep, OUTPUT);
  digitalWrite(pinStep, invertStep^false );

  pinMode(pinDir, OUTPUT);
  digitalWrite(pinDir, invertDir^false);

  pinMode(pinEn, OUTPUT);
  digitalWrite(pinEn, invertEn^false);

  pinMode(pinStart, INPUT);
  Serial << "  [DONE]" << endl;

  Serial << "- Configuration ----------------" << endl;
  Serial << "Microsteps:   [" << microsteps << "]" << endl;
  Serial << "Pulley ratio: [" << pulley << "]" << endl;
  Serial << "Worm ratio:   [" << worm << "]" << endl;
  Serial << "--------------------------------" << endl << endl;
  Serial << "Initialisation complete, entering finite state machine." << endl;
}

void loop()
{
  switch(state)
  {
    // get number of divisions
    case 1:
      getDivisions();

      if(totalDivisions == 0)
      {
        return;
      }

      if(totalDivisions < 3)
      {
        Serial << "ERROR! Divisions currently set to [" << totalDivisions << "]. must be 3 or greater." << endl;
        state = 1;
        return;
      }

      division = 1;
      stepsPerDiv = stepsPerRevolution / totalDivisions;

      state = 2;
      return;
    break;

    // display message to run cutter
    case 2:
      digitalWrite(pinEn, invertEn^true);
      Serial << "Run cutter for division " << division
                                    << "/" << totalDivisions << ". ";
      Serial << "Press CYCLE START when cutter has cleared." << endl;

      // if last division, wait for CS to display complete message
      if(division == totalDivisions)
      {
        state = 5;
        return;
      }

      state = 3;
      return;
    break;

    // awaiting CYCLESTART
    case 3:
      // if button pushed, continue to state 3
      delay(5);
      if(digitalRead(pinStart) == HIGH || getCycleStart())
        state = 4;
      return;
    break;

    // rotate workpiece
    case 4:
      division++;
      if(division <= totalDivisions)
      {
        Serial << "Rotating to division " << division << "/"
                                          << totalDivisions << " ";
        for(unsigned long i = 0; i < stepsPerDiv; i++)
        {
          digitalWrite(pinStep, invertStep^true);
          delayMicroseconds(stepPeriod);
          digitalWrite(pinStep, invertStep^false);
          delayMicroseconds(stepPeriod);
        }

        Serial << "[ DONE ]" << endl;
      }
      state = 2;
      return;
    break;

    // awaiting CYCLESTART for final pass
    case 5:
      // if button pushed, continue to state 3
      delay(5);
      if(digitalRead(pinStart) == HIGH || getCycleStart())
      {
        Serial << "--------------------------------" << endl;
        Serial << "Operation complete." << endl;
        Serial << "--------------------------------" << endl;
        state = 0;
      }
      return;
    break;

    // unknown, unset, or 0 state for FSM
    default:
      division = 0; // zero out the divisions counters
      totalDivisions = 0;
      Serial << endl << "Enter number of divisions (3 or greater): ";
      state = 1;
    break;
  }
}

void getDivisions()
{
  int inByte;
  while(Serial.available())
  {
    char inByte = Serial.read();
    if(isDigit(inByte))
    {
      buf += inByte;
      Serial << inByte;
    }

    if(inByte == '\n' || inByte == '\r')
    {
      Serial << endl;
      totalDivisions = (unsigned int) buf.toInt();
      buf="";
    }
  }
}

bool getCycleStart()
{
  int inByte;
  while(Serial.available())
  {
    char inByte = Serial.read();
    if(inByte=='!')
    {
      state = 4;
      return true;
    }
  }
}
