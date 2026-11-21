
#include <WiFi.h>

const char* ssid = "VERTEX RESEARCH CENTRE";
const char* password = "VERTxE!2023";

const char* serverIP = "192.168.0.80";
#define BUTTON_PIN 4

#define CLIENT2_ID 0xC2

#define REQUEST_DATA 0xF1

// Client 2 feedback
#define FEEDBACK_DATA 0xAA

WiFiClient client;

void connectServer() {

  while (!client.connected()) {

    client.stop();

    Serial.println("Connecting to Server...");

    if (client.connect(serverIP, 80)) {

      Serial.println("Connected to Server");

      // Identify as Client 2
      client.write((uint8_t)CLIENT2_ID);
    }

    delay(1000);
  }
}

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  connectServer();
}

void loop() {

  if (!client.connected()) {

    connectServer();
  }

  // Wait for server request
  while (client.available()) {

    uint8_t receivedData = client.read();

    if (receivedData == REQUEST_DATA) {

      Serial.println("Feedback request received");

  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press
  if (lastButtonState == HIGH && buttonState == LOW) {

      // Send feedback to server
      client.write((uint8_t)FEEDBACK_DATA);

      Serial.println("Feedback sent: 0xAA");
    }
  }

  delay(10);
}
