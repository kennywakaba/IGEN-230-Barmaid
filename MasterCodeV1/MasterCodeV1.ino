#include "Motor_Functions.h"
#include "UI_Functions.h"
#include "LC_Functions.h"
#include "Valve_Functions.h"

void setup() {

  Serial.begin(9600);
  LCSetup();
  valveSetup();
  motorSetup();
  UISetup();


}

void loop() {

  menu();

}
