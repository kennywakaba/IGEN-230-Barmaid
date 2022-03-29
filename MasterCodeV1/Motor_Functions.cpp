#include "Motor_Functions.h"

//variable initialization

long desiredPosition = 0;
long Position = 0;

const int dirPin = 7;
const int stepPin = 6;

//motor configuration
AccelStepper stepper(1, stepPin, dirPin);

void motorSetup(){
  stepper.setMaxSpeed(1400);
  stepper.setSpeed(1400);
  stepper.setAcceleration(1200);
  stepper.setCurrentPosition(0);
  stepper.disableOutputs();
}

void driveMotor(long Position) {

  stepper.enableOutputs();
  stepper.moveTo(Position);
  while (stepper.currentPosition() != Position) {
    stepper.run();
  }
  stepper.disableOutputs();
}
