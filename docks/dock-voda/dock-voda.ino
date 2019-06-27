/**************************************
  rtOS
***************************************
  Name:     DOCK-voda
  Version:  v1.0
  Libraries.........................
  rtOS.h
***************************************/
#include "rtOS.h"
#include <Wire.h>
#include <Keypad.h>
#include <Servo.h>
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

#define keyServoPin 11
#define senzorServoPin 10

Servo keyServo;
Servo senzorServo;

#define pinAnalog A0
#define pinNapajeni A1

#define Password_Lenght 5
char Data[Password_Lenght];
char Master[Password_Lenght] = "3832";
char Master1[Password_Lenght] = "8756";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


//pins on keypad - 2,7,6,4
byte rowPins[ROWS] = {4, 9, 8, 6};
//pins on keypad - 3, 1, 5
byte colPins[COLS] = {5, 3, 7};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char dataCode = '1';
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
  keyServo.attach(keyServoPin);
  senzorServo.attach(senzorServoPin);
  sevoOpen(0);
  pinMode(pinNapajeni, OUTPUT);
  pinMode(pinAnalog, INPUT);
  /*------------------------------------*/
  TT.start();
} //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  customKey = customKeypad.getKey();
  if (customKey) {
    Data[data_count] = customKey;
    data_count++;
    if (customKey == '*') {
      clearData();
    }
  }
  if (data_count == Password_Lenght - 1) {
    if (openSenzor) {
      if (!strcmp(Data, Master1)) {
        senzor = true;
        openSenzor = false;
        sevoOpen(SENZOR);
      }
      clearData();
    } else {
      if (!strcmp(Data, Master)) {
        dataCode = 'A';
        openSenzor = true;
        sevoOpen(KEY);
      }
      clearData();
    }
  }

}
void task1() {
  if (showPass) {
    dataCode = 'B';
  } else {
    dataCode = dataCode;
    if (dataCode == 'B') {
      dataCode = 'A';
    }
  }
}
void task2() {
}
void task3() {
  if (senzor) {
    digitalWrite(pinNapajeni, HIGH);
    int analog = analogRead(pinAnalog);
    digitalWrite(pinNapajeni, LOW);
    if (analog > 512) {
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

void sevoOpen(int data) {
  switch (data) {
    case 0:
      keyServo.write(180);
      senzorServo.write(180);
      break;
    case 1:
      keyServo.write(0);
      break;
    case 2:
      senzorServo.write(0);
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
