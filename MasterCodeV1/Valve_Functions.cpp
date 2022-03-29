#include "Valve_Functions.h"

//variable initialization
const int RST = 2;

const int valve1 = 12;
const int valve2 = 13;
const int valve3 = A3;
const int valve4 = A2;
const int valve5 = A1;
const int valve6 = A0;

//Functions
void valveSetup() {
  
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  pinMode(valve4, OUTPUT);
  pinMode(valve5, OUTPUT);
  pinMode(valve6, OUTPUT);
  pinMode(RST, INPUT);
  digitalWrite(valve1, HIGH);
  digitalWrite(valve2, HIGH);
  digitalWrite(valve3, HIGH);
  digitalWrite(valve4, HIGH);
  digitalWrite(valve5, HIGH);
  digitalWrite(valve6, HIGH);
  
}

void valveOperation(int valve){

  digitalWrite(valve, LOW);
  delay(2000);
  digitalWrite(valve, HIGH);
  
}
