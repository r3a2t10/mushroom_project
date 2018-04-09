#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "FastLED.h"
#include <EEPROM.h>

#define X_Pin A0
#define Y_Pin A1
#define Z_Pin A2

int Din_pin=11;
int Dout_pin=13;
int M1 = 5;
int M0 = 6;

#define NUM_LEDS 45
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
uint8_t gHue = 0; 
int counterDelay= 0 ;
int d = 10;

float tempX;
float getGx;

int state = 0;
//state = 0 : waiting for the first trigger
//state = 1 : i am main theme
//state = 2 : i am other theme
char myName ;
int ismotion = 0;
int waitingTime = 20;

boolean isPlaying  = false;
boolean SideisPlaying = false;
int count = 0; 
int sidecount = 0;

int averagevolume = 30;
int mainvolume = 30;
int sidevolume = 30;

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

SoftwareSerial mySoftwareSerial(9, 10); // RX, TX  //V1

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

// call this function can write data in sucessful way
char writeData[3] = "";
void writeDataFunction(char Action,char Name)
{
    writeData[0] = Action;
    writeData[1] = Name;
    writeData[2] = ',';
    for(int i=0;i<sizeof(writeData);i++){
        Serial.write(writeData[i]);
    }
}

void setup()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  myName = EEPROM.read(0);
  //Serial.println(myName);
  
  pinMode(M1, OUTPUT);
  pinMode(M0, OUTPUT);
  digitalWrite(M1,LOW);
  digitalWrite(M0,LOW);
  pinMode(Din_pin, INPUT);
  pinMode(Dout_pin, OUTPUT);
    
  //Serial.println();
  //Serial.println(F("DFRobot DFPlayer Mini Demo"));
  //Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  //LoRaSerial.println("Initial Ready");
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  //Serial.println(F("DFPlayer Mini online."));
  //LoRaSerial.println("Mushroom Ready");
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  //myDFPlayer.play(2);  //Play the first mp3
  //myDFPlayer.enableLoop(); //enable loop.
  //myDFPlayer.next();
  //Serial.println(myDFPlayer.readCurrentFileNumber()); 
}
void lightranbow()
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  gHue++;
  FastLED.show();
}
void lightwhite()
{
  for(int i = 0;i<NUM_LEDS; i++){
    leds[i] = CRGB(d,d,d);
  }
  FastLED.show();  
}
void loop()
{
  static unsigned long timer = millis();
  static unsigned long timer3 = millis();

 ismotion = digitalRead(Din_pin);
 digitalWrite(Dout_pin, HIGH);
 //Serial.println(state);

 // finite-state machine to decide what to do now
 //state = 0 : waiting for the first trigger
 //state = 1 : i am main theme
 //state = 2 : i am other theme

 if(state == 0){
  isPlaying  = false;
  SideisPlaying = false;
  count = 0; 
  sidecount = 0;
	 //等待狀態
    if(ismotion == 1){
      //等待狀態收到訊號-立即成為主旋律
      state = 1;
      //立馬跟其他人廣播使其他人變副旋律(state 2)
    }
    else{
      //等待狀態沒有訊號，繼續等待
    }
 }
 else if(state == 1){
    //我是主旋律
    //主旋律之前還沒有開播
    if(isPlaying == false){
      count = 0 ;
      mainvolume = averagevolume;
      myDFPlayer.volume(mainvolume);
      myDFPlayer.play(1);//播放第一首歌
      isPlaying = true;
      lightranbow();
      writeDataFunction('m',myName);
    }
    else{
        //主旋律正在播放
        count++;
        //Serial.println(count);
        if(count<waitingTime){
          delay(1000);//count完後要delay不然會一直瘋狂重來
          //Serial.println("主旋律播放還未達到週期時間");
          //主旋律播放還未達到週期時間
          if(ismotion==1){
            //又被碰到:沒關係你本來就該播放
            mainvolume = averagevolume;
            myDFPlayer.volume(mainvolume);
            lightranbow();
          }
          else{
            //沒被碰到:漸弱or靜音
            mainvolume = mainvolume - 1;
            myDFPlayer.volume(mainvolume);
            lightwhite();
          }
        }
        else{
          //Serial.println("主旋律已經播放到達時間");//appear once
          //主旋律已經播放到達時間
          isPlaying = false;
          myDFPlayer.stop();
          state = 3;
          lightwhite();
          writeDataFunction('d',myName);
       }
    }
 }
 else if(state == 2){
    //被告知為副旋律
    if(SideisPlaying == false){
      sidecount = 0 ;
      sidevolume = averagevolume;
      myDFPlayer.volume(sidevolume);
      int a = myName - '0';
      myDFPlayer.play(a);
      SideisPlaying = true;
      lightranbow();
      //writeDataFunction('s',myName);
    }
    else{
        //副旋律正在播放
        sidecount++;
        //Serial.println(count);
        if(sidecount<waitingTime){
          delay(1000);//count完後要delay不然會一直瘋狂重來
          //Serial.println("副旋律播放還未達到週期時間");
          //副旋律播放還未達到週期時間
          if(ismotion==1){
            //又被碰到:沒關係你本來就該播放
            sidevolume = averagevolume;
            myDFPlayer.volume(sidevolume);
            lightranbow();
          }
          else{
            //沒被碰到:漸弱or靜音
            sidevolume = sidevolume - 1;
            myDFPlayer.volume(sidevolume);
            lightwhite();
          }
        }
        else{
          //Serial.println("副旋律已經播放到達時間");//appear once
          //副旋律已經播放到達時間
          SideisPlaying = false;
          myDFPlayer.stop();
          state = 3;
          lightwhite();
          writeDataFunction('d',myName);
       }
    }
 }
 else if(state ==3){
  //writeDataFunction('z',myName);
  //wait everyone in the same state
  if(Comp("allstop")==0){
    //writeDataFunction('z',myName);
    state = 0;
  }
 }
  
  //==================================================
  
  if (myDFPlayer.available()) {
     //printDetail(myDFPlayer.readType(), myDFPlayer.read());
	 //Print the detail message from DFPlayer to handle different errors and states.
     if(myDFPlayer.readType()=="DFPlayerPlayFinished"){
          myDFPlayer.next();
      }
  }
  //==================================================
  //compare the sting from processing 判斷從外面來的訊號字串
  if(Comp("urnotmain")==0){
      if(state!=2){
		//if interupt then
		//1.reset the interupted state
		//2.change the state(can't only change state)
        isPlaying = false;
        myDFPlayer.stop();
        state = 2;
      }
      //writeDataFunction('t',state+'0');
  }
  if(Comp("restart")==0){
    state=0;
  }
}

//compare the string if the same return 0
//====================================================
char Comp(char* This){

 while(Serial.available() > 0) // Don't read unless
   // there you know there is data
 {
   if(index < 19) // One less than the size of the array
   {
     inChar = Serial.read(); // Read a character
     inData[index] = inChar; // Store it
     index++; // Increment where to write next
     inData[index] = '\0'; // Null terminate the string
   }
 }

 if(strcmp(inData,This)  == 0){
   for(int i=0;i<19;i++){
     inData[i]=0;
   }
   index=0;
   return(0);

 }
 else{
   return(1);

 }
}
//print DFPlayer Detail information
//====================================================
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
