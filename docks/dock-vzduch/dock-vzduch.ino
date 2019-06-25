/**************************************
 rtOS 
***************************************
Name:     DOCK-vzduch
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

#define pinPrutokomer 2
#define pinPreruseni 0
volatile byte pocetPulzu = 0;

#define pinCLK 8
#define pinDT  9
#define pinSW  10
float poziceEnkod = 0;
int stavPred;
int stavCLK;
int stavSW;
String pass = "";
int lenPass=0;
int presed=0;

const int buzzer = 5;

#define Password_Lenght 5
char Data[Password_Lenght]; 
char Master[Password_Lenght] = "2348"; 
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
byte rowPins[ROWS] = {3,8,7,11}; 
   //pins on keypad - 3, 1, 5
byte colPins[COLS] = {4, 12, 6};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char dataCode='3';
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
int counterTone=0;
int counterTone1=0;
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
pinMode(pinPrutokomer, INPUT);
attachInterrupt(pinPreruseni, prictiPulz, FALLING);
pinMode(pinCLK, INPUT);
pinMode(pinDT, INPUT);
pinMode(pinSW, INPUT_PULLUP);
pinMode(buzzer, OUTPUT); 
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
     dataCode='E';
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
    dataCode = 'F';
    }else{
      dataCode=dataCode;
      if(dataCode=='F'){
          dataCode='E';
        }
      }
}
void task2() {
  if (openSenzor){
    stavCLK = digitalRead(pinCLK);
    if (stavCLK != stavPred) {
      if (digitalRead(pinDT) != stavCLK) {
        poziceEnkod=poziceEnkod+0.5;
        counterTone++;
        if (counterTone==1){
          tone(buzzer, 1000); 
          delay(500);       
          noTone(buzzer);
          counterTone=0; 
          }
        if(poziceEnkod>=9){poziceEnkod=0;}
      }else{
        poziceEnkod=poziceEnkod-0.5;
        counterTone1++;
        if (counterTone1==1){
          tone(buzzer, 500); 
          delay(500);       
          noTone(buzzer);
          counterTone1=0; 
          }
        if(poziceEnkod<0){poziceEnkod=9;}
      }
    }
    stavPred = stavCLK;
    stavSW = digitalRead(pinSW);
    if (stavSW == 0) {
      if(presed>20){
        pass=pass + (String)(int)poziceEnkod;
        poziceEnkod=0;
        lenPass++;
        presed=0;
        }
      presed++;
      }
      
    customKey = customKeypad.getKey();
    if(customKey=='*'){
      lenPass=0;
      pass=""; 
      } 
      
    if(lenPass>4){
      if (pass=="5703"){
          pass="";
          senzor=true;
          openSenzor=false; 
          sevoOpen(SENZOR);
          }else{
            tone(buzzer, 1500); 
            delay(500);       
            noTone(buzzer); 
            lenPass=0;
            pass="";
          }        
      }
    }
}
void task3() {
  if (senzor){
    detachInterrupt(pinPreruseni);
    if (pocetPulzu > 10) {
      showPass=true;
      }else{
        showPass=false;
        }
    pocetPulzu = 0;
    attachInterrupt(pinPreruseni, prictiPulz, FALLING);
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

void prictiPulz() {
  pocetPulzu++;
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
