// GAMETORY CAR SCANNER
// ROTATES 60°, PAUSE 1 SEC
// WHEN REACHES 360°, PAUSE 3 SEC

#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 2, 5);

// Number of stop positions in one full rotation
// 0°, 60°, 120°, 180°, 240°, 300°, 360°
const int NUM_POSITIONS = 7;

// Absolute step positions for each angle
// Based on 1600 steps per revolution (1/8 microstepping)
long positions[NUM_POSITIONS] = {
  0,      // 0°
  267,    // 60°
  533,    // 120°
  800,    // 180°
  1067,   // 240°
  1333,   // 300°
  1600    // 360°
};

// Keeps track of which position we are moving to
int index = 0;

// Used to control pause timing
bool waiting = false;
unsigned long pauseStart = 0;

// Default pause time between steps (1 second)
int pauseTime = 1000;

void setup() {

  // Maximum speed of the motor (steps per second)
  stepper.setMaxSpeed(800);

  // Acceleration rate (steps per second^2)
  stepper.setAcceleration(400);

  // Move to the first position (0°)
  stepper.moveTo(positions[index]);
}

void loop() {

  // If we are not currently waiting, keep the motor moving
  if (!waiting) {

    stepper.run();  // required for AccelStepper to move the motor

    // Check if the motor reached its target position
    if (stepper.distanceToGo() == 0) {

      // Start pause timer
      waiting = true;
      pauseStart = millis();

      // At 360° pause longer (3 seconds)
      if (index == 6)
        pauseTime = 3000;
      else
        pauseTime = 1000;  // normal stops pause 1 second
    }
  }

  // After the pause finishes, move to the next position
  if (waiting && millis() - pauseStart >= pauseTime) {

    waiting = false;

    // Move to the next angle
    index++;

    // If we finished a full rotation
    if (index >= NUM_POSITIONS) {
      index = 0;

      // Reset internal position so the next rotation stays accurate
      stepper.setCurrentPosition(0);
    }

    // Move to the next target angle
    stepper.moveTo(positions[index]);
  }
}