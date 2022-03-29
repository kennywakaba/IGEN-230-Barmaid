#include "UI_Functions.h"
#include "Motor_Functions.h"
#include "Valve_Functions.h"
#include "LC_Functions.h"

//UI variable initialization
int count1 = 0;
int count2 = 0;

const int valve1 = 12;
const int valve2 = 13;
const int valve3 = A3;
const int valve4 = A2;
const int valve5 = A1;
const int valve6 = A0;

const int pushR = 11;
const int pushB = 10;
const int pushW = 9;
const int pushY = 8;

long staging = 0;
long station1 = 400;
long station2 = 940;
long station3 = 1460;
long station4 = 1990;
long station5 = 2510;
long station6 = 2975;

int internalcount = 0;

float weight = 0;
float weight1;

//lcd configuration
LiquidCrystal_I2C lcd(0x27, 20, 4);

void UISetup() {
  lcd.init();
  lcd.backlight();

  pinMode(pushR, INPUT);
  pinMode(pushB, INPUT);
  pinMode(pushW, INPUT);
  pinMode(pushY, INPUT);
}

void upDown() {
  //controls up button
  if (digitalRead(pushY) == HIGH) {
    //ensures that screen doesn't scroll when cursor reaches last
    if (internalcount >= 5) {
      internalcount == 5;
      delay(100);
    }

    else {
      internalcount++;
      delay(100);
    }

  }

  //controls down button
  else if (digitalRead(pushW) == HIGH) {
    //ensures that screen doesn't scroll when cursor reaches first option
    if (internalcount <= 0 ) {
      internalcount == 0;
      delay(100);
    }

    else {
      internalcount--;
      delay(100);
    }

  }
}


void introDisplay() {


  lcd.setCursor(count1, count2);
  lcd.print("Barmaid");
  delay(800);
  lcd.clear();

  count1++;

  if (count1 == 14) {
    count1 = 0;

    if (count2 == 3) {
      count2 = 0;
    }

    count2++;
  }

}

//controls right side of the UI box; allows for selection and cancelation of drinks
void drinkSelection() {

  int option;
  int selectionVar = 0;

  option = internalcount + 1;

  if (digitalRead(pushB) == HIGH) {
    lcd.clear();
    delay(500);

    //prints "Select Option x?"
    while (selectionVar == 0) {
      lcd.setCursor(2, 1);
      lcd.print("Select Option ");
      lcd.print(option);
      lcd.print("?");

      if (digitalRead(pushB) == HIGH) {
        selectionVar = 1;
        lcd.clear();
      }

      else if (digitalRead(pushR) == HIGH) {
        selectionVar = 2;
        lcd.clear();
      }

      //Preparation of drink
      if (selectionVar == 1) {


        if ( internalcount == 0) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");

          driveMotor(station1);

          digitalWrite(valve1, LOW);

          callLC();

          digitalWrite(valve1, HIGH);
          delay(1000);

          driveMotor(staging);

          resetLC();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 1) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");

          driveMotor(station2);

          digitalWrite(valve2, LOW);

          callLC();

          digitalWrite(valve2, HIGH);
          delay(1000);

          driveMotor(staging);

          resetLC();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 2) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station3);

          digitalWrite(valve3, LOW);

          callLC();

          digitalWrite(valve3, HIGH);
          delay(1000);

          driveMotor(staging);

          resetLC();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 3) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station4);

          digitalWrite(valve4, LOW);

          callLC();
          
          digitalWrite(valve4, HIGH);
          delay(1000);

          driveMotor(staging);

          resetLC();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);

        }

        else if (internalcount == 4) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");

          driveMotor(station5);

          digitalWrite(valve5, LOW);

          callLC();

          digitalWrite(valve5, HIGH);
          delay(1000);

          driveMotor(staging);

          resetLC();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 5) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");

          driveMotor(station6);

          digitalWrite(valve6, LOW);

          callLC();

          digitalWrite(valve6, HIGH);
          delay(1000);

          driveMotor(staging);

          resetLC();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        selectionVar = 2;
        lcd.clear();

      }

    }

  }
}
//Toggles through the menu and allows for the preparation of drinks
void menu() {

  while (internalcount == 0) {
    lcd.setCursor(8, 0);
    lcd.print("MENU");
    lcd.setCursor(0, 1);
    lcd.print("1. Option 1    <");
    lcd.setCursor(0, 2);
    lcd.print("2. Option 2");
    lcd.setCursor(0, 3);
    lcd.print("3. Option 3");

    drinkSelection();
    upDown();
  }

  lcd.clear();

  while (internalcount == 1) {
    lcd.setCursor(8, 0);
    lcd.print("MENU");
    lcd.setCursor(0, 1);
    lcd.print("1. Option 1");
    lcd.setCursor(0, 2);
    lcd.print("2. Option 2    <");
    lcd.setCursor(0, 3);
    lcd.print("3. Option 3");

    drinkSelection();
    upDown();
  }

  lcd.clear();

  while (internalcount == 2) {
    lcd.setCursor(8, 0);
    lcd.print("MENU");
    lcd.setCursor(0, 1);
    lcd.print("1. Option 1");
    lcd.setCursor(0, 2);
    lcd.print("2. Option 2");
    lcd.setCursor(0, 3);
    lcd.print("3. Option 3    <");

    drinkSelection();
    upDown();
  }

  lcd.clear();

  while (internalcount == 3) {
    lcd.setCursor(0, 0);
    lcd.print("1. Option 1");
    lcd.setCursor(0, 1);
    lcd.print("2. Option 2");
    lcd.setCursor(0, 2);
    lcd.print("3. Option 3");
    lcd.setCursor(0, 3);
    lcd.print("4. Option 4    <");

    drinkSelection();
    upDown();
  }

  lcd.clear();

  while (internalcount == 4) {
    lcd.setCursor(0, 0);
    lcd.print("2. Option 2");
    lcd.setCursor(0, 1);
    lcd.print("3. Option 3");
    lcd.setCursor(0, 2);
    lcd.print("4. Option 4");
    lcd.setCursor(0, 3);
    lcd.print("5. Option 5    <");

    drinkSelection();
    upDown();
  }

  lcd.clear();

  while (internalcount == 5) {
    lcd.setCursor(0, 0);
    lcd.print("3. Option 3");
    lcd.setCursor(0, 1);
    lcd.print("4. Option 4");
    lcd.setCursor(0, 2);
    lcd.print("5. Option 5");
    lcd.setCursor(0, 3);
    lcd.print("6. Option 6    <");

    drinkSelection();
    upDown();
  }

}






//Current write function is broken and return value must be changed to 1 (previously was 0);
inline size_t LiquidCrystal_I2C::write(uint8_t value) {
  send(value, Rs);
  return 1;
}
