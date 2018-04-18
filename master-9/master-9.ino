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

const unsigned int INTERVAL = 1000;


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
    case 217:{
      // light lamps around 100
//      int targetIDList[2][21] = {
//                                  { 101,106,107,102,112,108,113,114},
//                                  { 103,109,115,121,118,119,120,
//                                    104,110,116,122,128,127,126,
//                                    124,125,105,111,117,123,129 }
//                                 };
      int targetIDList[2][21] = {
                                  { 211, 212, 213, 216, 218, 221, 222, 223},
                                  { 205, 206, 207, 208, 209, 210, 214, 215, 219, 220, 224,
                                    225, 226, 227, 228, 229, 200, 201, 202, 203, 204}
                                 };
      for (int i=0; i<2; i++){
        for (int j=0; j<21; j++){
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
      // light lamps around 100
      int targetIDList[2][21] = {   
                                  { 212, 213, 214, 217, 218, 222, 223, 224},
                                  { 206, 207, 208, 209, 211, 216, 221, 226, 227, 228, 229,
                                    200, 201, 202, 203, 204, 205, 210, 215, 220, 225}
                                 };
      for (int i=0; i<2; i++){
        for (int j=0; j<21; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);
          delay(10); 
        }
        delay(INTERVAL);
      }
      break;
    }
    
    case 200:{
      // light lamps around 100
      int targetIDList[2][21] = {   
                                  { 201, 202, 205, 206, 207, 210, 211, 212},
                                  { 215, 216, 217, 218, 213, 208, 203, 204, 209, 214, 219,
                                    220, 221, 222, 223, 224, 225, 226, 217, 228, 229}
                                 };
      for (int i=0; i<2; i++){
        for (int j=0; j<21; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);
          delay(10); 
        }
        delay(INTERVAL);
      }
      break;
    }
  }
}


