/*
 * Set pitch to be the ratio of the worm gear in the rotary table. Typically
 * this value is either 40:1 or 90:1, so enter either 40 or 90. It means 40 or
 * 90 rotations of the hand wheel will rotate the work piece one revolution.
 */
int worm = 90;

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
 * This variable is calculated in setup() but not actually used anywhere (yet)
 */

/*
 * OUTPUT PINS
 * These pins are connected to the stepper motor driver (such as an M542).
 * Three pins are needed, pinStep, pinDir, and pinEn:
 *
 *  o pinStep - toggles once per step (or microstep)
 *  o pinDir  - used to set the direction. Currently FACUN-spintroller only
 *    supports travel in a single direction to minimise backlash. To reverse
 *    direction, toggle this value.
 *  o pinEn   - enables the stepper driver.
 *
 * All stepper output pins can be set be set for inverted logic allowing a wide
 * range of controllers to be employed.
 */
int pinStep = 3; // STEP pin for stepper driver
int pinDir  = 4; // DIR pin for stepper driver
int pinEn   = 5; // ENABLE pin for stepper driver
bool invertStep = false; // invert STEP?
bool invertDir  = false; // invert DIR?
bool invertEn   = true;  // invert ENABLE?

/*
 * stepPeriod
 * The stepPeriod value is the width of a step pulse sent to the stepper
 * driver. It determines how fast the motor will spin. Many drivers are capable
 * of being run with a period of 1.5µs (1.5 microseconds) between each pulse,
 * however there are several caveats:
 *
 * 1) The Arduino probably will struggle to keep up with anything faster than
 *    20µs (50kHz).
 * 2) The stepper motor itself might stall if it is under too much load for a
 *    speed that fast.
 * 3) The controller may not be able to keep up with signalling that fast.
 *
 * Most rotary tables are not being run at great speed for this is not a lathe.
 * Given the default settings of 1600 microsteps, 4:1 pulley ratio and a 90:1
 * worm ratio, we can calculate how long a complete revolution of the part will
 * take in seconds:
 * stepsPerRevolution = microsteps * pulley * worm
 *                    = 1600 * 4 * 90
 *                    = 576 000 steps.
 * timePerRevolution  = 2 * stepPeriod * stepsPerRevolution / 100 000
 *                    = 2 * 50 µs * 576 000
 *                    = 5.76x10^7 µs
 *                    = 57.6 seconds.
 *
 * This sounds awfully long, but is actually relatively. Suppose you are cutting
 * a 32 tooth gear, it will take 57.6/32 seconds to move around to each tooth.
 * This is 1.8 seconds, and when you consider how long it takes to crank the
 * hand wheel using dividing plates through multiple rotations, this is actually
 * a significant improvement, and explains why the value of 50µs was chosen.
 */
unsigned short stepPeriod = 50;

/*
 * INPUT PINS
 * These pins are connected to switches to control the microcontroller.
 *
 *  o pinStart  - CYCLESTART pin - will tell the controller to start the next
 *    movement.
 *  o pinAbort  - ABORT pin - will halt movement of stepper.
 *
 * Pins can be set for inverted logic allowing connection to limit switches, or
 * other control systems, as well as allowing designer to use either pullup or
 * pull down resistors as they see fit. If using inverted logic, choose pull up
 * resistors, and the switch pulls to GND. If using regular logic, choose pull
 * down resistors and the switch pulls up to VCC.
 */
int pinStart = 6; // CYCLESTART button
int pinAbort = 7; // ABORT button
bool invertStart = false; // invert CYCLESTART?
bool invertAbort = false; // invert ABORT?

/*
 * Serial Communication
 * Set the desired speed to communicate with host PC. Eventually, this option
 * will become deprecated when a keypad and LCD is added.
 */
int serialSpeed = 115200; // 9600, 19200, 38400, 57600 and 115200 are common
