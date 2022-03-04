#include <HX711_ADC.h>
#include <EEPROM.h>
#include <AccelStepper.h>

//pins:
const int HX711_dout = 5; //mcu > HX711 dout pin
const int HX711_sck = 4; //mcu > HX711 sck pin
const int RST = 2;

const int valve = 3;

const int dirPin = 6;
const int stepPin = 7;
const int endPin = 8;

const int pushA = 11;
const int pushB = 12;

float weight;
long position1 = 0;
long position2 = 2000;

boolean valve_on = 0;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

AccelStepper stepper(1, stepPin, dirPin);

void RESET() {
  Serial.println("Reset");
  LoadCell.start(2000, true);

  digitalWrite(valve, HIGH);
  valve_on = 0;
}

void ENDSTOP() {
  Serial.println("Origin detected!");
  stepper.setCurrentPosition(0);
}

void DriveMotor(long Position) {
  stepper.enableOutputs();
  stepper.moveTo(Position);
  while (stepper.currentPosition() != Position) {
    stepper.run();
  }
  stepper.disableOutputs();
}

void setup() {
  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  //calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
  
  // MOTOR SETUP
  stepper.setMaxSpeed(2000);
  stepper.setSpeed(2000);
  stepper.setAcceleration(200);
  stepper.setCurrentPosition(0);
  stepper.disableOutputs();

  // VALVE SETUP
  pinMode(valve, OUTPUT);
  pinMode(RST, INPUT);
  pinMode(endPin, INPUT);
  //attachInterrupt (0, RESET, RISING);
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  if (stepper.currentPosition() != position2 || weight > 45) {
    digitalWrite(valve, HIGH);
  }

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      weight = LoadCell.getData() * -1;
      Serial.print("Load_cell output val: ");
      Serial.println(weight);
      newDataReady = 0;
      t = millis();

      if (digitalRead(pushA) == HIGH) {
        DriveMotor(position1);
      }
      if (digitalRead(pushB) == HIGH) {
        DriveMotor(position2);
        valve_on = 1;
      }

      if (digitalRead(endPin) == HIGH) { // indicating the origin 
        ENDSTOP();
      }

      if (stepper.currentPosition() == position2 && weight < 45 && valve_on == 1) {
        digitalWrite(valve, LOW);
      }
      else if (stepper.currentPosition() == position2 && weight > 45) {
        digitalWrite(valve, HIGH);
        RESET();
      }

    }
  }

}
