#include <HX711_ADC.h>
#include <EEPROM.h>
#include <AccelStepper.h>

//pins:
const int HX711_dout = 5; //mcu > HX711 dout pin
const int HX711_sck = 4; //mcu > HX711 sck pin
const int RST = 2;

const int valve1 = 8;
const int valve2 = 9;
const int valve3 = 10;
const int valve4 = 11;
const int valve5 = 12;
const int valve6 = 13 ;

const int dirPin = 7;
const int stepPin = 6;
const int endPin = 9;

float weight;
long origin = 0;
long rum = 1000;
long coke = 1500;

boolean valve1_on = 0;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

// motor constructor
AccelStepper stepper(1, stepPin, dirPin);

// user interface variablesint
int input = 0;
int order = 1;

void RESET() {
  Serial.println("Reset");
  LoadCell.start(2000, true);

  digitalWrite(valve1, HIGH);
  valve1_on = 0;
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
  stepper.setMaxSpeed(1400);
  stepper.setSpeed(1400);
  stepper.setAcceleration(1200);
  stepper.setCurrentPosition(0);
  stepper.disableOutputs();

  // valve1 SETUP
  pinMode(valve1, OUTPUT);
  pinMode(RST, INPUT);
  //pinMode(endPin, INPUT);
  //attachInterrupt (0, RESET, RISING);

  Serial.println("");
  Serial.println("Welcome to the virtual Barmaid!");
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

      ////////////////////////////////////////////// USER INTERFACE //////////////////////////////////////////////

//      Serial.println("Please enter a number 1 through to 6: \n1: Rum and Coke \n2: Gin and Tonic \n3: Old Fashion \n4: Soju and Sprite \n5: Go Crazy \n6: Aim on these men");
//      while (order == 0) {
//        while (Serial.available() > 0) {
//          input = Serial.parseInt();
//          if (input != 0) {
//            Serial.println(input);
//            order = input;
//          }
//        }
//        if (order == 1) {
//          Serial.println("Rum and Coke");
//          Serial.println("");
//          Serial.println("Please enter a number 1 through to 6: \n1: Rum and Coke \n2: Gin and Tonic \n3: Old Fashion \n4: Soju and Sprite \n5: Go Crazy \n6: Aim on these men");
//        }
//        else if (order == 2) {
//          Serial.println("Gin and Tonic Sequence");
//          Serial.println("");
//          Serial.println("Please enter a number 1 through to 6: \n1: Rum and Coke \n2: Gin and Tonic \n3: Old Fashion \n4: Soju and Sprite \n5: Go Crazy \n6: Aim on these men");
//        }
//      }

      ////////////////////////////////////////////// USER INTERFACE //////////////////////////////////////////////

  if (stepper.currentPosition() != rum || weight > 45) {
    digitalWrite(valve1, HIGH);
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

      if (order = 0) {
        DriveMotor(origin);
      }

      if (order = 1) { // Coke and Rum sequence
        DriveMotor(rum);
        valve1_on = 1;

        if (stepper.currentPosition() == rum && weight < 45 && valve1_on == 1) { // turn on the valve1
          digitalWrite(valve1, HIGH);
        }
        else if (stepper.currentPosition() == rum && weight > 45) { // turn off the valve1; go to the next drink
          digitalWrite(valve1, LOW);
          RESET();

          DriveMotor(coke);
          valve1_on = 1;
          if (stepper.currentPosition() == coke && weight < 45 && valve1_on == 1) { // turn on the valve1
            digitalWrite(valve1, HIGH);
          }
          else if (stepper.currentPosition() == coke && weight > 45) { // turn off the valve1; go back to origin
            digitalWrite(valve1, LOW);
            RESET();

            DriveMotor(origin);
            order = 0;

          }
          order = -1;
        }
      }

      if (digitalRead(endPin) == HIGH) { // indicating the origin
        //ENDSTOP();
      }
    }
  }

}
