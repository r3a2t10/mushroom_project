import processing.serial.*;
//Serial[] port;
Serial port;
int index=0;
char[] inData = new char[20];

//char[] inData = new char[7];
boolean[] isPlaying = new boolean[7];
boolean[] haveSound = new boolean[7];
boolean[] isExist = new boolean[7];

boolean anystart = false;
boolean allisplaying;
boolean allhavesound;
int isexist;
int allisexist = 1;
void setup() {
   //frameRate(1);
   printArray(Serial.list());
   println("num:"+Serial.list().length);
   port = new Serial(this,Serial.list()[2], 9600);
   //port = new Serial[Serial.list().length];
   //for(int i=0;i<Serial.list().length;i++){
   //  port[i]=new Serial(this,Serial.list()[i], 9600);
   //  println(i+"-"+Serial.list()[i]);
   //}
   for(int i=0;i<isExist.length;i++){
         isExist[i] = false;
   }
   init();
   
}
void init() {
   for(int i=0;i<inData.length;i++){
         inData[i]=0;
   }
   for(int i=0;i<isPlaying.length;i++){
         isPlaying[i]=false;
   }
   for(int i=0;i<haveSound.length;i++){
         haveSound[i]=false;
   }
   anystart = false;
}
void draw() {
  
  //for(int i=0;i<Serial.list().length;i++){
    //the port I
    //==============================================
    if ( port.available() > 0) {
      char inChar = port.readChar();
      print("["+inChar+"]");
      if(inChar!=','){
        inData[index] = inChar;
        //println("index:"+index+"/inChar:"+inChar);
        if(inChar!=0)index++; // Increment where to write next
      }
      else{
        for(int i=0;i<inData.length;i++){
               inData[i]=0;
         } 
        index=0;
        //println("=================");
      }
      
      //inData[0]= port.readChar();//0
      //inData[1]= port.readChar();//1
      //inData[2]= port.readChar();//,
      println("inData:"+inData[0]+"/"+inData[1]);
      int mushNum = inData[1]-'0';
      if(mushNum<7 && inData[0]!=0 && inData[1]!=0){
      //String myWord = str(inData[0]) + str(inData[1]);
      //println(mushNum+myWord);
      //wrong way to scan the state of mushroom
      //because only use "if" will scan only one character 
      //mush's signal may mix together eg. [wanted]m1m2 [result]mm12
      //so every mush port scan two character together
      //so will get right signal 
      
      //********************************
      //start to comapre the string
      //inData[0] = what hapen 
      //inData[1] = Arduino Name
      
      
      //some one get the main signal and nbody is triggered
      println("anystart::"+anystart);
      if(anystart==false && inData[0]=='m'){
        
        //println(port[i]+" is main");
        println("main mush:" + mushNum);
        for(int z=0;z<isExist.length;z++){
          println("z>>>"+z);
          //if(isExist[z]==true){
            //println("exist!");
            if(z==mushNum){
              String myWord = "a"+str(mushNum)+",";
              println("write main>>>"+myWord);
              port.write(myWord);
            }
            //else{
            //  String myWord = "a"+str(z)+",";
            //  println("write side>>>"+myWord);
            //  port.write(myWord);
            //}
            isPlaying[z]=true;
            haveSound[z]=true;
            anystart = true;
          //}
        }
      }
      //that mush wait too long not triggered or play to the end
      else if(inData[0]=='n'){//
        //println("mush exist:" + mushNum);
        ////println(port[i]+" is exeist");
        //isExist[mushNum]=true;
        //isexist = 0;
        //for(int z=0;z<isExist.length;z++){
        //  if(isExist[z]==true)isexist = isexist + 1;
        //}
        //printArray(isExist);
        //println("isexist->" + isexist);
        //if(isexist==allisexist){
        //  println("allisexist!");
        //  //port.write("restart");
        //  for(int z=0;z<isExist.length;z++){
        //    //port.write("restart");
        //    if(isExist[z]==true){
        //      String myWord = "r"+str(z)+",";
        //      println("restart>>>"+myWord);
        //      port.write(myWord);
        //    }
        //    //if(isExist[z]==true)isexist = isexist + 1;
        //  }
        //}
        /*
        init();
        anystart=false;
        //port.write("restart");
        for(int z=0;z<isExist.length;z++){
          //port.write("restart");
            String myWord = "r"+str(z)+",";
            println("restart>>>"+myWord);
            port.write(myWord);
        }
        */
      }
      
      else if(inData[0]=='d'){//
        println("stop mush:" + mushNum);
        //println(port[i]+" is stop");
        isPlaying[mushNum]=false;
        init();
        anystart=false;
        //port.write("restart");
        for(int z=0;z<isExist.length;z++){
          //port.write("restart");
          //if(isExist[z]==true){
            String myWord = "r"+str(z)+",";
            println("restart>>>"+myWord);
            port.write(myWord);
          //}
        }
      }
      
      else if(inData[0]=='p'){
        println("mute mush:" + mushNum);
        //println(port[i]+" is mute");
        haveSound[mushNum]=false;
      }
      
      else if(inData[0]=='f'){
        println("sound mush:" + mushNum);
        //println(port[i]+" is sound");
        haveSound[mushNum]=true;
      }
      
      //mush are been triggered
      if(anystart==true){
        //=====================================
        //printArray(isPlaying);
        allisplaying = false;
        for(int z=0;z<isPlaying.length;z++){
          allisplaying = allisplaying || isPlaying[z];
        }
        println("allisplaying->"+allisplaying);
        
        //printArray(haveSound);
        allhavesound = false;
        for(int z=0;z<haveSound.length;z++){
          allhavesound = allhavesound || haveSound[z];
        }
        println("allhavesound->"+allhavesound);
        
        //if every body stop restart the period
        if(allisplaying==false || allhavesound==false){
        //if(allhavesound==false){
          init();
          anystart=false;
          //port.write("restart");
          for(int z=0;z<isExist.length;z++){
            //port.write("restart");
            if(isExist[z]==true){
              String myWord = "r"+str(z)+",";
              println("restart>>>"+myWord);
              port.write(myWord);
            }
          }
        }
        }
    }
    }

}

void keyPressed() {
 //port0.write("light on");
 if (key == 'b' || key == 'B') {
   init();
    anystart=false;
    //port.write("restart");
    for(int z=0;z<isExist.length;z++){
      //port.write("restart");
        String myWord = "r"+str(z)+",";
        println("restart>>>"+myWord);
        port.write(myWord);
    }
 }
 if (key == 'r' || key == 'R') {
   init();
    anystart=false;
    //port.write("restart");
    for(int z=0;z<isExist.length;z++){
      //port.write("restart");
        String myWord = "s"+str(z)+",";
        println("restart>>>"+myWord);
        port.write(myWord);
    }
 }
} 
void mouseReleased() {
 //port0.write("light off");
}
