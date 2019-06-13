int ROW_PINS[] = {2,3,4,5,6,7,8,12,13};
int COL_PINS[] = {9,10,11};
                  // ABC  DEF  GHI  JKL  MNO  PQR  STU  VWX  YZ
byte PATTERN[9][9]={{B000,B111,B111,B111,B111,B111,B111,B111,B000},
                    {B111,B000,B111,B111,B111,B111,B111,B000,B111},
                    {B111,B111,B000,B111,B111,B111,B000,B111,B111},
                    {B111,B111,B111,B000,B111,B000,B111,B111,B111},
                    {B111,B111,B111,B111,B000,B111,B111,B111,B111},
                    {B111,B111,B111,B000,B111,B000,B111,B111,B111},
                    {B111,B111,B000,B111,B111,B111,B000,B111,B111},
                    {B111,B000,B111,B111,B111,B111,B111,B000,B111},
                    {B000,B111,B111,B111,B111,B111,B111,B111,B000},
                   };//last part of pattern
#define ROW_COUNT (sizeof(ROW_PINS)/sizeof(ROW_PINS[0]))
#define COL_COUNT (sizeof(COL_PINS)/sizeof(COL_PINS[0]))
#define PATTERN_SIZE (9)
#define PATTERN_DELAY (65)
unsigned long previousMillis = 0; 
int count=0;
bool endAnimation = false;
void setup(){
  int pinset=0;
  Serial.begin(9600);
  for (pinset=0; pinset<9; pinset++){
    pinMode (ROW_PINS[pinset],OUTPUT);
    }
  for (pinset=0; pinset<3; pinset++){
    pinMode (COL_PINS[pinset],OUTPUT);
    }
}
void loop(){
  int pD = PATTERN_DELAY;
  //int loopControl = 0;
  animation(pD);
  endAnimation = false;
}

void animation(int pD){
    if(!endAnimation){
    DispOut(PATTERN[count]);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= pD) {
        previousMillis = currentMillis;
        Serial.println("satrt");
        count++;
        Serial.println("end");
        if(count==PATTERN_SIZE+1){
          count=0;
          endAnimation = true;
          }
    }
    }
  }

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
     /* 
    bool wait=true;
    while(wait){
       unsigned long currentMillis = millis();
       if (currentMillis - previousMillis >= waitTime) {
           previousMillis = currentMillis;
           wait=false;
      }
    }
    wait=true;*/
    delay(3);
    pattern_index++;
    }
}

