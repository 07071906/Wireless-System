/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

// Configuration parameters for Access Point
const char * ssid = "VERTEX RESEARCH CENTRE";
const char * password = "VERTxE!2023";

const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 5;    // the number of the LED pin
//char ssid[] = "**********";           // SSID of your home WiFi
//char pass[] = "*********";            // password of your home WiFi


// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

unsigned long askTimer = 0;

IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
    // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  WiFi.begin(ssid, password );             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  pinMode(ledPin, OUTPUT);
}

void loop () {
  client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  Serial.println(".");
   buttonState = digitalRead(buttonPin);
     // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    client.println("ON\r");  // sends the message to the server
  } else {
    // turn LED off:
    client.println("OFF\r");  // sends the message to the server
  }
  client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
  String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println("from server: " + answer);
  client.flush();
  digitalWrite(ledPin, HIGH);
  delay(2000);                  // client will trigger the communication after two seconds
}
