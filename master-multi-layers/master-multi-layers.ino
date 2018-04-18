#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>


/* ******************************************************************************************
 * Arguments Area
 * ******************************************************************************************
 */

// wifi setting
char ssid[] = "PiFi";
char pass[] = "pifi@7002";

// WiFiUDP 
WiFiUDP Udp;



// ID
const unsigned int ID = 199;                      // Board ID
const unsigned int LISTEN_PORT = 8888;            // no need to change

// color setting
const unsigned int COLOR_COUNTS = 7;
unsigned int colorList[COLOR_COUNTS] = {0xFF629D, 0xFF6897, 0xFF9867, 0xFFB04F, 0xFF5AA5, 0xFF42BD, 0xFF4AB5};

// global varibale

int ledState = -1;
unsigned int sourceID = 0;
unsigned int colorCode = 0;

// delay interval

const unsigned int INTERVAL = 500;


/*  *****************************************************************************************
 *  Functions Area
 *  *****************************************************************************************
 */ 
 
void sendOSCTo(int targetID = 255, int ledState=0, unsigned int colorCode=0, int sourceID = 2);
void receiveOSC();

void setup() {
  //wifi
  WiFi.config(IPAddress(192,168,1,ID),IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  
  Udp.begin(LISTEN_PORT);
    
  //for debug
  Serial.begin(115200);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
}

void loop() {
   receiveOSC();
}

void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID){
    // the message formate: ledState, colorCode, sourceID.    
    // send this message to targetID
    OSCMessage msgOut("/color");
    msgOut.add(ledState);    // add led state 
    msgOut.add(colorCode);   // add color code 
    msgOut.add(sourceID);    // add source ID
    // send via udp
    Udp.beginPacket(IPAddress(192,168,1,targetID), LISTEN_PORT);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();
}

//void receiveOSC(){
//  // listen to LISTEN_PORT, if get message, process to get it. then call colorEvent() funciton.
//  OSCMessage msgIN;
//    int size;
//    if((size = Udp.parsePacket())>0){
//        while(size--)
//            msgIN.fill(Udp.read());
//        if(!msgIN.hasError()){
//            msgIN.route("/color", colorEvent);
//        }
//    } 
//}

//void receiveOSC(){
//  // listen to LISTEN_PORT, if get message, process to get it. then call colorEvent() funciton.
//  OSCMessage msgIN;
//    int size;
//    if((size = Udp.parsePacket())>0){
//        while(size--)
//            msgIN.fill(Udp.read());
//        if(!msgIN.hasError()){
//            msgIN.route("/color", colorEvent); 
//        }
//    while((size = Udp.parsePacket()) >0){
//      while(size--)
//        Udp.read();
//     }
//    } 
//}

void receiveOSC(){
  // listen to LISTEN_PORT, if get message, process to get it. then call colorEvent() funciton.
  OSCMessage msgIN;
    int size;
    if((size = Udp.parsePacket())>0){
        while(size--)
            msgIN.fill(Udp.read());
        if(!msgIN.hasError()){
            msgIN.route("/color", colorEvent); 
        }
    while((size = Udp.parsePacket()) >0){
      while(size--)
        Udp.read();
     }
    } 
}

void colorEvent(OSCMessage &msg, int addrOffset){
  Serial.println("message receive!");
  
  ledState = msg.getInt(0);
  Serial.print("led state: ");
  Serial.println(ledState);
  
  colorCode = msg.getInt(1);
  Serial.print("color code: ");
  Serial.println(colorCode);

  int sourceID = msg.getInt(2);
  Serial.print("message from: ");
  Serial.println(sourceID);

  delay(INTERVAL);
  switch (sourceID){
    case 200:{
      int targetIDList[9][9] = {
                                  { 201, 205},
                                  { 202, 206, 210},
                                  { 203, 207, 211, 215},
                                  { 204, 208, 212, 216, 220},
                                  { 209, 213, 217, 221, 225},
                                  { 214, 218, 222, 226},
                                  { 219, 223, 227},
                                  { 224, 228},
                                  { 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 201:{
      int targetIDList[9][9] = {
                                  { 200, 206,202},
                                  { 205, 211, 207, 203},
                                  { 210, 216, 212, 208, 204},
                                  { 215, 221, 217, 213, 209},
                                  { 220, 226, 222, 218, 214},
                                  { 225, 227, 223, 219},
                                  { 228, 224},
                                  { 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 202:{
      int targetIDList[9][9] = {
                                  { 201, 207, 203},
                                  { 200, 206, 212, 208, 204},
                                  { 205, 211, 217, 213, 209},
                                  { 210, 216, 222, 218, 214},
                                  { 215, 221, 227, 223, 219},
                                  { 220, 226, 228, 224},
                                  { 225, 229}
                                  };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 203:{
      int targetIDList[9][9] = {
                                  { 202, 204, 208},
                                  { 201, 207, 213, 209},
                                  { 200, 206, 212, 218, 214},
                                  { 205, 211, 217, 223, 219},
                                  { 210, 216, 222, 219},
                                  { 215, 221, 227, 229},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 204:{
      int targetIDList[9][9] = {
                                  { 203, 209},
                                  { 202, 208, 214}, 
                                  { 201, 207, 213, 219},
                                  { 200, 206, 212, 218, 224},
                                  { 205, 211, 217, 223, 229},
                                  { 210, 216, 222, 228},
                                  { 215, 211, 227},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 205:{
      int targetIDList[9][9] = {
                                  { 200, 206, 210},
                                  { 201, 207, 211, 215},
                                  { 220, 216, 212, 208, 202},
                                  { 203, 209, 213, 217, 221, 225},
                                  { 226, 222, 218, 214, 204},
                                  { 219, 223, 227},
                                  { 228, 224},
                                  { 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 206:{
      int targetIDList[9][9] = {
                                  { 201, 205, 207, 211},
                                  { 200, 202, 208, 212, 216, 210},
                                  { 203, 209, 213, 217, 221, 215},
                                  { 220, 226, 222, 218, 214, 204},
                                  { 219, 223, 227, 225},
                                  { 228, 224},
                                  { 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 207:{
      int targetIDList[9][9] = {
                                  { 202, 208, 212, 206},
                                  { 203, 209, 213, 217, 211, 205, 201},
                                  { 200, 210, 216, 222, 218, 214, 204},
                                  { 215, 221, 227, 223, 219},
                                  { 220, 226, 228, 224},
                                  { 225, 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 208:{
      int targetIDList[9][9] = {
                                  { 203, 207, 209, 213},
                                  { 202, 204, 214, 218, 212, 206},
                                  { 201, 205, 211, 217, 223, 219},
                                  { 200, 210, 216, 222, 228, 224},
                                  { 215, 221, 227, 229},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 209:{
      int targetIDList[9][9] = {
                                  { 204, 208, 214},
                                  { 203, 207, 213, 219},
                                  { 202, 206, 212, 218, 224},
                                  { 201, 205, 211, 217, 223, 229},
                                  { 200, 210, 216, 222, 228},
                                  { 215, 221, 227},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    
    case 210:{
      int targetIDList[9][9] = {
                                  { 205, 211, 215},
                                  { 200, 206, 212, 216, 220},
                                  { 201, 207, 213, 217, 221, 225},
                                  { 202, 208, 214, 218, 222, 226},
                                  { 203, 209, 219, 223, 227},
                                  { 204, 224, 228},
                                  { 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 211:{
      int targetIDList[9][9] = {
                                  { 206, 210, 212, 216},
                                  { 201, 205, 207, 213, 215, 217, 221},
                                  { 200, 202, 208, 214, 218, 222, 226, 220},
                                  { 203, 209, 219, 223, 227, 225},
                                  { 204, 224, 228},
                                  { 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 212:{
      int targetIDList[9][9] = {
                                  { 207, 213, 217, 211},
                                  { 202, 208, 214, 218, 222, 216, 210, 206},
                                  { 201, 203, 209, 219, 223, 227, 221, 215, 205},
                                  { 220, 204, 220, 226, 228, 224},
                                  { 225, 229}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 213:{
     int targetIDList[9][9] = {
                                  { 209, 213, 219},
                                  { 204, 208, 212, 218, 224},
                                  { 203, 207, 211, 217, 223, 229},
                                  { 202, 206, 210, 216, 222, 228},
                                  { 201, 205, 215, 221, 227},
                                  { 200, 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 214:{
      int targetIDList[9][9] = {
                                  { 209, 213, 219},
                                  { 204, 208, 212, 218, 224},
                                  { 203, 207, 211, 217, 223, 229},
                                  { 202, 206, 210, 216, 222, 228},
                                  { 201, 205, 215, 221, 227},
                                  { 200, 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 215:{
      int targetIDList[9][9] = {
                                  { 210, 216, 220},
                                  { 205, 211, 217, 221, 225},
                                  { 200, 206, 212, 218, 222, 226},
                                  { 201, 207, 213, 219, 223, 227},
                                  { 202, 208, 214, 224, 228},
                                  { 203, 209, 229},
                                  { 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 216:{
      int targetIDList[9][9] = {
                                  { 211, 215, 217, 221},
                                  { 206, 212, 218, 222, 226, 220, 210},
                                  { 201, 207, 213, 219, 223, 227, 225, 205},
                                  { 200, 202, 208, 214, 224, 228},
                                  { 203, 209, 229},
                                  { 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 217:{
      int targetIDList[9][9] = {
                                  { 212, 216, 222, 218},
                                  { 207, 213, 219, 223, 227, 221, 215, 211},
                                  { 202, 206, 208, 214, 224, 228, 226, 220, 210},
                                  { 201, 203, 209, 229, 225, 205},
                                  { 200, 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 218:{
      int targetIDList[9][9] = {
                                  { 213, 217, 219, 223},
                                  { 208, 212, 214, 216, 222, 228, 224},
                                  { 203, 207, 209, 211, 215, 221, 227, 229},
                                  { 202, 204, 206, 210, 220, 226},
                                  { 201, 205, 225},
                                  { 200}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 219:{
      int targetIDList[9][9] = {
                                  { 214, 218, 224},
                                  { 209, 213, 217, 223, 229},
                                  { 204, 208, 212, 216, 222, 228},
                                  { 203, 207, 211, 215, 221, 227},
                                  { 202, 206, 210, 220, 226},
                                  { 201, 205, 225},
                                  { 200}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 220:{
      int targetIDList[9][9] = {
                                  { 215, 221, 225},
                                  { 210, 216, 222, 226},
                                  { 205, 211, 217, 223, 227},
                                  { 200, 206, 212, 218, 224, 228},
                                  { 201, 207, 213, 219, 229},
                                  { 202, 208, 214},
                                  { 203, 209},
                                  { 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 221:{
      int targetIDList[9][9] = {
                                  { 216, 220, 222, 226},
                                  { 211, 217, 223, 227, 225, 215},
                                  { 206, 212, 210, 218, 224, 226},
                                  { 201, 207, 213, 219, 229, 205},
                                  { 202, 200, 208, 214},
                                  { 203, 209},
                                  { 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 222:{
      int targetIDList[9][9] = {
                                  { 217, 221, 223, 227},
                                  { 212, 218, 224, 228, 226, 220, 216},
                                  { 207, 213, 219, 229, 225, 215, 211},
                                  { 202, 208, 214, 206, 210},
                                  { 201, 203, 205, 209},
                                  { 200, 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }
    
    case 223:{
      int targetIDList[9][9] = {
                                  { 218, 222, 224, 228},
                                  { 213, 217, 219, 221, 227, 229},
                                  { 208, 212, 214, 216, 220, 226},
                                  { 203, 207, 209, 211, 215, 225},
                                  { 202, 206, 210, 204},
                                  { 201, 205},
                                  { 200}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 224:{
      int targetIDList[9][9] = {
                                  { 219, 223, 229},
                                  { 214, 218, 222, 228},
                                  { 209, 213, 217, 221, 227},
                                  { 204, 208, 212, 216, 220, 226},
                                  { 203, 207, 211, 215, 225},
                                  { 202, 206, 210},
                                  { 201, 205},
                                  { 200}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 225:{
      
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 226:{
      int targetIDList[9][9] = {
                                  { 225,221,227},
                                  { 220,216,222,228},
                                  { 215,211,217,223,229},
                                  { 210,206,212,218,224},
                                  { 205,201,207,213,219},
                                  { 200,202,208,214},
                                  { 203,209},
                                  { 204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 227:{
      int targetIDList[9][9] = {
                                  { 226,222,228},
                                  { 225,221,217,223,229},
                                  { 220,216,212,218,224},
                                  { 215,211,207,213,219},
                                  { 210,206,202,208,214},
                                  { 205,201,203,209},
                                  { 200,204}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 228:{
      int targetIDList[9][9] = {
                                  { 227,223,229},
                                  { 226,222,218,224},
                                  { 225,221,217,213,219},
                                  { 220,216,212,208,214},
                                  { 215,211,207,203,209},
                                  { 210,206,202,204},
                                  { 205,201},
                                  { 200}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    }

    case 229:{
      int targetIDList[9][9] = {
                                  { 228,224},
                                  { 227,223,219},
                                  { 226,222,218,214},
                                  { 225,221,217,213,209},
                                  { 220,216,212,208,204},
                                  { 215,211,207,203},
                                  { 210,206,202},
                                  { 205,201},
                                  { 200}
                                 };
      for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(INTERVAL);
      }
      break;
    } 
  }
}


