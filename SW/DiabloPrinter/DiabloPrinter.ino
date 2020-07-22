#include <string.h>
#define DB1 PA0
#define DB2 PA1
#define DB3 PA2
#define DB4 PA3
#define DB5 PA4
#define DB6 PA5
#define DB7 PA6
#define DB8 PA7
#define DB9 PA8
#define DB10 PC14
#define DB11 PC15
#define DB12 PC13

#define SELECT PA15
#define RIB_LIFT PB15
#define PRT_WHL_STR PB14
#define PRT_OPT_STR PB13
#define PRT_CAR_STR PB12
#define PRT_FEED_STR PB11
#define RESTORE PB10

#define PAP_OUT PB0
#define END_OF_RIB PB1
#define COV_OPEN PB3
#define OPT_STATUS PB4
#define CHECK PB5
#define PRT_CAR_RDY PB6
#define PRT_WHL_RDY PB7
#define PRT_READY PB8
#define PRT_FEED_RDY PB9
#define CARRIAGE_LEFT 1
#define CARRIAGE_RIGHT 2
#define FEED_DOWN 3
#define FEED_UP 4


class DiabloPrinter {
    int hPos;
    int hSpace;
    int vSpace;
    int previousSpace = 0;
    void addPos (int increment) {
      hPos += increment;  
    };
    int getPos () {
      return hPos;
    }
    void strobePulse (int strobe) {
      digitalWrite(strobe,HIGH);
      delay(1);
      digitalWrite(strobe,LOW); 
      delay(1); 
    }
    void waitReady(int ready) {
      while (digitalRead(ready) == LOW) {
        delay(1);  
        //Serial.println("WaitReady!");  
      }
    }
    void carriageReady() {
      //Serial.println("Carriage Ready check");
      waitReady(PRT_CAR_RDY);
    }

    void wheelReady () {
      //Serial.println("Wheel Ready check");
      waitReady(PRT_WHL_RDY);
    }

    void feedReady () {
      //Serial.println("Feed Ready check");
      waitReady(PRT_FEED_RDY);
    }
    void outputDatabits(int bits) {
      digitalWrite(DB1, (bits & (1<<0))?HIGH:LOW);
      digitalWrite(DB2, (bits & (1<<1))?HIGH:LOW);
      digitalWrite(DB3, (bits & (1<<2))?HIGH:LOW);
      digitalWrite(DB4, (bits & (1<<3))?HIGH:LOW);
      digitalWrite(DB5, (bits & (1<<4))?HIGH:LOW);
      digitalWrite(DB6, (bits & (1<<5))?HIGH:LOW);
      digitalWrite(DB7, (bits & (1<<6))?HIGH:LOW);
      digitalWrite(DB8, (bits & (1<<7))?HIGH:LOW);
      digitalWrite(DB9, (bits & (1<<8))?HIGH:LOW);
      digitalWrite(DB10,(bits & (1<<9))?HIGH:LOW);
      digitalWrite(DB11,(bits & (1<<10))?HIGH:LOW);
      digitalWrite(DB12,(bits & (1<<11))?HIGH:LOW);
    }
    void sendCommand(int strobe, int ready, int value) {
      outputDatabits(value);
      strobePulse(strobe);
      waitReady(ready);
    }
    void sendCommandNoWait(int strobe, int value) {
      outputDatabits(value);
      strobePulse(strobe);
    }
  public:
  // direction true = left
  // direction false = right
    void moveCarriage (int direction, int value) {
      int tmp = value >> 1;
      //tmp=100;
      tmp |= ((value&0x01)<<11);
      tmp |= ((direction==CARRIAGE_LEFT?1:0)<<10);
      sendCommandNoWait(PRT_CAR_STR, tmp);
    }
    void printChar  (char ch) {
      if (ch == ' ') {
        previousSpace++;
      } else {
        moveCarriage(CARRIAGE_RIGHT, hSpace * (1 + previousSpace));
        addPos(hSpace * (1 + previousSpace));
        previousSpace=0;
        sendCommandNoWait(PRT_WHL_STR, ch);
        wheelReady();    
      }
      
    };
    //void printString (String str){
    //  printString(str.c_str()); 
    //};
    void printString (const char * str){
      int i;
      for (i=0; i< strlen(str); i++) {
        printChar(str[i]);    
      }
    };
    void restore () {
      // send restore pulse
      digitalWrite(RESTORE, HIGH);
      delay(100);
      digitalWrite(RESTORE, LOW);
    };
    void feed(int direction, int distance) {
      int tmp = distance;
      tmp |= ((direction == FEED_DOWN)?0:1<<10);
      sendCommand(PRT_FEED_STR, PRT_FEED_RDY,tmp);
    }
    void lineFeed () {
      feed(FEED_DOWN, vSpace);  
    };
    void formFeed () {
      int i;
      for (i=1; i<10; i++) {
        lineFeed();
      }
    }
    void carriageReturn () {
      moveCarriage(CARRIAGE_LEFT,getPos());
      carriageReady();
      hPos=0;
      previousSpace=0;
    };

    bool checkPrinter () {
      if (digitalRead(CHECK)==HIGH) {
        return true;
      }
      if (digitalRead(PRT_READY)==LOW) {
        return true;
      }
      return false;
    }
    DiabloPrinter (int hs = 12, int vs = 8) {
      pinMode(DB1,OUTPUT);
      pinMode(DB2,OUTPUT);
      pinMode(DB3,OUTPUT);
      pinMode(DB4,OUTPUT);
      pinMode(DB5,OUTPUT);
      pinMode(DB6,OUTPUT);
      pinMode(DB7,OUTPUT);
      pinMode(DB8,OUTPUT);
      pinMode(DB9,OUTPUT);
      pinMode(DB10,OUTPUT);
      pinMode(DB11,OUTPUT);
      pinMode(DB12,OUTPUT);
      pinMode(SELECT,OUTPUT);
      pinMode(PAP_OUT,INPUT);
      pinMode(END_OF_RIB,INPUT);
      pinMode(COV_OPEN,INPUT);
      pinMode(OPT_STATUS,INPUT);
      pinMode(CHECK,INPUT);
      pinMode(PRT_CAR_RDY,INPUT);
      pinMode(PRT_WHL_RDY,INPUT);
      pinMode(PRT_READY,INPUT);
      pinMode(PRT_FEED_RDY,INPUT);
      pinMode(RESTORE,OUTPUT);
      pinMode(PRT_FEED_STR,OUTPUT);
      pinMode(PRT_CAR_STR,OUTPUT);
      pinMode(PRT_OPT_STR,OUTPUT);      
      pinMode(PRT_WHL_STR,OUTPUT);
      pinMode(RIB_LIFT,OUTPUT); 

      // Set all outputs to sensible values.
      digitalWrite(DB1,LOW);
      digitalWrite(DB2,LOW);
      digitalWrite(DB3,LOW);
      digitalWrite(DB4,LOW);
      digitalWrite(DB5,LOW);
      digitalWrite(DB6,LOW);
      digitalWrite(DB7,LOW);
      digitalWrite(DB8,LOW);
      digitalWrite(DB9,LOW);
      digitalWrite(DB10,LOW);
      digitalWrite(DB11,LOW);
      digitalWrite(DB12,LOW);

      digitalWrite(RESTORE,LOW);
      digitalWrite(PRT_FEED_STR,LOW);
      digitalWrite(PRT_CAR_STR,LOW);
      digitalWrite(PRT_OPT_STR,LOW);      
      digitalWrite(PRT_WHL_STR,LOW);

      // Set ribbon up!
      digitalWrite(RIB_LIFT,HIGH); 
      
      // Select the printer
      digitalWrite(SELECT, HIGH);
      hSpace = hs;
      vSpace = vs;
      hPos=0;
      restore();
    }
};

DiabloPrinter * dp;
String readString;
bool previousCharIsCR;

void printLine(const char * str) {
  while (dp->checkPrinter()) {
        dp->restore();
        delay(1000);
        Serial.println("Printer not ready!");
      }
      Serial.println("Printing String");
      dp->printString(str);
      Serial.println("DOing CR");
      dp->carriageReturn();
      Serial.println("LF");
      dp->lineFeed();
}


void setup() {
  int i;
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  Serial1.begin(19200);
  // put your setup code here, to run once:
  previousCharIsCR = false;
  dp = new DiabloPrinter();
  //for (i=1; i<10; i++) {
  //  printLine("www.datormuseum.se ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  //}
}

void loop() {
  // put your main code here, to run repeatedly:


  char c;
  bool cr = false;
  int arg=0;
  
  if(Serial1.available()>0) {
    c = Serial1.read();
    if (c == '\r') {
      cr = true;
      previousCharIsCR = true;
    } else if (c == '\n') {
      if (!previousCharIsCR) {
        cr = true;
      }
      previousCharIsCR = false;
    }
    else if ((c>31) && (c<128)){ 
      readString.concat(c);
      previousCharIsCR = false;
    } 
      Serial.write(c);   
    }
    if (cr) {
      Serial1.write(19);
      Serial.println("");
      Serial.println("Sending XOFF");
      while (dp->checkPrinter()) {
        dp->restore();
        delay(1000);
        Serial.println("Printer not ready!");
      }
      Serial.println("Printing String");
      switch (readString.c_str()[0]) {
        case ' ':
          Serial.println("Doing space command");
          dp->lineFeed();
          break;
        case '+':
          Serial.println("Doing + command");
          break;
        case '1':
          Serial.println("Doing 1 command");
          dp->formFeed();
          break;
        case '0':
          Serial.println("Doing 0 command");
          dp->lineFeed();
          dp->lineFeed();
          break;
        case '-':
          Serial.println("Doing - command");  
          dp->lineFeed();
          dp->lineFeed();
          dp->lineFeed();
          break;
        default: 
          Serial.print("Doing default : ");
          Serial.write(readString.c_str()[0]);
          Serial.println("");
      }
      dp->carriageReturn();
      dp->printString((readString.c_str()+1));
      cr = false;
      readString = "";
      Serial1.write(17);
      Serial.println("Sending XON");
    }
}
