#include "LC_Functions.h"

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
const int RST = 2;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void LCSetup() {
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
}

void callLC() {

    float weight = 0;
    static boolean newDataReady = 0;
    const int serialPrintInterval = 0; //increase value to slow down serial print activity


    while (weight < 10){

      if (LoadCell.update()) {
      newDataReady = true;
    }
    
    if (newDataReady) {
      if (millis() > t + serialPrintInterval) {
        weight = LoadCell.getData();
        Serial.print("Load_cell output val: ");
        Serial.print(weight);
        Serial.println();
        newDataReady = 0;
        t = millis();
      }
    }
  }
}

void resetLC() {
  
  Serial.println("Reset");
  LoadCell.start(2000, true);
  
}
