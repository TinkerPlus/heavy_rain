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
    case 200:{
      int targetIDList[3][17] = {
                                  { 201, 205},
                                  { 202, 206, 210},
                                  { 203, 207, 211},
                                  { 204, 208, 212, 216, 220},
                                  { 209, 213, 217, 211, 225},
                                  { 214, 218, 222, 226},
                                  { 219, 223, 227},
                                  { 224, 226},
                                  { 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 200, 206,202},
                                  { 205, 211, 207, 203},
                                  { 210, 216, 212, 208, 204},
                                  { 215, 221, 217, 213, 209},
                                  { 220, 226, 222, 218, 214},
                                  { 225, 227, 223, 219},
                                  { 228, 224},
                                  { 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 201, 207, 203},
                                  { 200, 206, 212, 208, 204},
                                  { 205, 211, 217, 213, 209},
                                  { 210, 216, 222, 218, 214},
                                  { 215, 221, 227, 223, 219},
                                  { 220, 226, 228, 224},
                                  { 225, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 202, 204, 208},
                                  { 201, 207, 213, 209},
                                  { 200, 206, 212, 218, 214},
                                  { 205, 211, 217, 223, 219},
                                  { 210, 216, 222, 219},
                                  { 215, 221, 227, 229},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
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
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 200, 206, 210},
                                  { 201, 207, 211, 215},
                                  { 220, 216, 212, 208, 202},
                                  { 203, 209, 213, 217, 221, 225},
                                  { 226, 222, 218, 214, 204},
                                  { 219, 223, 227},
                                  { 228, 224},
                                  { 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 201, 205, 207, 211},
                                  { 200, 202, 208, 212, 216, 210},
                                  { 203, 209, 213, 217, 221, 215},
                                  { 220, 226, 222, 218, 214, 204},
                                  { 219, 223, 227, 225},
                                  { 228, 224},
                                  { 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 202, 208, 212, 206},
                                  { 203, 209, 213, 217, 211, 205, 201},
                                  { 200, 210, 216, 222, 218, 214, 204},
                                  { 215, 221, 227, 223, 219},
                                  { 220, 226, 228, 224},
                                  { 225, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 203, 207, 209, 213},
                                  { 202, 204, 214, 218, 212, 206},
                                  { 201, 205, 211, 217, 223, 219},
                                  { 200, 210, 216, 222, 228, 224},
                                  { 215, 221, 227, 229},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 204, 208, 214},
                                  { 203, 207, 213, 219},
                                  { 202, 206, 212, 218, 224},
                                  { 201, 205, 211, 217, 223, 229},
                                  { 200, 210, 216, 222, 228},
                                  { 215, 221, 227},
                                  { 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 205, 211, 215},
                                  { 200, 206, 212, 216, 220},
                                  { 201, 207, 213, 217, 221, 225},
                                  { 202, 208, 214, 218, 222, 226},
                                  { 203, 209, 219, 223, 227},
                                  { 204, 224, 228},
                                  { 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 206, 210, 212, 216},
                                  { 201, 205, 207, 213, 215, 217, 221},
                                  { 200, 202, 208, 214, 218, 222, 226, 220},
                                  { 203, 209, 219, 223, 227, 225},
                                  { 204, 224, 228},
                                  { 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 207, 213, 217, 211},
                                  { 202, 208, 214, 218, 222, 216, 210, 206},
                                  { 201, 203, 209, 219, 223, 227, 221, 215, 205},
                                  { 220, 204, 220, 226, 228, 224},},
                                  { 225, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 208, 212, 214, 218},
                                  { 203, 207, 209, 211, 214, 218},
                                  { 202, 204, 206, 210, 216, 222, 226, 224},
                                  { 201, 205, 215, 221, 227, 229},
                                  { 200, 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 209, 213, 219},
                                  { 204, 208, 212, 218, 224},
                                  { 203, 207, 211, 217, 223, 229},
                                  { 202, 206, 210, 216, 222, 228},
                                  { 201, 205, 215, 221, 227},
                                  { 200, 220, 226},
                                  { 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 210, 216, },
                                  { },
                                  { },
                                  { },
                                  { 204}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 210, 211, 212, 215, 217, 220, 221, 222},
                                  { },
                                  { 205, 206, 207, 208, 213, 218, 223, 225, 226, 227, 228},
                                  { },
                                  { 220, 201, 202, 203, 204, 209, 214, 219, 224, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 212, 213, 216, 218, 221, 222, 223},
                                  { },
                                  { 205, 206, 207, 208, 209, 210, 214, 215, 219, 220, 224, 225, 226, 227, 228, 229},
                                  { },
                                  { 220, 201, 202, 203, 204}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 212, 213, 214, 217, 218, 219, 222, 223, 224},
                                  { },
                                  { 206, 207, 208, 209, 211, 216, 221, 226, 227, 228},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 210, 215, 220, 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 212, 214, 217, 218, 222, 223, 224},
                                  { },
                                  { 206, 207, 208, 209, 211, 216, 221, 226, 227, 228, 229},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 210, 215, 220, 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 215, 216, 217, 221, 222, 225, 226, 227},
                                  { },
                                  { 210, 211, 212, 213, 218, 223, 228},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 214, 219, 224, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 215, 216, 217, 220, 222, 225, 226, 227},
                                  { },
                                  { 210, 211, 212, 213, 218, 223, 228},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 214, 219, 224, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 216, 217, 218, 221, 223, 226, 227, 228},
                                  { },
                                  { 210, 211, 212, 213, 214, 215, 219, 220, 224, 225, 229},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 217, 218, 219, 222, 224, 227, 228, 229},
                                  { },
                                  { 211, 212, 213, 214, 216, 221, 226},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 215, 220, 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 217, 218, 219, 222, 223, 227, 228, 229},
                                  { },
                                  { 211, 212, 213, 214, 216, 221, 226},
                                  { },
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 215, 220, 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 215, 216, 217, 220, 221, 222, 226, 227},
                                  { },
                                  { 210, 211, 212, 213, 218, 223, 228},
                                  {}
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 214, 219, 224, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 215, 216, 217, 220, 221, 222, 225, 227},
                                  { 210, 211, 212, 213, 218, 223, 228},
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 214, 219, 224, 229}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 216, 217, 218, 221, 222, 223, 226, 228},
                                  { 210, 211, 212, 213, 214, 215, 219, 220, 224, 225, 229},
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 217, 218, 219, 222, 223, 224, 227, 229},
                                  { 211, 212, 213, 214, 216, 221, 226},
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 215, 220, 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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
      int targetIDList[3][17] = {
                                  { 217, 218, 219, 222, 223, 224, 227, 228},
                                  { 211, 212, 213, 214, 216, 221, 226},
                                  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 215, 220, 225}
                                 };
      for (int i=0; i<3; i++){
        for (int j=0; j<17; j++){
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


