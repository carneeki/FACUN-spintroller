#include <Streaming.h> // for stdio style << operators
#include "config.h"
//#include "helpers.cpp"

volatile unsigned int state = 0; // finite state machine tracker

String buf; // buffer for serial input

unsigned long division=0; // current division counter
unsigned long totalDivisions=0; // number of total divisions to cut
unsigned long stepsToDrive=0;   // number of steps to be driven

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
    // MSG:Select mode
    case 0:
      // zero ALL the things!
      buf="";
      division = 0;
      totalDivisions = -1;
      stepsToDrive = 0;

      Serial << endl << "Select mode: (D)ivisions or (A)ngle: ";
      state = 1;
    break;

    // INPUT:Mode
    case 1:
      getMode();
    break;

    // MSG:Division entry
    case 2:
      Serial << endl << "Enter number of divisions (2 or greater; Q to quit): ";
      state = 3; // division input state
    break;

    // INPUT:Number of divisions
    case 3:
      getDivisions();

      if(totalDivisions == -1)
      {
        return;
      }

      if(totalDivisions < 2)
      {
        Serial << "ERROR! Divisions currently set to [" << totalDivisions << "]. must be 2 or greater." << endl;
        totalDivisions = -1;
        state = 2; // MSG: division entry
        return;
      }

      division = 1;
      stepsToDrive = stepsPerRevolution / totalDivisions;

      state = 4; // MSG: run cutter
      return;
    break;

    // MSG: run cutter
    case 4:
      digitalWrite(pinEn, invertEn^true);
      Serial << "Run cutter for division " << division
                                    << "/" << totalDivisions << ". ";
      Serial << "Press CYCLE START when cutter has cleared." << endl;

      // if last division, wait for CS to display complete message
      if(division == totalDivisions)
      {
        state = 7; // WAIT: CS for final pass
        return;
      }

      state = 5; // WAIT: CS for next pass
      return;
    break;

    // WAIT:CS to rotate
    case 5:
      delay(5);
      if(digitalRead(pinStart) == HIGH || getCycleStart())
        state = 6; // MSG: run cutter
      return;
    break;

    // ACTION:rotate
    case 6:
      division++;
      if(division <= totalDivisions)
      {
        Serial << "Rotating to division " << division << "/"
                                          << totalDivisions << " ";
        rotateSteps();
      }
      state = 4; // MSG: run cutter
      return;
    break;

    // WAIT: CS for final pass
    case 7:
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

    // MSG:Angle entry
    case 8:
      Serial << "Enter angle in decimal degrees (Q to quit): ";
      state = 9;
    break;

    // INPUT:angle
    case 9:
      getAngle();
    break;

    // ACTION:rotate n steps
    case 10:
      Serial << "Rotating... ";
      rotateSteps();

      state=8;
    break;
    // unknown, unset mode
    default:
      state = 0; // MSG: Select mode
    break;
  }
}
