/**************************************
 rtOS APPLICATION MutiFunction Shield
***************************************
Name:     KostkaV2
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
const byte dT[8]= {1,0,0,0,0,0,0,0}; //start delay <1;255>xtic ms
const byte tT[8]= {50,0,0,0,0,0,0,0}; //period <1;256>xtic ms
/**************************************
 User global definitions:
     - constants
     - variables
*******D E C L A R A T I O N s*********/
#define COL0 9
#define COL1 10
#define COL2 11

                //0 1 2
#define Row0 2  //a j s
#define Row1 3  //b k t
#define Row2 4  //c l u
#define Row3 5  //d m v
#define Row4 6  //e n w
#define Row5 7  //f o x
#define Row6 8  //g p y
#define Row7 12 //h q z
#define Row8 13 //i r

#define messageLen 4
                            //      A           H           O           J
int message[messageLen][2]={{COL0,Row0},{COL0,Row7},{COL1,Row5},{COL1,Row0}};

#define messageLen1 27
int message1[messageLen1][2]={
  {COL0,Row0}, //a
  {COL0,Row1}, //b
  {COL0,Row2}, //c
  {COL0,Row3}, //d
  {COL0,Row4}, //e
  {COL0,Row5}, //f
  {COL0,Row6}, //g
  {COL0,Row7}, //h
  {COL0,Row8}, //i
  {COL1,Row0}, //j
  {COL1,Row1}, //k
  {COL1,Row2}, //l
  {COL1,Row3}, //m
  {COL1,Row4}, //n
  {COL1,Row5}, //o
  {COL1,Row6}, //p
  {COL1,Row7}, //q
  {COL1,Row8}, //r
  {COL2,Row0}, //s
  {COL2,Row1}, //t
  {COL2,Row2}, //u
  {COL2,Row3}, //v
  {COL2,Row4}, //w
  {COL2,Row5}, //x
  {COL2,Row6}, //y
  {COL2,Row7}, //z
  };

int ledsInCol[9]={Row0,Row1,Row2,Row3,Row4,Row5,Row6,Row7,Row8};

int couterTime = 0;
int i = 0;
bool connectionOK = false;
int dockNum = 0;
int stoneNum = 2;
int Mode=0;
bool reverseMode = false;

/*-----------------------------------*/
void setup() {
/**************************************
     Setup for user Tasks
*************S E T U P****************/
Serial.begin(9600);
for (int thisPin = 2; thisPin < 14; thisPin++) {
    pinMode(thisPin, OUTPUT);
}
Wire.begin(0x02); 
Wire.onReceive(comunicationI2C);
Serial.println("run");
//invertMode
off();
//on();
/*------------------------------------*/
  TT.start(); } //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  if(connectionOK){

    switch (Mode) {
        case 1:
          //Display alfabet
          if(couterTime==1){
            setCol(message1[i][0]);
            offOn(message1[i][1]);
          }else if(couterTime==10){
            offOn(message1[i][1]);
            couterTime=0;
            i++;
            if(i == messageLen1){
              i=0;
              Mode=2;
            }
          }
          break;
        case 2:
          //Display message
          if(couterTime==20){
            setCol(message[i][0]);
            offOn(message[i][1]);
         }else if(couterTime==40){
            offOn(message[i][1]);
            couterTime=0;
            i++;
            if(i == messageLen){
              i=0; 
            }
          }  
          break;
        default:
          //animation
          if(couterTime==0){
            setCol(3);
            offOn(ledsInCol[i]);
          }else if(couterTime==1){
            offOn(ledsInCol[i]);
            couterTime=0;
            
            if(reverseMode){
                i--;
            }else{
                i++;
            }
            
            if(i == 9){
              reverseMode = true;
            }
            
            if(i==0){
              Mode=1;
              reverseMode = false;
            }
          }
          break;
    }
    couterTime++;
  } 
} 
void task1() {
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

void setCol(int pin){
  switch (pin) {
    case COL0:
      digitalWrite(COL0, LOW);    
      digitalWrite(COL1, HIGH);
      digitalWrite(COL2, HIGH);
      break;
    case COL1:
      digitalWrite(COL0, HIGH);    
      digitalWrite(COL1, LOW);
      digitalWrite(COL2, HIGH);
      break;
    case COL2:
      digitalWrite(COL0, HIGH);    
      digitalWrite(COL1, HIGH);
      digitalWrite(COL2, LOW);
      break;
    default:
      digitalWrite(COL0, LOW);    
      digitalWrite(COL1, LOW);
      digitalWrite(COL2, LOW);
      break;
    
  }
}

void off(){
  setCol(3);
  for (int thisPin = 0; thisPin < 9; thisPin++) {
    digitalWrite(ledsInCol[thisPin], LOW);
  }  
}


void on(){
  setCol(3);
  for (int thisPin = 0; thisPin < 9; thisPin++) {
    digitalWrite(ledsInCol[thisPin], HIGH);
  }  
}
  
void offOn(int pin){
  digitalWrite(pin, !digitalRead(pin));
}

void comunicationI2C(int len) {
  Serial.print("connect: ");
  dockNum = Wire.read();
  if(dockNum==stoneNum){
      connectionOK= true;
      Serial.println("Good dock");
    }else{
      Serial.println("Bad dock");
      connectionOK=false;
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
