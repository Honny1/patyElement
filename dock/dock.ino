/**************************************
  rtOS APPLICATION MutiFunction Shield
***************************************
  Name:     DOCK
  Version:  v1.0
  Libraries.........................
  rtOS.h
***************************************/
#include "rtOS.h"
#include <Wire.h>
#include <Keypad.h>
#include <Servo.h>
#include <HMC5983.h>
/*------------------------------------*/
rtOS TT(1);    //construct rtOS tic(1ms)
/**************************************
     dT- delay start for task
     tT- period for schedule of task
***************************************/
const byte dT[8] = {1, 2, 3, 4, 0, 0, 0, 0}; //start delay <1;255>xtic ms
const byte tT[8] = {50, 1, 50, 100, 0, 0, 0, 0}; //period <1;256>xtic ms
/**************************************
  User global definitions:
     - constants
     - variables
*******D E C L A R A T I O N s*********/
#define KEY 1
#define SENZOR 2

#define keyServoPin 9
#define senzorServoPin 10

Servo keyServo;
Servo senzorServo;

HMC5983 compass;

#define Password_Lenght 5
char Data[Password_Lenght];
char Master[Password_Lenght] = "1234";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

/*
   UP - 1
   DOWN - 2
   LEFT - 3
   RIGHT - 4
*/
#define buttonUP 12
#define buttonDOWN 13
#define buttonLEFT A1
#define buttonRIGHT A2

#define Button_Password_Lenght 5
char Button_Data[Button_Password_Lenght];
char Button_Master[Button_Password_Lenght] = "1432";
byte button_data_count = 0, button_master_count = 0;
bool Button_Pass_is_good;
char buttonCustomKey;


const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


//pins on keypad - 2,7,6,4
byte rowPins[ROWS] = {3, 8, 7, 5};
//pins on keypad - 3, 1, 5
byte colPins[COLS] = {4, 2, 6};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char dataCode = '4';
/*
   DATA
   voda - 1
   lightOn - A
   show pass - B

   ohen - 2
   lightOn - C
   show pass - D

   vyduch - 3
   lightOn - E
   show pass - F

   zeme - 4
   lightOn - G
   show pass - H
*/

bool showPass = false;
bool openSenzor = false;
bool openKey = true;
bool senzor = false;
/*-----------------------------------*/
void setup() {
  /**************************************
       Setup for user Tasks
  *************S E T U P****************/
  Serial.begin(9600);

  Serial.println("run");
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  pinMode(buttonUP, INPUT);
  pinMode(buttonDOWN, INPUT);
  pinMode(buttonRIGHT, INPUT);
  pinMode(buttonLEFT, INPUT);
  keyServo.attach(keyServoPin);
  senzorServo.attach(senzorServoPin);
  sevoOpen(0);
  compass.begin();
  /*------------------------------------*/
  TT.start();
} //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  if (openKey) {
    customKey = customKeypad.getKey();
    if (customKey) {
      Data[data_count] = customKey;
      data_count++;
      if (customKey == '*') {
        clearData();
      }
    }

    if (data_count == Password_Lenght - 1) {
      if (!strcmp(Data, Master)) {
        dataCode = 'G';
        openKey = false;
        openSenzor = true;
        sevoOpen(KEY);
      }
      clearData();
    }
  }
}
void task1() {
  if (showPass) {
    dataCode = 'H';
  } else {
    dataCode = dataCode;
    if (dataCode == 'H') {
      dataCode = 'G';
    }
  }
}
void task2() {
  // RESET input - doresit
  customKey = customKeypad.getKey();
  if (customKey == '*') {
    clearData();
    buttonClearData();
  }
  if (openSenzor) {
    buttonCustomKey = readButtons();
    if (buttonCustomKey != '9') {
      Button_Data[button_data_count] = buttonCustomKey;
      button_data_count++;
    }
    if (button_data_count == Button_Password_Lenght - 1) {
      if (!strcmp(Button_Data, Button_Master)) {
        senzor = true;
        openSenzor = false;
        sevoOpen(SENZOR);
      }
      buttonClearData();
    }
  }
}
void task3() {
  if (senzor) {
    int c = -999;
    c = compass.read();
    if (c > 90 and c < 120) {
      showPass = true;
    } else {
      showPass = false;
    }
  }
}
void task4() {
}
void task5() {
}
void task6() {
}
void task7() {
}
/*************************************
******User S U B R O U T I N E s******/
void requestEvent() {
  Wire.write(dataCode);
}

void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}

void buttonClearData() {
  while (button_data_count != 0) {
    Button_Data[button_data_count--] = 0;
  }
  return;
}

char readButtons() {
  if (digitalRead(buttonUP) == HIGH) {
    return '1';
  } else if (digitalRead(buttonDOWN) == HIGH) {
    return '2';
  } else if (digitalRead(buttonRIGHT) == HIGH) {
    return '4';
  } else if (digitalRead(buttonLEFT) == HIGH) {
    return '3';
  } else {
    return '9';
  }
}


void sevoOpen(int data) {
  switch (data) {
    case 0:
      keyServo.write(0);
      senzorServo.write(0);
      break;
    case 1:
      keyServo.write(180);
      break;
    case 2:
      senzorServo.write(180);
      break;
  }
}
/*************************************
     SUPER LOOP (dispatcher)
****   never modify(!!!)   ***********/
void loop() {
  TT.loop();
}
/**************************************
     ISR (service Timer2 tic)
***************************************/
ISR(TIMER2_OVF_vect) {
  TCNT2 = TT.tcnt2;
  TT._overflow();
}
