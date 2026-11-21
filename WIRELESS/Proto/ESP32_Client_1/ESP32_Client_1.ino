
#include <WiFi.h>

const char* ssid = "VERTEX RESEARCH CENTRE";
const char* password = "VERTxE!2023";

const char* serverIP = "192.168.0.80";

#define BUTTON_PIN 4

#define CLIENT1_ID 0xC1
#define FEEDBACK_DATA 0xAA

WiFiClient client;

bool lastButtonState = HIGH;

void connectServer() {

  while (!client.connected()) {

    client.stop();

    Serial.println("Connecting to Server...");

    if (client.connect(serverIP, 80)) {

      Serial.println("Connected to Server");

      // Identify as Client 1
      client.write((uint8_t)CLIENT1_ID);
    }

    delay(1000);
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

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

  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press
  if (lastButtonState == HIGH && buttonState == LOW) {

    client.write((uint8_t)FEEDBACK_DATA);

    Serial.println("Feedback sent: 0xAA");
  }

  lastButtonState = buttonState;

  delay(30);
}
