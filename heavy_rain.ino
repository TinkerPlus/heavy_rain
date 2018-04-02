#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>

char ssid[] = "PiFi";
char pass[] = "pifi@7002";

const int btnPin = D3;                 // D3 pin at WeMos Di mini
const int ledPin = D4;                 // D4 pin at WeMos Di mini BUILDIN_LED

WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
IPAddress destIp(192,168,1,238);   // remote IP of the target device
const unsigned int destPort = 9999;    // remote port of the target device where the NodeMCU sends OSC to
const unsigned int localPort = 8888;   // local port to listen for UDP packets at the NodeMCU (another device must send OSC messages to this port)


void setup() {
  Serial.begin(115200);
  WiFi.config(IPAddress(192,168,1,218),IPAddress(192,168,1,1), IPAddress(255,255,255,0)); 
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

  pinMode(btnPin, INPUT);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  sendOSC();
  receiveOSC();
}

void sendTo(IPAddress targetIP, OSCMessage msg){
  Udp.beginPacket(targetIP,destPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void sendOSC(){
  // control the led
  int btnVal = digitalRead(btnPin);
  if (btnVal == 0){ //default btnPin is Pull-Up, the value is 1
    digitalWrite(ledPin, HIGH); // open the led
    // prepare the message
    OSCMessage msgOut("/color");
    msgOut.add(1);
    // send via udp
    Udp.beginPacket(destIp, destPort);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  
  delay(100);
}

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
  






