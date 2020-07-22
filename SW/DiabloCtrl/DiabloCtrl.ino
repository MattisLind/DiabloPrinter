/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/

#define DB1 PA0
#define DB2 PA1
#define DB3 PA2
#define DB4 PA3
#define DB5 PA4
#define DB6 PA5
#define DB7 PA6
#define DB8 PA7
#define DB9 PA8
#define DB10 PA9
#define DB11 PA10
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


String readString;
char select = 0;
char lift=0;


void printUsage() 
{
  Serial.println("S - select, D - Diode, R - Restore, F - Feed, C - Carrige, O - Option, W - Wheel, L - Lift Ribbon");
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  pinMode(LED_BUILTIN, OUTPUT);
   Serial.begin(9600);
   printUsage();
   Serial.write("DIABLOCTRL> ");
   readString="";
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
}

void outputDatabits(int bits)
{
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

void printStatus () {
  if (digitalRead(PAP_OUT)) {
    Serial.write("PaperOut ");
  }
  if (digitalRead(END_OF_RIB)) {
    Serial.write("EndOfRibbon ");
  }
  if (digitalRead(COV_OPEN)) {
    Serial.write("CoverOpen ");
  }
  if (digitalRead(OPT_STATUS)) {
    Serial.write("OptionStatus ");
  }
  if (digitalRead(CHECK)) {
    Serial.write("Check ");
  }
  if (digitalRead(PRT_CAR_RDY)) {
    Serial.write("CarriageReady ");
  }
  if (digitalRead(PRT_WHL_RDY)) {
    Serial.write("WheelReady ");
  }
  if (digitalRead(PRT_READY)) {
    Serial.write("PrinterReady ");
  }
  if (digitalRead(PRT_FEED_RDY)) {
    Serial.write("FeedReady ");
  }
  Serial.println("");
}



// the loop function runs over and over again forever
void loop() {
  char c;
  bool cr = false;
  int arg=0;
  
  if(Serial.available()>0) {
    c = Serial.read();
    if (c == '\r') {
      cr = true;
    }
    else { 
      readString.concat(c);
      } 
      Serial.write(c);   
    }
    if (cr) {
      cr = false;
      arg = atoi(readString.substring(1).c_str());
      //Serial.println(readString);
      //Serial.println(readString[0]);
      //Serial.println(arg);
      switch (readString[0]) {
        case 'D':
        case 'd':
        if (arg==1) {
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
          Serial.println("LED on");
        }
        else {
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          Serial.println("LED off");
        }
        break;
        case 'S':
        case 's':
        // toggle select 
          select = 1 - select;
          digitalWrite(SELECT, select);
          break;
        case 'R':
        case 'r':
        // send restore pulse
          digitalWrite(RESTORE, HIGH);
          delay(100);
          digitalWrite(RESTORE, LOW);
          break;
        case 'F':
        case 'f':
          outputDatabits(arg);
          delay(1);
          digitalWrite(PRT_FEED_STR,HIGH);
          delay(1);
          digitalWrite(PRT_FEED_STR,LOW);
          break;
        case 'C':
        case 'c':
          outputDatabits(arg);
          delay(1);
          digitalWrite(PRT_CAR_STR,HIGH);
          delay(1);
          digitalWrite(PRT_CAR_STR,LOW);
          break;
        case 'O':
        case 'o':
          outputDatabits(arg);
          delay(1);
          digitalWrite(PRT_OPT_STR,HIGH);
          delay(1);
          digitalWrite(PRT_OPT_STR,LOW);
          break;
        case 'W':
        case 'w':
          outputDatabits(arg);
          delay(1);
          digitalWrite(PRT_WHL_STR,HIGH);
          delay(1);
          digitalWrite(PRT_WHL_STR,LOW);
          break;
        case 'L':
        case 'l':
        // toggle select 
          lift = 1 - lift;
          digitalWrite(RIB_LIFT, lift);
          //digitalWrite(PA15, lift);
          break;
          
           
      }
      readString="";
      delay(10);
      printStatus();
      printUsage();
      Serial.write("DIABLOCTRL> ");
    }
  }

