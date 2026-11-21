#include <SPI.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D7, D8); // RX, TX

unsigned char data[4] = {};
float distance;

// WiFi credentials
const char *ssid = "VERTEX RESEARCH CENTRE";
const char *password = "VERTxE!2023";

byte ledPin = D2;

IPAddress server(192,168,0,80);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("\nWiFi Connected");
  pinMode(ledPin, OUTPUT);
}

void loop() {

  // -------- SENSOR READ --------
  if (mySerial.available() >= 4) {

    for (int i = 0; i < 4; i++) {
      data[i] = mySerial.read();
    }

    if (data[0] == 0xFF) {

      int sum = (data[0] + data[1] + data[2]) & 0x00FF;

      if (sum == data[3]) {

        distance = (data[1] << 8) + data[2];
        float cm = distance / 10.0;

        Serial.print("Distance: ");
        Serial.print(cm);
        Serial.println(" cm");

        // -------- SERVER COMM --------
        if (client.connect(server, 80)) {

          digitalWrite(ledPin, LOW);
          Serial.println("Connected to server");

         // if (cm > 20 && cm < 30) {
          if (cm < 20 ) {
            client.println("ON");
            Serial.println("Sent: ON");
          } 
          if (cm >60  ){
            client.println("OFF");
            Serial.println("Sent: OFF");
          }

          client.stop();  // VERY IMPORTANT
          digitalWrite(ledPin, HIGH);

        } else {
          Serial.println("Server connection FAILED");
        }

      } else {
        Serial.println("Checksum ERROR");
      }
    }
  }

  //delay(100);
}