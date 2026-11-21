/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
#include <SPI.h>
#include <ESP8266WiFi.h>


// Configuration parameters for Access Point
const char * ssid = "VERTEX RESEARCH CENTRE";
const char * password = "VERTxE!2023";
byte ledPin = D2;
//char ssid[] = "**********";               // SSID of your home WiFi
//char pass[] = "**********";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 0, 80);            // IP address of the server
IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

void setup() {
  Serial.begin(9600);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid,  password);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  pinMode(ledPin, OUTPUT);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: "); Serial.println(request);
      if(request=="ON")
      {
        digitalWrite(ledPin, HIGH);
        delay(30000);
        Serial.println(".");
      }
      client.flush();
      //client.println("Hi client! No, I am listening.\r"); // sends the answer to the client
      //digitalWrite(ledPin, HIGH);
    }
    client.stop();                // tarminates the connection with the client
  }
}
