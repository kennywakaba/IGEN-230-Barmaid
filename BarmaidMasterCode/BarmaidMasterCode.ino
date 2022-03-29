#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

//motor variable initialization
const int stepsPerRev = 200;

long staging = 0;
long station1 = 400;
long station2 = 940;
long station3 = 1460;
long station4 = 1990;
long station5 = 2510;
long station6 = 2975;

long desiredPosition = 0;
long Position = 0;

const int dirPin = 6;
const int stepPin = 7;
const int sleepPin = 4;

//UI variable initialization
int count1 = 0;
int count2 = 0;

int pushR = 2;
int pushB = 3;
int pushY = 4;
int pushW = 5;

int internalcount = 0;

int runthrough = 0;

//motor configuration
AccelStepper stepper(1, stepPin, dirPin);
//lcd configuration
LiquidCrystal_I2C lcd(0x27, 20, 4);



//function prototypes
void driveMotor();
void upDown();
void selectDrink();
void menu();
//void introDisplay();
//void runningCount();

void setup() {

  //Motor setup
  stepper.setMaxSpeed(1400);
  stepper.setSpeed(1400);
  stepper.setAcceleration(1200);
  stepper.setCurrentPosition(0);
  stepper.disableOutputs();

  //UI setup
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(pushR, INPUT);
  pinMode(pushB, INPUT);
  pinMode(pushW, INPUT);
  pinMode(pushY, INPUT);

}

void loop() {

  menu();

}




//controls left hand side of UI box; allows for scrolling of drink options
void upDown() {

  //controls up button
  if (digitalRead(pushY) == HIGH) {

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



//controls right side of the UI box; allows for selection and cancelation of drinks
void drinkSelection() {

  int option;
  int selectionVar = 0;

  option = internalcount + 1;

  if (digitalRead(pushB) == HIGH) {
    lcd.clear();
    delay(500);

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

      if (selectionVar == 1) {


        if ( internalcount == 0) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station1);
          delay(2000);
          driveMotor(staging);
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 1) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station2);
          delay(1000);
          driveMotor(staging);
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 2) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station3);
          delay(2000);
          driveMotor(staging);
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 3) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station4);
          delay(2000);
          driveMotor(staging);
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);

        }

        else if (internalcount == 4) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station5);
          delay(2000);
          driveMotor(staging);
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("Drink Complete !");
          delay(3000);
        }

        else if (internalcount == 5) {
          lcd.setCursor(2, 1);
          lcd.print("Preparing Drink");
          driveMotor(station6);
          delay(2000);
          driveMotor(staging);
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



void menu() {
  if (runthrough == 0) {
    delay(700);
  }

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

  runthrough++;
}

void driveMotor(long Position) {

  stepper.enableOutputs();
  stepper.moveTo(Position);
  while (stepper.currentPosition() != Position) {
    stepper.run();
  }
  stepper.disableOutputs();
}

/*void introDisplay() {


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
*/



/*void runningCount() {

  int runningtime = 0;

  if (digitalRead(pushR) == HIGH || digitalRead(pushB) == HIGH || digitalRead(pushY) == HIGH || digitalRead(pushW) == HIGH) {
    noInterrupts ();
    timer0_millis = 0;
    interrupts ();
  }

  runningtime = millis();

  return runningtime;
  }
*/

//Current write function is broken and return value must be changed to 1 (previously was 0);
inline size_t LiquidCrystal_I2C::write(uint8_t value) {
  send(value, Rs);
  return 1;
}
