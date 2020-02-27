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
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
/*------------------------------------*/
rtOS TT(1);    //construct rtOS tic(1ms)
/**************************************
     dT- delay start for task
     tT- period for schedule of task
***************************************/
const byte dT[8] = {1, 2, 3, 4, 5, 6, 0, 0}; //start delay <1;255>xtic ms
const byte tT[8] = {10, 10, 10, 10, 30, 0, 0, 0}; //period <1;256>xtic ms
/**************************************
  User global definitions:
     - constants
     - variables
*******D E C L A R A T I O N s*********/
LiquidCrystal_I2C lcd(0x27, 20, 4);

/*
 * voda - DQST - I
 * ohen - DGLX - C
 * vzduch - AELW - H
 * zeme - HLMS - F
 */


#define pinCLK 5
#define pinDT  4
#define pinSW  3
int poziceEnkod = 65;
int stavPred;
int stavCLK;
int stavSW;
String pass = "";
int lenPass = 0;
bool wait = false;
String PASSWORD = "DQST";
String aPASSWORD = "DQSTI";
bool showINPUT = true;
int pressed = 0;

#define pinCLK1 13
#define pinDT1  12
#define pinSW1  A0
int poziceEnkod1 = 65;
int stavPred1;
int stavCLK1;
int stavSW1;
String pass1 = "";
int lenPass1 = 0;
bool wait1 = false;
String PASSWORD1 = "DGIX";
String aPASSWORD1 = "DGLXC";
bool showINPUT1 = true;
int pressed1 = 0;

#define pinCLK2 A3
#define pinDT2  A2
#define pinSW2  A1
int poziceEnkod2 = 65;
int stavPred2;
int stavCLK2;
int stavSW2;
String pass2 = "";
int lenPass2 = 0;
bool wait2 = false;
String PASSWORD2 = "AELW";
String aPASSWORD2 = "AELWH";
bool showINPUT2 = true;
int pressed2 = 0;

#define pinCLK3 7
#define pinDT3  8
#define pinSW3  6
int poziceEnkod3 = 65;
int stavPred3;
int stavCLK3;
int stavSW3;
String pass3 = "";
int lenPass3 = 0;
bool wait3 = false;
String PASSWORD3 = "HLMS";
String aPASSWORD3 = "HLMSF";
bool showINPUT3 = true;
int pressed3 = 0;

bool openSERVO = false;
bool openSERVO1 = false;
bool openSERVO2 = false;
bool openSERVO3 = false;


bool open1SERVO = false;
bool open1SERVO1 = false;
bool open1SERVO2 = false;
bool open1SERVO3 = false;

#define KEY 1
#define SENZOR 2

#define keyServoPin 11
#define senzorServoPin 10

Servo keyServo;
Servo senzorServo;

#define LED 9
int brightness = 0;
int fadeAmount = 5; 
bool lightOn = false;
/*-----------------------------------*/
void setup() {
  /**************************************
       Setup for user Tasks
  *************S E T U P****************/
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor ( 4, 0 );
  lcd.print("|");
  lcd.setCursor ( 4, 1 );
  lcd.print("|");
  lcd.setCursor ( 4, 2 );
  lcd.print("|");
  lcd.setCursor ( 4, 3 );
  lcd.print("|");

  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  poziceEnkod = random(65, 90);


  pinMode(pinCLK1, INPUT);
  pinMode(pinDT1, INPUT);
  pinMode(pinSW1, INPUT_PULLUP);
  poziceEnkod1 = random(65, 90);



  pinMode(pinCLK2, INPUT);
  pinMode(pinDT2, INPUT);
  pinMode(pinSW2, INPUT_PULLUP);
  poziceEnkod2 = random(65, 90);


  pinMode(pinCLK3, INPUT);
  pinMode(pinDT3, INPUT);
  pinMode(pinSW3, INPUT_PULLUP);
  poziceEnkod3 = random(65, 90);

  pinMode(LED, OUTPUT);
  keyServo.attach(keyServoPin);
  senzorServo.attach(senzorServoPin);
  sevoOpen(0);
  /*------------------------------------*/
  TT.start();
} //start rtOS
/*   PROCEDURES SPACE
     task0 ... task7
********U S E R   C O D E**************/
void task0() {
  stavCLK = digitalRead(pinCLK);
  if (stavCLK != stavPred) {
    if (digitalRead(pinDT) != stavCLK) {
      poziceEnkod++;
      if (poziceEnkod > 90) {
        poziceEnkod = 65;
      }
    } else {
      poziceEnkod--;
      if (poziceEnkod < 65) {
        poziceEnkod = 90;
      }
    }
  }
  stavPred = stavCLK;
  stavSW = digitalRead(pinSW);
  if (stavSW == 0) {
    wait = true;
    pressed++;
  }
  if (wait and pressed > 100) {
    pass = "";
    poziceEnkod = 65;
    lenPass = 0;
    wait = false;
    lcd.setCursor ( 0, 0 );
    lcd.print("    |");
    pressed = 0;
  } else if (wait and stavSW == 1) {
    pass = pass + (String)(char)poziceEnkod;
    poziceEnkod = 65;
    lenPass++;
    wait = false;
    pressed = 0;
  }

  
  if (showINPUT) {
    lcd.setCursor(0, 0);
    lcd.print(pass);
    lcd.print((char)poziceEnkod);
  }
  if (openSERVO) {
    if (lenPass > 4) {
      Serial.println(pass);
      if (pass == aPASSWORD) {
        lcd.setCursor ( 0, 0 );
        lcd.print("                    ");
        pass = "";
        lenPass = 0;
        showINPUT = false;
        open1SERVO = true;
      } else {
        lenPass = 0;
        pass = "";
        lcd.setCursor ( 0, 0 );
        lcd.print("     |");
      }
    }
  } else {
    if (lenPass > 3) {
      Serial.println(pass);
      if (pass == PASSWORD) {
        lcd.setCursor ( 18, 0 );
        lcd.print("ok");
        lcd.setCursor ( 0, 0 );
        lcd.print("     |");
        lenPass = 0;
        openSERVO = true;
        pass = "";
      } else {
        lenPass = 0;
        pass = "";
        lcd.setCursor ( 0, 0 );
        lcd.print("    |");
      }
    }
  }

}
void task1() {
  stavCLK1 = digitalRead(pinCLK1);
  if (stavCLK1 != stavPred1) {
    if (digitalRead(pinDT1) != stavCLK1) {
      poziceEnkod1++;
      if (poziceEnkod1 > 90) {
        poziceEnkod1 = 65;
      }
    } else {
      poziceEnkod1--;
      if (poziceEnkod1 < 65) {
        poziceEnkod1 = 90;
      }
    }
  }
  stavPred1 = stavCLK1;

  stavSW1 = digitalRead(pinSW1);
  if (stavSW1 == 0) {
    wait1 = true;
    pressed1++;
  }
  if (wait1 and pressed1 > 100) {
    pass1 = "";
    poziceEnkod1 = 65;
    lenPass1 = 0;
    wait1 = false;
    lcd.setCursor ( 0, 1 );
    lcd.print("    |");
    pressed1 = 0;
  } else if (wait1 and stavSW1 == 1) {
    pass1 = pass1 + (String)(char)poziceEnkod1;
    poziceEnkod1 = 65;
    lenPass1++;
    wait1 = false;
    pressed1 = 0;
  }
  if (showINPUT1) {
    lcd.setCursor(0, 1);
    lcd.print(pass1);
    lcd.print((char)poziceEnkod1);
  }
  if (openSERVO1) {
    if (lenPass1 > 4) {
      Serial.println(pass1);
      if (pass1 == aPASSWORD1) {
        lcd.setCursor ( 0, 1 );
        lcd.print("                    ");
        pass1 = "";
        lenPass1 = 0;
        showINPUT1 = false;
        open1SERVO1 = true;
      } else {
        lenPass1 = 0;
        pass1 = "";
        lcd.setCursor ( 0, 1 );
        lcd.print("     |");
      }
    }
  } else {
    if (lenPass1 > 3) {
      Serial.println(pass1);
      if (pass1 == PASSWORD1) {
        lcd.setCursor ( 18, 1 );
        lcd.print("ok");
        lcd.setCursor ( 0, 1 );
        lcd.print("     |");
        lenPass1 = 0;
        openSERVO1 = true;
        pass1 = "";
      } else {
        lenPass1 = 0;
        pass1 = "";
        lcd.setCursor ( 0, 1 );
        lcd.print("    |");
      }
    }
  }

}
void task2() {
  stavCLK2 = digitalRead(pinCLK2);
  if (stavCLK2 != stavPred2) {
    if (digitalRead(pinDT2) != stavCLK2) {
      poziceEnkod2++;
      if (poziceEnkod2 > 90) {
        poziceEnkod2 = 65;
      }
    } else {
      poziceEnkod2--;
      if (poziceEnkod2 < 65) {
        poziceEnkod2 = 90;
      }
    }
  }
  stavPred2 = stavCLK2;

  stavSW2 = digitalRead(pinSW2);
  if (stavSW2 == 0) {
    wait2 = true;
    pressed2++;
  }
  if (wait2 and pressed2 > 100) {
    pass2 = "";
    poziceEnkod2 = 65;
    lenPass2 = 0;
    wait2 = false;
    lcd.setCursor ( 0, 2 );
    lcd.print("    |");
    pressed2 = 0;
  } else if (wait2 and stavSW2 == 1) {
    pass2 = pass2 + (String)(char)poziceEnkod2;
    poziceEnkod2 = 65;
    lenPass2++;
    wait2 = false;
    pressed2 = 0;
  }
  
  if (showINPUT2) {
    lcd.setCursor(0, 2);
    lcd.print(pass2);
    lcd.print((char)poziceEnkod2);
  }
  if (openSERVO2) {
    if (lenPass2 > 4) {
      Serial.println(pass2);
      if (pass2 == aPASSWORD2) {
        lcd.setCursor ( 0, 2 );
        lcd.print("                    ");
        pass2 = "";
        lenPass2 = 0;
        showINPUT2 = false;
        open1SERVO2 = true;
      } else {
        lenPass2 = 0;
        pass2 = "";
        lcd.setCursor ( 0, 2 );
        lcd.print("     |");
      }
    }
  } else {
    if (lenPass2 > 3) {
      Serial.println(pass2);
      if (pass2 == PASSWORD2) {
        lcd.setCursor ( 18, 2 );
        lcd.print("ok");
        lcd.setCursor ( 0, 2 );
        lcd.print("     |");
        lenPass2 = 0;
        openSERVO2 = true;
        pass2 = "";
      } else {
        lenPass2 = 0;
        pass2 = "";
        lcd.setCursor ( 0, 2 );
        lcd.print("    |");
      }
    }
  }

}
void task3() {
  stavCLK3 = digitalRead(pinCLK3);
  if (stavCLK3 != stavPred3) {
    if (digitalRead(pinDT3) != stavCLK3) {
      poziceEnkod3++;
      if (poziceEnkod3 > 90) {
        poziceEnkod3 = 65;
      }
    } else {
      poziceEnkod3--;
      if (poziceEnkod3 < 65) {
        poziceEnkod3 = 90;
      }
    }
  }
  stavPred3 = stavCLK3;

  stavSW3 = digitalRead(pinSW3);
  if (stavSW3 == 0) {
    wait3 = true;
    pressed3++;
  }
  if (wait3 and pressed3 > 100) {
    pass3 = "";
    poziceEnkod3 = 65;
    lenPass3 = 0;
    wait3 = false;
    lcd.setCursor ( 0, 3 );
    lcd.print("    |");
    pressed3 = 0;
  } else if (wait3 and stavSW3 == 1) {
    pass3 = pass3 + (String)(char)poziceEnkod3;
    poziceEnkod3 = 65;
    lenPass3++;
    wait3 = false;
    pressed3 = 0;
  }
  
  if (showINPUT3) {
    lcd.setCursor(0, 3);
    lcd.print(pass3);
    lcd.print((char)poziceEnkod3);
  }
  if (openSERVO3) {
    if (lenPass3 > 4) {
      Serial.println(pass3);
      if (pass3 == aPASSWORD3) {
        lcd.setCursor ( 0, 3 );
        lcd.print("                    ");
        pass3 = "";
        lenPass3 = 0;
        showINPUT3 = false;
        open1SERVO3 = true;
      } else {
        lenPass3 = 0;
        pass3 = "";
        lcd.setCursor ( 0, 3 );
        lcd.print("     |");
      }
    }
  } else {
    if (lenPass3 > 3) {
      Serial.println(pass3);
      if (pass3 == PASSWORD3) {
        lcd.setCursor ( 18, 3 );
        lcd.print("ok");
        lcd.setCursor ( 0, 3 );
        lcd.print("     |");
        lenPass3 = 0;
        openSERVO3 = true;
        pass3 = "";
      } else {
        lenPass3 = 0;
        pass3 = "";
        lcd.setCursor ( 0, 3 );
        lcd.print("    |");
      }
    }
  }

}
void task4() {
  if (openSERVO and openSERVO1 and openSERVO2 and openSERVO3) {
    sevoOpen(KEY);
    lightOn=true;
  }  
  if (open1SERVO and open1SERVO1 and open1SERVO2 and open1SERVO3) {
    sevoOpen(SENZOR);
  }
}
void task5() {
  if (lightOn){
    analogWrite(LED, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
      }
    }else{
      analogWrite(LED, 0);
    }
}
void task6() {
}
void task7() {
}
/*************************************
******User S U B R O U T I N E s******/
void sevoOpen(int data) {
  switch (data) {
    case 0:
      keyServo.write(130);
      senzorServo.write(170);
      break;
    case 1:
      keyServo.write(1);
      break;
    case 2:
      senzorServo.write(1);
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
