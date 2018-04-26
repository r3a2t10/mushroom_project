#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "FastLED.h"
#include <EEPROM.h>
#define BRIGHTNESS          96

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
int d = 30;
int bright = BRIGHTNESS;

float tempX;
float getGx;

int state = 5;
//state = 0 : waiting for the first trigger
//state = 1 : i am main theme
//state = 2 : i am other theme
//state = 3 : waiting for everybody stop
//state = 5 : waiting for everybody open
char myName ;
int ismotion = 0;
int isM = 0;

int waitingTime = 250;

boolean isPlaying  = false;
boolean SideisPlaying = false;
boolean haveSound = false;
boolean isTrigger = false;
boolean isDay = true;

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int count = 0; 
int sidecount = 0;
int minvol = 18;

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
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection( 0xAACC5c );
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  myName = EEPROM.read(0);

  //setCorrection( 0xAACC5c );
  FastLED.setBrightness(  100 );
  
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
  isTrigger = false;
  for(int i=0;i<19;i++){
   inData[i]=0;
 }
 index=0;
  writeDataFunction('n',myName);
  writeDataFunction('e',myName);
  //state = 1;
  
}
void lightranbow()
{
  if(isDay){
    fill_solid(leds,NUM_LEDS,0);
    FastLED.show();
  }
  else{
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
    gHue+=10;
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();
  }
}
void lightwhite()
{
  if(isDay){
    fill_solid(leds,NUM_LEDS,0);
    FastLED.show();
  }
  else{
    for(int i = 0;i<NUM_LEDS; i++){
      leds[i] = CRGB::White;//CRGB(d,d,d);
    }
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();  
  }
  
}
void lightyellow(int vol)
{
  if(isDay){
    fill_solid(leds,NUM_LEDS,0);
    FastLED.show();
  }
  else{
    int bri = BRIGHTNESS/12*(vol-minvol);
    for(int i = 0;i<NUM_LEDS; i++){
      leds[i] = CRGB::White;//CRGB(d,d,d);
    }
    FastLED.setBrightness(bri);
    FastLED.show();   
  }
}
void loop()
{
  static unsigned long timer = millis();
  static unsigned long timer2 = millis();
  static unsigned long timer3 = millis();
  static unsigned long timer4 = millis();

if(millis() - timer > 500){//scan every 10 second
  timer = millis();
  isM = isM + digitalRead(Din_pin);
  Serial.print(isM);
}

if(millis() - timer4 > 2000){//scan every 10 second
  timer4 = millis();
  if(isM==0){
    ismotion = 0;
    isM=0;
  }
  else{
    ismotion = 1;
    Serial.print("^^,");
    isM=0;
  }
}


if(millis() - timer2 > 3600000){//scan every 1 hour
  timer2 = millis();
  writeDataFunction('e',myName);
}
 //Serial.println(state);
if(millis() - timer3 > 1000){
 
 //digitalWrite(Dout_pin, HIGH);
 //writeDataFunction('t',state+'0');
 timer3 = millis();
 if(state == 0){
  isPlaying  = false;
  SideisPlaying = false;
  haveSound = false;
  myDFPlayer.stop();
  count = 0; 
  sidecount = 0;
  lightwhite();
   //等待狀態
    if(ismotion == 1){
      //等待狀態收到訊號:成為主旋律，主機會去過濾多餘的主旋律
      if(isTrigger==false){
        writeDataFunction('m',myName);
        isTrigger = true;
      }
      //delay(500);
      //state = 2;
      //立馬跟server說大家的狀況使之分配各個蘑菇下個狀態
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
      haveSound = true;
      writeDataFunction('f',myName);
      lightyellow(count);
    }
    else{
        //主旋律正在播放
        count++;
        //Serial.println(count);
        if(count<waitingTime){
          //delay(500);//count完後要delay不然會一直瘋狂重來
          //Serial.println("主旋律播放還未達到週期時間");
          //主旋律播放還未達到週期時間
          if(ismotion==1){
            //又被碰到:沒關係你本來就該播放
            if(haveSound==false){
              haveSound=true;
              writeDataFunction('f',myName);
            }
            mainvolume = averagevolume;
            myDFPlayer.volume(mainvolume);
            lightyellow(averagevolume);
          }
          else{
            //沒被碰到:漸弱or靜音
            //Serial.print(mainvolume);
            if(mainvolume < minvol){
              mainvolume = 0;
              if(haveSound==true){
                haveSound=false;
                writeDataFunction('p',myName);
              }
            }
            else{
              mainvolume = mainvolume - 2;
            }
            myDFPlayer.volume(mainvolume);
            if(mainvolume<minvol){lightyellow(minvol);}
            else {lightyellow(mainvolume);}
          }
        }
        else{
          //Serial.println("主旋律已經播放到達時間");//appear once
          //主旋律已經播放到達時間
          state = 3;
          writeDataFunction('d',myName);
       }
    }
 }
 else if(state == 2){
    //被告知為副旋律
    if(SideisPlaying == false){
      sidecount = 0 ;
      sidevolume = 10;
      myDFPlayer.volume(sidevolume);
      int a = myName - '0';
      myDFPlayer.play(a);
      SideisPlaying = true;
      haveSound = false;
      writeDataFunction('p',myName);
      lightyellow(sidevolume);
    }
    else{
        //副旋律正在播放
        sidecount++;
        //Serial.println(count);
        if(sidecount<waitingTime){
          //delay(500);//count完後要delay不然會一直瘋狂重來
          //Serial.println("副旋律播放還未達到週期時間");
          //副旋律播放還未達到週期時間
          if(ismotion==1){
            //又被碰到:沒關係你本來就該播放
            if(haveSound==false){
              haveSound=true;
              writeDataFunction('f',myName);
            }
            sidevolume = averagevolume;
            myDFPlayer.volume(sidevolume);
            lightyellow(averagevolume);
          }
          else{
            //沒被碰到:漸弱or靜音
            //Serial.print(sidevolume);
            if(sidevolume < minvol){
              sidevolume = 0;
              if(haveSound==true){
                haveSound=false;
                writeDataFunction('p',myName);
              }
            }
            else{
              sidevolume = sidevolume - 2;
            }
            myDFPlayer.volume(sidevolume);
            if(sidevolume<minvol){lightyellow(minvol);}
            else {lightyellow(sidevolume);}
          }
        }
        else{
          //Serial.println("副旋律已經播放到達時間");//appear once
          //副旋律已經播放到達時間
          state = 3;
          writeDataFunction('d',myName);
       }
    }
 }
 else if(state == 3){
  //wait everyone in the same state then start trigger
  isPlaying = false;
  SideisPlaying = false;
  haveSound = false;
  myDFPlayer.stop();
  lightwhite();
 }
 else if(state == 5){
  //wait for all mush join
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
//  char compare = Compp();
//  if(compare=='0'){
//    //main
//    state = 1;
//  }
//  if(compare=='1'){
//    //side
//    state = 2;
//  }
//  if(compare=='2'){
//    //restart
//    state = 0;
//    isTrigger=false;
//  }
  if (stringComplete) {
    //Serial.write("^");
    //Serial.write(inputString.charAt(0));
    //Serial.write(inputString.charAt(1));
    //Serial.write(inputString.charAt(2));
    if(inputString.charAt(0)=='a'&&inputString.charAt(1)==myName){
      state = 1;
      //isTrigger=false;
    }
    else if(inputString.charAt(0)=='a'&&inputString.charAt(1)!=myName){
      state = 2;
      //isTrigger=false;
    }
    else if(inputString.charAt(0)=='r'&&inputString.charAt(1)==myName){
      state = 0;
      isTrigger=false;
    }
    else if(inputString.charAt(0)=='s'&&inputString.charAt(1)==myName){
      setup();
      //isTrigger=false;
    }
    else if(inputString.charAt(0)=='w'&&inputString.charAt(1)==myName){
      isDay = true;
      state = 0;
      isTrigger=false;
    }
    else if(inputString.charAt(0)=='q'&&inputString.charAt(1)==myName){
      isDay = false;
      state = 0;
      isTrigger=false;
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

//compare the string if the same return 0
//====================================================
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == ',') {
      stringComplete = true;
    }
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
