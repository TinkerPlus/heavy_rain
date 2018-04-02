/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation216;
NetAddress myRemoteLocation217;

void setup() {
  size(400,400);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,9999);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation216 = new NetAddress("192.168.1.216",8888);
  myRemoteLocation217 = new NetAddress("192.168.1.217",8888);
}


void draw() {
  fill(255);
  rect(0,0,200,400);
}

void mousePressed() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/color");
  //OscMessage myMessage1 = new OscMessage("/color");
  
  if(mouseX < 200) myMessage.add(1); /* add an int to the osc message */
  else myMessage.add(0);
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation216); 
  oscP5.send(myMessage, myRemoteLocation217);
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}