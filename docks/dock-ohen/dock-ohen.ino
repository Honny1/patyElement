/**************************************
 rtOS
***************************************
Name:     DOCK-ohen
Version:  v1.0     
Libraries.........................
  rtOS.h
***************************************/
#include "rtOS.h"
#include <Wire.h>
#include <Keypad.h>
#include <Servo.h>  
#include <OneWire.h>
#include <DallasTemperature.h>
/*------------------------------------*/
rtOS TT(1);    //construct rtOS tic(1ms)
/**************************************
     dT- delay start for task
     tT- period for schedule of task
***************************************/
const byte dT[8]= {1,2,3,4,0,0,0,0}; //start delay <1;255>xtic ms
const byte tT[8]= {50,1,50,100,0,0,0,0}; //period <1;256>xtic ms
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

const int pinCidlaDS = 4;
OneWire oneWireDS(pinCidlaDS);
DallasTemperature senzoryDS(&oneWireDS);

#define Password_Lenght 5
char Data[Password_Lenght]; 
char Master[Password_Lenght] = "5742"; 
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
byte rowPins[ROWS] = {3,8,7,5}; 
   //pins on keypad - 3, 1, 5
byte colPins[COLS] = {13, 2, 6};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char dataCode='2';
/*
 * DATA
 * voda - 1
 * lightOn - A 
 * show pass - B
 * 
 * ohen - 2
 * lightOn - C
 * show pass - D
 * 
 * vyduch - 3
 * lightOn - E
 * show pass - F
 * 
 * zeme - 4 
 * lightOn - G 
 * show pass - H
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
keyServo.attach(keyServoPin);
senzorServo.attach(senzorServoPin);
sevoOpen(0);
senzoryDS.begin();
/*------------------------------------*/
  TT.start(); } //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  if(openKey){
  customKey = customKeypad.getKey();
  if (customKey){
    Data[data_count] = customKey; 
    data_count++;
    if(customKey=='*'){
      clearData();   
      } 
  }

  if(data_count == Password_Lenght-1){
    if(!strcmp(Data, Master)){
     dataCode='C';
     openKey=false;
     openSenzor=true;
     sevoOpen(KEY); 
    }
    clearData();   
  }
  }
} 
void task1() {
  if(showPass){
    dataCode = 'D';
    }else{
      dataCode=dataCode;
      if(dataCode=='D'){
          dataCode='C';
        }
      }
}
void task2() {
  if (openSenzor){
        senzor=true;
        openSenzor=false; 
        sevoOpen(SENZOR);     
      }
    
}
void task3() {
  if (senzor){
    senzoryDS.requestTemperatures();
    if ((int)senzoryDS.getTempCByIndex(0) > 50) {
      showPass=true;
      }else{
        showPass=false;
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

void clearData(){
  while(data_count !=0){    
    Data[data_count--] = 0;
  }
  return;
}

void sevoOpen(int data){
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
void loop() {TT.loop();}
/**************************************
     ISR (service Timer2 tic)
***************************************/
ISR(TIMER2_OVF_vect){
   TCNT2 =TT.tcnt2;
   TT._overflow();}
