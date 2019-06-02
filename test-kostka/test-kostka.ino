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

int Time=1000;

void setup() {
  for (int thisPin = 2; thisPin < 14; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
  
}

void loop() {
  setRow(ROW0);
  for (int thisPin = 2; thisPin < 9; thisPin++) {
    offOn(thisPin);
    delay(Time);
    offOn(thisPin);
  }
  for (int thisPin = 12; thisPin < 14; thisPin++) {
    offOn(thisPin);
    delay(Time);
    offOn(thisPin);
  }

  
  setRow(ROW1);
  for (int thisPin = 2; thisPin < 9; thisPin++) {
    offOn(thisPin);
    delay(Time);
    offOn(thisPin);
  }
  for (int thisPin = 12; thisPin < 14; thisPin++) {
    offOn(thisPin);
    delay(Time);
    offOn(thisPin);
  }
  
  setRow(ROW2);
  for (int thisPin = 2; thisPin < 9; thisPin++) {
    offOn(thisPin);
    delay(Time);
    offOn(thisPin);
  }
  for (int thisPin = 12; thisPin < 14; thisPin++) {
    offOn(thisPin);
    delay(Time);
    offOn(thisPin);
  }
  Time = 0;
}


void setRow(int pin){
  switch (pin) {
    case ROW0:
      analogWrite(ROW0, 0);    
      analogWrite(ROW1, 255);
      analogWrite(ROW2, 255);
      break;
    case ROW1:
      analogWrite(ROW0, 255);    
      analogWrite(ROW1, 0);
      analogWrite(ROW2, 255);
      break;
    case ROW2:
      analogWrite(ROW0, 255);    
      analogWrite(ROW1, 255);
      analogWrite(ROW2, 0);
      break;
  }
}

void offOn(int pin){
  digitalWrite(pin, !digitalRead(pin));
}
