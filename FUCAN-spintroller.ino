#include "config.ino"

void setup()
{
  Serial.begin(serialSpeed);
  Serial.println("");
  Serial.println("");
  Serial.println("Initialising FUCAN-spintroller...");

  /* Steps per revolution is the calculated number of steps required to rotate
   * the workpiece one revolution.
   */
  int stepsPerRevolution = microsteps * pulley * pitch;
  Serial.write("Pitch set to: [", pitch,"]");
  Serial.write("Pulley ratio: [", pulley,"]");
  Serial.write("Microsteps:   [", microsteps,"]");
  Serial.write("");
  Serial.write("Setting up output pins.")
  // Initialise all the output pins
  pinMode(pinStep, OUTPUT);
  digitalWrite(pinStep, (invertStep)? HIGH: LOW);

  pinMode(pinDir, OUTPUT);
  digitalWrite(pinDir, (invertDir)? HIGH: LOW);

  pinMode(pinEn, OUTPUT);
  digitalWrite(pinEn, (invertEn)? HIGH: LOW);

  Serial.write("Initialisation complete.")
}

void loop()
{
  // put your main code here, to run repeatedly:
}
