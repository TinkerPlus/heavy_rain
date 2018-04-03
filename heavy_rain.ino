#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>

char ssid[] = "PiFi";
char pass[] = "pifi@7002";

#define btnPin D3                 // D3 pin at WeMos Di mini
#define ledPin D4                 // D4 pin at WeMos Di mini BUILDIN_LED
#define IR_LED D2

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
IPAddress destIp(192,168,1,255);   // remote IP of the target device
const unsigned int destPort = 8888;    // remote port of the target device where the NodeMCU sends OSC to
const unsigned int localPort = 8888;   // local port to listen for UDP packets at the NodeMCU (another device must send OSC messages to this port)

IPAddress IPlist[3][2] = {
                              {IPAddress(192,168,1,216),IPAddress(192,168,1,218)},
                              {IPAddress(192,168,1,217)},
                              {IPAddress(192,168,1,238)}
                            };

void setup() {
  Serial.begin(115200);
  
  irsend.begin();
  
  WiFi.config(IPAddress(192,168,1,219),IPAddress(192,168,1,1), IPAddress(255,255,255,0)); 
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  //attachInterrupt(digitalPinToInterrupt(btnPin), btnIntCallBack, FALLING);
}

void btnIntCallBack(){
  delay(2000);
  Serial.println("Key Pressed");
  //irsend.sendNEC(0xFFB04F, 32);
  detachInterrupt(digitalPinToInterrupt(btnPin));
  digitalWrite(ledPin, LOW);
  //sendOSC();
  attachInterrupt(digitalPinToInterrupt(btnPin), btnIntCallBack, FALLING);
}
void sendOSCTo(int addr, int state){
    OSCMessage msgOut("/color");
    msgOut.add(state);
    // send via udp
    Udp.beginPacket(IPAddress(192,168,1,addr), 8888);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();
}
//void sendOSC(){
//    OSCMessage msgOut("/color");
//    msgOut.add(0);
//    // send via udp
//    Udp.beginPacket(IPAddress(192,168,1,217), 8888);
//    msgOut.send(Udp);
//    Udp.endPacket();
//    delay(1000);
//    Udp.beginPacket(IPAddress(192,168,1,216), 8888);
//    msgOut.send(Udp);
//    Udp.endPacket();
//    msgOut.empty();
//}
//void sendOSC(){
//    OSCMessage msgOut("/color");
//    msgOut.add(0);
//    // send via udp
//    for(int i=0; i<3; i++){
//      for(int j=0; j<2; j++){
//        Udp.beginPacket(IPlist[i][j], destPort);
//        msgOut.send(Udp);
//        Udp.endPacket();
//      }
//      delay(1000);
//    }
//    msgOut.empty();

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(btnPin) == 0){
    delay(100);
    if(digitalRead(btnPin) == 1){
       Serial.println("KeyPressed");
      digitalWrite(ledPin, LOW);
      delay(500);
      sendOSCTo(216,0);
      delay(500);
      sendOSCTo(216,1);
      sendOSCTo(217,0);
      delay(500);
      sendOSCTo(216,0);
      sendOSCTo(217,1);
      delay(500);
      sendOSCTo(216,1);
      sendOSCTo(217,0);
    }
  }
  receiveOSC();
}

//void sendOSC(){
//    OSCMessage msgOut("/color");
//    msgOut.add(0);
//    // send via udp
//    for(int i=0; i<3; i++){
//      for(int j=0; j<2; j++){
//        Udp.beginPacket(IPlist[i][j], destPort);
//        msgOut.send(Udp);
//        Udp.endPacket();
//      }
//      delay(1000);
//    }
//    msgOut.empty();
//}


void receiveOSC(){
  OSCMessage msgIN;
    int size;
    if((size = Udp.parsePacket())>0){
        while(size--)
            msgIN.fill(Udp.read());
        if(!msgIN.hasError()){
            msgIN.route("/color", receiveEvent);
        }
    }
}

void receiveEvent(OSCMessage &msg, int addrOffset){
  int ledState = msg.getInt(0);
  digitalWrite(ledPin, ledState);
  Serial.println("message receive!");
  Serial.println(ledState);
}
  






