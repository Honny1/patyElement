/**************************************
 rtOS APPLICATION MutiFunction Shield
***************************************
Name:     rtOS_core
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
int dockNum=2;



/*-----------------------------------*/
void setup() {
/**************************************
     Setup for user Tasks
*************S E T U P****************/
Serial.begin(9600);
Wire.begin(0x01);

Serial.println("run");

/*------------------------------------*/
  TT.start(); } //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
   Serial.print("send: ");
   Serial.println(dockNum);
   Wire.beginTransmission(0x02);
   Wire.write(dockNum);
   Wire.endTransmission();
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
