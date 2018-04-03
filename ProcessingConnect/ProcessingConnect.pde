import processing.serial.*;
Serial[] port;

int index=0;
char[] inData = new char[20];
boolean[] isReady = new boolean[10];

void setup() {
   printArray(Serial.list());
   println("num:"+Serial.list().length);
   port = new Serial[Serial.list().length];
   for(int i=0;i<Serial.list().length;i++){
     port[i]=new Serial(this,Serial.list()[i], 9600);
     println(i+"-"+Serial.list()[i]);
   }
   // punt my string from a string to a char array
   //strChar = stringOut.toCharArray();
   for(int i=0;i<20;i++){
         inData[i]=0;
   }
   index=0;
   //port0.write("restart");
   //port1.write("restart");
   for(int i=0;i<10;i++){
         isReady[i]=false;
   }
}
void draw() {
  for(int i=0;i<Serial.list().length;i++){
    //the port I
    //==============================================
    if ( port[i].available() > 0) {
      char inChar = port[i].readChar();
      if(inChar!=',' && inChar!=0){
        inData[index] = inChar;
        println("index:"+index+"/inChar:"+inChar);
        index++; // Increment where to write next
      }
      else{
        index=0;
        //println("=================");
      }
      //********************************
      //start to comapre the string
      //inData[0] = what hapen 
      //inData[1] = Arduino Name
      if(inData[0]=='m'){//
        println("number "+inData[1]+" is main");
        //int Name2int = inData[1] - '0';
        for(int j=0;j<Serial.list().length;j++){
          if(j!=i)port[j].write("urnotmain");
        }
        delay(500);
        break;
      }
    }
  }
 
}
void mousePressed() {
 //port0.write("light on");
} 
void mouseReleased() {
 //port0.write("light off");
}
