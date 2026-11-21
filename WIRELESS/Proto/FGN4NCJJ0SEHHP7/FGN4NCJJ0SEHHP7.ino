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
byte ledPin = 2;
//char ssid[] = "**********";           // SSID of your home WiFi
//char pass[] = "*********";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
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
  client.println("This is my data\r");  // sends the message to the server
  buttonState = digitalRead(buttonPin);
 // String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println("from server: " + answer);
  client.flush();
  digitalWrite(ledPin, HIGH);
  delay(2000);                  // client will trigger the communication after two seconds
}
