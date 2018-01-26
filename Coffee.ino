#include <CallbackFunction.h>
#include <Switch.h>
#include <UpnpBroadcastResponder.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "config.h"

// prototypes
boolean connectWifi();

//on/off callbacks
// void officeLightsOn(); // TODO Delete me
// void officeLightsOff(); // TODO Delete me
void coffeeMachineOn();
void coffeeMachineOff();

// Change this before you flash
//#######################################
const char* ssid = "WIFI_SSID"; //enter your access point/wifi router name
const char* password = "WIFI_PASSWORD"; //enter router password
// change gpio pins as you need it.
//I am using ESP8266 EPS-12E GPIO16 and GPIO14
// const int relayPin1 = 16; //TODO Delete me
const int relayPin2 = 14;

//#######################################
boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

// Switch *office = NULL; //TODO Delete me
Switch *coffeeMachine = NULL;


void setup()
{
  Serial.begin(115200);
   // pinMode(relayPin1, OUTPUT); // TODO Delete me
   pinMode(relayPin2, OUTPUT);
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    // office = new Switch("office lights", 80, officeLightsOn, officeLightsOff); //TODO Delete
    coffeeMachine = new Switch("coffee machine", 81, coffeeMachineOn, coffeeMachineOff);

    Serial.println("Adding switches upnp broadcast responder");
    // upnpBroadcastResponder.addDevice(*office); //TODO Delete me
    upnpBroadcastResponder.addDevice(*coffeeMachine);
  }
}

void loop()
{
   if(wifiConnected){
      upnpBroadcastResponder.serverLoop();

      coffeeMachine->serverLoop();
      // office->serverLoop(); //TODO Delete me
   }
}

//TODO Delete
/*
void officeLightsOn() {
    Serial.print("Switch 1 turn on ...");
    digitalWrite(relayPin1, HIGH);
}

void officeLightsOff() {
    Serial.print("Switch 1 turn off ...");
    digitalWrite(relayPin1, LOW);
}
*/

void coffeeMachineOn() {
    Serial.print("Switch 2 turn on ...");
    digitalWrite(relayPin2, HIGH);
    delay(500);
    digitalWrite(relayPin2, LOW);
}

void coffeeMachineOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(relayPin2, LOW);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }

  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
