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
const byte tT[8]= {100,0,0,0,0,0,0,0}; //period <1;256>xtic ms
/**************************************
 User global definitions:
     - constants
     - variables
*******D E C L A R A T I O N s*********/
#define ROW0 9
#define ROW1 10
#define ROW2 11

                //0 1 2
#define Col0 2  //a j s
#define Col1 3  //b k t
#define Col2 4  //c l u
#define Col3 5  //d m v
#define Col4 6  //e n w
#define Col5 7  //f o x
#define Col6 8  //g p y
#define Col7 12 //h q z
#define Col8 13 //i r

#define messageLen 4
                          //      A           H           O           J
int message[messageLen][2]={{ROW0,Col0},{ROW0,Col7},{ROW1,Col5},{ROW1,Col0}};

int couterTime = 0;
int i = 0;
bool connectionOK = false;
int dockNum = 0;
int stoneNum = 2;
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

/*------------------------------------*/
  TT.start(); } //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  if(connectionOK){
    if(couterTime==10){
      setRow(message[i][0]);
      offOn(message[i][1]);
    }else if(couterTime==20){
      offOn(message[i][1]);
      couterTime=0;
      i++;
      if(i > messageLen){
        i=0;
      }
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

void setRow(int pin){
  switch (pin) {
    case ROW0:
      digitalWrite(ROW0, LOW);    
      digitalWrite(ROW1, HIGH);
      digitalWrite(ROW2, HIGH);
      break;
    case ROW1:
      digitalWrite(ROW0, HIGH);    
      digitalWrite(ROW1, LOW);
      digitalWrite(ROW2, HIGH);
      break;
    case ROW2:
      digitalWrite(ROW0, HIGH);    
      digitalWrite(ROW1, HIGH);
      digitalWrite(ROW2, LOW);
      break;
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
