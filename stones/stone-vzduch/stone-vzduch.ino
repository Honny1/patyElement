/**************************************
 rtOS
***************************************
Name:     stone-vzduch
Version:  v1.0     
Libraries.........................
  rtOS.h
***************************************/
#include "rtOS.h"
#include <Wire.h>
/*------------------------------------*/
rtOS TT(1);    //construct rtOS tic(1ms)
/**************************************
     dT- delay start for task
     tT- period for schedule of task
***************************************/
const byte dT[8]= {1,10,0,0,0,0,0,0}; //start delay <1;255>xtic ms
const byte tT[8]= {16,123,0,0,0,0,0,0}; //period <1;256>xtic ms
/**************************************
 User global definitions:
     - constants
     - variables
*******D E C L A R A T I O N s*********/
int ROW_PINS[] = {2,4,5,6,7,8,9,12,13};
int COL_PINS[] = {3,10,11};
                   // ABC  DEF  GHI  JKL  MNO  PQR  STU  VWX  YZ
byte PATTERN[3][9]={{B110,B111,B111,B111,B101,B111,B011,B110,B111},
                   {B111,B111,B111,B111,B111,B111,B111,B111,B111},
                   {B000,B000,B000,B000,B000,B000,B000,B000,B000},
                   };

byte ANIMATION_PATTERN[9][9]={
                    {B000,B111,B111,B111,B111,B111,B111,B111,B000},
                    {B111,B000,B111,B111,B111,B111,B111,B000,B111},
                    {B111,B111,B000,B111,B111,B111,B000,B111,B111},
                    {B111,B111,B111,B000,B111,B000,B111,B111,B111},
                    {B111,B111,B111,B111,B000,B111,B111,B111,B111},
                    {B111,B111,B111,B000,B111,B000,B111,B111,B111},
                    {B111,B111,B000,B111,B111,B111,B000,B111,B111},
                    {B111,B000,B111,B111,B111,B111,B111,B000,B111},
                    {B000,B111,B111,B111,B111,B111,B111,B111,B000}
                    };
#define ROW_COUNT (sizeof(ROW_PINS)/sizeof(ROW_PINS[0]))
#define COL_COUNT (sizeof(COL_PINS)/sizeof(COL_PINS[0]))
#define ANIMATION_PATTERN_SIZE 9
#define ANIMATION_PATTERN_DELAY 65
unsigned long previousMillisAnimation = 0; 
int animationCount=0;
bool runAnimation = true;
bool repeteAnimation = true;


bool connectionOK = false;
bool lightOn = false;
bool showPass = false;
int data = 0;

int stoneNum = 3;
char lightOnCode='E';
char showPassCode='F';
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

/*-----------------------------------*/
void setup() {
/**************************************
     Setup for user Tasks
*************S E T U P****************/
Serial.begin(9600);
for (int thisPin = 2; thisPin < 14; thisPin++) {
    pinMode(thisPin, OUTPUT);
}
Wire.begin(); 
Serial.println("run");

/*------------------------------------*/
  TT.start(); } //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  if(connectionOK){
      connectionOK= true;
      //Serial.println("Good dock");
      if (lightOn){
        if(runAnimation){
          animation(ANIMATION_PATTERN_DELAY);
        }else{
          DispOut(PATTERN[1]);
          }
      }else if(showPass){
        DispOut(PATTERN[0]);
      }else{
        DispOut(PATTERN[2]);
      }
    }else{
      DispOut(PATTERN[2]);
      lightOn = false;
      showPass = false;
      }
} 

void task1() {
  Wire.requestFrom(8, 1);
  while (Wire.available()) { 
    comunicationI2C();
  }

}
void task2() {
}
void task3() {
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

void DispOut(byte currentPattern[]){
  int pattern_index =0;
  int last_row=ROW_COUNT-1;
  for (int row_index = 0; row_index < ROW_COUNT; last_row=row_index++){
    digitalWrite(ROW_PINS[row_index], HIGH);
    digitalWrite(ROW_PINS[last_row], LOW);
    byte pat = currentPattern[pattern_index];
    for (int col_index = COL_COUNT-1; col_index >= 0; col_index--, pat >>=1){
      digitalWrite(COL_PINS[col_index], pat & 1 ? LOW : HIGH);
      }
    delay(2);
    pattern_index++;
    }
}

void animation(int pD){
    if(runAnimation){
      DispOut(ANIMATION_PATTERN[animationCount]);
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillisAnimation >= pD) {
        previousMillisAnimation = currentMillis;
        animationCount++;
        if(repeteAnimation and animationCount==ANIMATION_PATTERN_SIZE+1){
          animationCount=0;
          repeteAnimation=false;
          }else{
            if(animationCount==ANIMATION_PATTERN_SIZE+1){
                animationCount=0;
                repeteAnimation=false;
                runAnimation = false;
          }
      }
   }
}
}

void comunicationI2C() {
  //Serial.print("connect: ");
  data = Wire.read();
  if(data==stoneNum){
      connectionOK= true;
      //Serial.println("Good dock");
    }else if(data == lightOnCode){
      //Serial.println("lightON");
      connectionOK= true;
      showPass = false;
      lightOn = true;
   }else if(data == showPassCode){
      //Serial.println("show pass");
      connectionOK= true;
      showPass = true;
      lightOn = false;
   }else{
      connectionOK = false;
      //Serial.println("bad dock");
    }
   data=0;
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
