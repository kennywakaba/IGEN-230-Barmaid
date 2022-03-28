#include <HX711_ADC.h>
#include <EEPROM.h>
#include <AccelStepper.h>

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
const int RST = 2;

const int valve1 = 8;
const int valve2 = 9;
const int valve3 = 10;
const int valve4 = 11;
const int valve5 = 12;
const int valve6 = 13;

const int dirPin = 7;
const int stepPin = 6;
const int endPin = 13;

const int pushBottom = 2;
const int pushTop = 3;

float weight;
long origin = 0;
long station1 = 400;
long station2 = 940;
long station3 = 1460;
long station4 = 1990;
long station5 = 2510;
long station6 = 2975

int sequence1 = 0;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

AccelStepper stepper(1, stepPin, dirPin);

void RESET() {
  Serial.println("Reset");
  LoadCell.start(2000, true);

  digitalWrite(valve6, HIGH);
  digitalWrite(valve5, HIGH);
  digitalWrite(valve4, HIGH);
  digitalWrite(valve3, HIGH);
  digitalWrite(valve2, HIGH);
  digitalWrite(valve1, HIGH);
}

void ENDSTOP() {
  Serial.println("Origin detected!");
  stepper.setCurrentPosition(0);
}

void DRIVE(long Position) {
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
  stepper.setMaxSpeed(1400);
  stepper.setSpeed(1400);
  stepper.setAcceleration(1200);
  stepper.setCurrentPosition(0);
  stepper.disableOutputs();

  // VALVE SETUP
  pinMode(valve2, OUTPUT);
  pinMode(valve4, OUTPUT);
  pinMode(RST, INPUT);
  pinMode(endPin, INPUT);
  //attachInterrupt (0, RESET, RISING);
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  if (stepper.currentPosition() != station2 || weight > 10) {
    digitalWrite(valve2, HIGH);
  }
  if (stepper.currentPosition() != station4 || weight > 10) {
    digitalWrite(valve4, HIGH);
  }

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      weight = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(weight);
      newDataReady = 0;
      t = millis();

      if (digitalRead(pushBottom) == HIGH) { // black button
        DRIVE(station4);
        sequence1 = 1;
        RESET();
      }
      if (digitalRead(pushTop) == HIGH) { // red button
        DRIVE(station4);
        sequence1 = 1;
        RESET();
      }

      if (digitalRead(endPin) == HIGH) { // indicating the origin
        //ENDSTOP();
      }

      // sequence 1: station4 -> station2
      // note the order of operation is reversed, this is due to generalized if statements that simplifies variable manipulations
      if (sequence1 = 1) {
        if (stepper.currentPosition() == station2 && weight < 10) { // step 2
          digitalWrite(valve2, LOW);
        }
        else if (stepper.currentPosition() == station2 && weight > 10) {
          digitalWrite(valve2, HIGH);
          delay(5000); // wait for the leakage to end
          DRIVE(origin);
          sequence1 = 0;
          RESET();
        }
        if (stepper.currentPosition() == station4 && weight < 10) { // step 1
          digitalWrite(valve4, LOW);
        }
        else if (stepper.currentPosition() == station4 && weight > 10) {
          digitalWrite(valve4, HIGH);
          delay(5000); // wait for the leakage to end
          DRIVE(station2);
          RESET();
        }
      }
    // sequence 1: 4 -> 2

    
    }
  }
}
