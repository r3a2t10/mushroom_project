import processing.serial.*;
Serial port0;
Serial port1;

int index=0;
char[] inData = new char[20];

void setup() {
   printArray(Serial.list());
   port0 = new Serial(this,Serial.list()[0], 9600);
   port1 = new Serial(this,Serial.list()[1], 9600);
   // punt my string from a string to a char array
   //strChar = stringOut.toCharArray();
   for(int i=0;i<20;i++){
         inData[i]=0;
   }
   index=0;
   
}
void draw() {
  //==============================================
  if ( port0.available() > 0) {
    char inChar = port0.readChar();
    //char val = port.readChar();
    if(inChar!=',' && inChar!=0){
      inData[index] = inChar;
      println("index:"+index+"/inChar:"+inChar);
      index++; // Increment where to write next
      //inData[index] = ' '; // Null terminate the string
      //print(inData[index]);
    }
    else{
      index=0;
      //println("=================");
    }
    //********************************
    //start to comapre the string
    //printArray(inData[0]);
    //printArray(inData[1]);
    if(inData[0]=='m' && inData[1]=='2'){
      println("number 2 is main");
      port1.write("urnotmain");
      delay(2000);
    }
  }
  //==============================================
  else if ( port1.available() > 0) {
    char inChar = port1.readChar();
    //char val = port.readChar();
    if(inChar!=',' && inChar!=0){
      inData[index] = inChar;
      println("index:"+index+"/inChar:"+inChar);
      index++; // Increment where to write next
      //inData[index] = ' '; // Null terminate the string
      //print(inData[index]);
    }
    else{
      index=0;
      //println("=================");
    }
    //********************************
    //start to comapre the string
    //printArray(inData[0]);
    //printArray(inData[1]);
    if(inData[0]=='m' && inData[1]=='3'){
      println("number 3 is main");
      port0.write("urnotmain");
      delay(2000);
    }
  }
}
void mousePressed() {
 //port0.write("light on");
 //port0.write("urnotmain");
} 
void mouseReleased() {
 //port0.write("light off");
}
