/* For: Heavy Rain Project: https://tinkerplus.github.io
 *      Slave part of this project: send OSC message to others via UDP, within a local network.
 *      
 *      - if button pressed, 
 *        change the lamp's color; send to master the message: ledState colorCode sourceID
 *      - if receive message,
 *        change the lamp's color according to the message from master ledState colorCode sourceID
 * Team: TinkerPlus
 * Author: Can Jia
 * Date: 2018/4/4
 * Board : ESP8266 like NodeMCU or WeMos D1/ D1 Mini
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>

#define btnPin D3                 // D3 pin at WeMos Di mini
#define ledPin D4                 // D4 pin at WeMos Di mini BUILDIN_LED
#define IR_LED D2                 // D2 for IR


/* ******************************************************************************************
 * Arguments Area
 * ******************************************************************************************
 */

// wifi setting
char ssid[] = "PiFi";
char pass[] = "pifi@7002";

// WiFiUDP 
WiFiUDP Udp;

// Target ID list
int targetIDList[9][9] = {
                                  { 203, 209},
                                  { 202, 208, 214}, 
                                  { 201, 207, 213, 219},
                                  { 200, 206, 212, 218, 224},
                                  { 205, 211, 217, 223, 229},
                                  { 210, 216, 222, 228},
                                  { 215, 221, 227},
                                  { 220, 226},
                                  { 225}
                                 };
const unsigned int ID = 204;                      // Board ID
const unsigned int MASTER_ID = 199;               // Master ID
const unsigned int LISTEN_PORT = 8888;            // no need to change

// IR
IRsend irsend(IR_LED);

// color setting
const unsigned int COLOR_COUNTS = 7;
unsigned int colorList[COLOR_COUNTS] = {0xFF629D, 0xFF6897, 0xFF9867, 0xFFB04F, 0xFF5AA5, 0xFF42BD, 0xFF4AB5};
unsigned int colorCode = 0;

                       

/*  *****************************************************************************************
 *  Functions Area
 *  *****************************************************************************************
 */ 
 
void sendOSCTo(int targetID = 255, int ledState=0, unsigned int colorCode=0, int sourceID = 2);
void receiveOSC();
void colorEvent(OSCMessage &msg, int addrOffset);
unsigned int getRandomColorCode();
bool isBtnPressed();

/* ******************************************************************************************
 * Main Area
 * ******************************************************************************************
 */
void setup() {
  // ir
  irsend.begin();
  //wifi
  WiFi.softAPdisconnect(true);
  WiFi.config(IPAddress(192,168,1,ID),IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  
  Udp.begin(LISTEN_PORT);
  //Pin setting
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(btnPin, INPUT);
  digitalWrite(btnPin, HIGH);

  //for debug
  Serial.begin(115200);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
  // message address
 
}



void loop() {
  
  int ledState = 0;
  // if button pressed, send message
  if(isBtnPressed()){
    Serial.println("button pressed");
    digitalWrite(ledPin, LOW);              //open built-in led for debug
    ledState = 1;
    colorCode = getRandomColorCode();   // generate random color code
    
    Serial.print("color code: ");
    Serial.println(colorCode);
    unsigned color = colorList[colorCode];
    for (int i=0; i<3; i++){
      irsend.sendNEC(color, 32);              // set the value via ir, change the lamp's color 
      delay(50); 
    }

    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
          //void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID)
          for (int times=0; times<3; times++){
            sendOSCTo(targetIDList[i][j], ledState, colorCode, ID);  
            delay(10); 
          }
        }
        delay(500);
    }

   int size;
   while((size = Udp.parsePacket()) >0){
      while(size--)
        Udp.read();
   }
  //listen to master call
  }
  receiveOSC();
}



void sendOSCTo(int targetID, int ledState, unsigned int colorCode, int sourceID){
    // the message formate: ledState, colorCode, sourceID.    
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
  // listen to LISTEN_PORT, if get message, process it. then call colorEvent() funciton.
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
  
  int ledState = msg.getInt(0);
  Serial.print("led state: ");
  Serial.println(ledState);
  digitalWrite(ledPin, ledState);
  
  int colorCode = msg.getInt(1);
  Serial.print("color code: ");
  Serial.println(colorCode);
  for (int i=0; i<3; i++){
    irsend.sendNEC(colorList[colorCode], 32);  
  }
  
  int sourceID = msg.getInt(2);
  Serial.print("message from: ");
  Serial.println(sourceID);
}


unsigned int getRandomColorCode(){
  //unsigned int colorCode = 0; //red by default
  // random choose
  //colorCode = random(COLOR_COUNTS);
  
  unsigned int temColorCode = 0;
  do{
    temColorCode = random(COLOR_COUNTS);
  }while(temColorCode == colorCode);
  
  return temColorCode;
}

bool isBtnPressed(){
  if(digitalRead(btnPin) == 0){
    delay(100);
    if(digitalRead(btnPin) == 1){
      //digitalWrite(btnPin, HIGH);
      return 1;
    }
  }
  else return 0;
}
