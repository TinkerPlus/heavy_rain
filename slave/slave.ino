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
 
// ID
const unsigned int ID = 219;                      // Board ID
const unsigned int MASTER_ID = 238;               // Master ID
const unsigned int LISTEN_PORT = 8888;            // no need to change

// IR
IRsend irsend(IR_LED);

// color setting
const unsigned int COLOR_COUNTS = 7;
unsigned int colorList[COLOR_COUNTS] = {0xFFB04F, 0xFF30CF, 0xFF708F, 0xFFA857, 0xFF28D7, 0xFF6897, 0xFF9867};





// for non-master version
IPAddress IPlist[3][2] = {
                              {IPAddress(192,168,1,216),IPAddress(192,168,1,218)},
                              {IPAddress(192,168,1,217)},
                              {IPAddress(192,168,1,238)}
                           };
                            

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
  // if button pressed, send message
  if(isBtnPressed()){
    Serial.println("button pressed");
    digitalWrite(ledPin, LOW);              //open built-in led for debug
    int colorCode = getRandomColorCode();   // generate random color code
    Serial.print("color code: ");
    Serial.println(colorCode);
    unsigned color = colorList[colorCode];
    irsend.sendNEC(color, 32);              // set the value via ir, change the lamp's color
    
    sendOSCTo(MASTER_ID, 0, colorCode, ID);       // sendOSC to master
  }
  //listen to master call
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
  irsend.sendNEC(colorList[colorCode], 32);

  int sourceID = msg.getInt(2);
  Serial.print("message from: ");
  Serial.println(sourceID);
}


unsigned int getRandomColorCode(){
  int colorCode = 0; //red by default
  // random choose
  colorCode = random(COLOR_COUNTS);
  return colorCode;
}

bool isBtnPressed(){
  if(digitalRead(btnPin) == 0){
    delay(100);
    if(digitalRead(btnPin) == 1){
      return 1;
    }
  }
  else return 0;
}
