import processing.serial.*;
Serial[] port;
int index=0;
char[] inData = new char[6];
boolean[] isPlaying = new boolean[6];
boolean anystart = false;
boolean allisplaying;
void setup() {
   //frameRate(1);
   printArray(Serial.list());
   println("num:"+Serial.list().length);
   port = new Serial[Serial.list().length];
   for(int i=0;i<Serial.list().length;i++){
     port[i]=new Serial(this,Serial.list()[i], 9600);
     println(i+"-"+Serial.list()[i]);
   }
   // punt my string from a string to a char array
   //strChar = stringOut.toCharArray();
   for(int i=0;i<inData.length;i++){
         inData[i]=0;
   }
   index=0;
   for(int i=0;i<isPlaying.length;i++){
         isPlaying[i]=false;
   }
}
void draw() {
  
  for(int i=0;i<Serial.list().length;i++){
    //the port I
    //==============================================
    if ( port[i].available() > 0) {
      //char inChar = port[i].readChar();
      
      inData[0]= port[i].readChar();
      inData[1]= port[i].readChar();//,
      inData[2]= port[i].readChar();//?
      println("inData:"+inData[0]+"/"+inData[1]);
      
      //wrong way to scan the state of mushroom
      //because only use "if" will scan only one character 
      //mush's signal may mix together eg. [wanted]m1m2 [result]mm12
      //so every mush port scan two character together
      //so will get right signal 
      //if(inChar!=',' && inChar!=0){
      //  inData[index] = inChar;
      //  println("index:"+index+"/inChar:"+inChar);
      //  index++; // Increment where to write next
      //}
      //else{
      //  index=0;
      //  //println("=================");
      //}
      //********************************
      //start to comapre the string
      //inData[0] = what hapen 
      //inData[1] = Arduino Name
      
      
      //some one get the main signal and nbody is triggered
      if(anystart==false && inData[0]=='m'){
        //tell the main mushroom the period is triggered
        println(port[i]+" is main");
        port[i].write("urmain");
        isPlaying[i]=true;
        anystart = true;
        
        //tell the side mushroom
        for(int j=0;j<Serial.list().length;j++){
          if(j!=i){//skip the main mushroom
            println(port[j]+" is not main");
            port[j].write("urnotmain");
            isPlaying[j]=true;
          }
        }
      }
      //that mush wait too long not triggered or play to the end
      else if(inData[0]=='d'){//
        println("stop port:"+i);
        println(port[i]+" is stop");
        isPlaying[i]=false;
      }
      
      //mush are been triggered
      if(anystart==true){
        printArray(isPlaying);
        //check every body is singing or not
        allisplaying = false;
        for(int z=0;z<isPlaying.length;z++){
          allisplaying = allisplaying || isPlaying[z];
        }
        println("allisplaying->"+allisplaying);
        
        //if every body stop restart the period
        if(allisplaying==false){
          for(int j=0;j<Serial.list().length;j++){
            port[j].write("restart");
          }
          anystart=false;
        }
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
