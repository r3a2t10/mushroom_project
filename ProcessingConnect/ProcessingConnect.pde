import processing.serial.*;
Serial[] port;

int index=0;
char[] inData = new char[20];
boolean[] isPlaying = new boolean[10];
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
   for(int i=0;i<20;i++){
         inData[i]=0;
   }
   index=0;
   for(int i=0;i<10;i++){
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
      
      
      if(anystart==false && inData[0]=='m'){//
        
        println("PORT i IS m:"+i);
        println(port[i]+" is main");
        isPlaying[i]=true;
        anystart = true;
        
        for(int j=0;j<Serial.list().length;j++){
          println("j:"+j);
          if(j!=i){
            println(port[j]+" is not main");
            port[j].write("urnotmain");
            isPlaying[j]=true;
          }
        }
        //break;
      }
      if(inData[0]=='d'){//
        println("stop port:"+i);
        println(port[i]+" is stop");
        isPlaying[i]=false;
        //break;
      }
      if(anystart==true){
        printArray(isPlaying);
        allisplaying = false;
        for(int z=0;z<10;z++){
          allisplaying = allisplaying || isPlaying[z];
        }
        println("allisplaying->"+allisplaying);
        //for(int j=0;j<Serial.list().length;j++){
        //  port[j].write("restart");
        //}
        //anystart = false;
      }
    }
  }
  if(anystart==true && allisplaying==false){
    for(int j=0;j<Serial.list().length;j++){
      port[j].write("restart");
    }
    anystart=false;
  }
 
}
void mousePressed() {
 //port0.write("light on");
} 
void mouseReleased() {
 //port0.write("light off");
}
