/*
 * Set pitch to be the ratio of the worm gear in the rotary table. Typically
 * this value is either 40:1 or 90:1, so enter either 40 or 90. It means 40 or
 * 90 rotations of the hand wheel will rotate the work piece one revolution.
 */
int pitch = 90;

/*
 * Set ratio to be the pulley ratio between the stepper motor or servo and the
 * handle on the rotary table. It is best to design for a whole number. If the
 * ratio is 4:1, then enter 4.
 */
int pulley = 4;

/*
 * Set the number of steps for the stepper or servo to complete one rotation of
 * the motor shaft. Many servos will actually use encoders and might have 4096
 * 'steps'. Many steppers will allow microstepping, allowing maybe 1600 steps
 * or more.
 */
int microsteps = 1600;

/* As a sort of sanity check, work out how many steps on the motor are required
 * to rotate the workpiece one whole revolution. It should be:
 *
 *   stepsPerRevolution = steps * pulley * pitch;
 *
 * This figure is calculated in setup() but not actually used anywhere.
 */

int pinStep = 3; // STEP pin for stepper driver
int pinDir  = 4; // DIR pin for stepper driver
int pinEn   = 5; // ENABLE pin for stepper driver
bool invertStep = false; // Should STEP pin be inverted?
bool invertDir  = false; // Should DIR pin be inverted?
bool invertEn   = true; // Should enable pin be inverted?

int serialSpeed = 115200; // 9600, 19200, 38400, 57600 and 115200 are common

void setup()
{
  Serial.begin(serialSpeed);
  Serial.write("");
  Serial.write("");
  Serial.write("Initialising FUCAN-spintroller...");

  /* Steps per revolution is the calculated number of steps required to rotate
   * the workpiece one revolution.
   */
  int stepsPerRevolution = microsteps * pulley * pitch;
  Serial.write("Pitch set to: [", pitch,"]");
  Serial.write("Pulley ratio: [", pulley,"]");
    Serial.write("Microsteps:   [", microsteps,"]");

  // Initialise all the output pins
  pinMode(pinStep, OUTPUT);
  digitalWrite(pinStep, (invertStep)? HIGH: LOW);

  pinMode(pinDir, OUTPUT);
  digitalWrite(pinDir, (invertDir)? HIGH: LOW);

  pinMode(pinEn, OUTPUT);
  digitalWrite(pinEn, (invertEn)? HIGH: LOW);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
