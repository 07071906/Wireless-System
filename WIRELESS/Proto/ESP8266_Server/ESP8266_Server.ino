
#include <ESP8266WiFi.h>

const char* ssid = "VERTEX RESEARCH CENTRE";
const char* password = "VERTxE!2023";

WiFiServer server(80);

IPAddress ip(192, 168, 0, 80);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

#define LED_PIN 2

#define CLIENT1_ID 0xC1
#define CLIENT2_ID 0xC2

#define REQUEST_DATA 0xF1
#define VALID_DATA   0xAA

#define TIMEOUT 5000

WiFiClient clients[2];
uint8_t clientID[2] = {0, 0};

uint8_t client1Data = 0;
uint8_t client2Data = 0;

bool feedbackPending = false;
bool requestSent = false;

unsigned long requestTime = 0;

void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED OFF (inverted logic)

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("Server IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  server.setNoDelay(true);

  Serial.println("SERVER READY");
}

void loop() {

  // Accept new clients
  WiFiClient newClient = server.accept();

  if (newClient) {

    bool assigned = false;

    for (int i = 0; i < 2; i++) {

      if (!clients[i] || !clients[i].connected()) {

        clients[i] = newClient;
        clientID[i] = 0;

        Serial.println("New client connected");

        assigned = true;
        break;
      }
    }

    if (!assigned) {
      newClient.stop();
    }
  }

  // Receive data from clients
  for (int i = 0; i < 2; i++) {

    if (clients[i] && clients[i].connected()) {

      while (clients[i].available()) {

        uint8_t data = clients[i].read();

        // Identify client
        if (clientID[i] == 0) {

          if (data == CLIENT1_ID || data == CLIENT2_ID) {

            clientID[i] = data;

            Serial.print("Client identified: ");
            Serial.println(data, HEX);
          }

          continue;
        }

        // Client 1 feedback
        if (clientID[i] == CLIENT1_ID) {

          client1Data = data;

          Serial.print("Client 1 Feedback: 0x");
          Serial.println(client1Data, HEX);

          // Start a new verification
          digitalWrite(LED_PIN, HIGH);

          client2Data = 0;
          feedbackPending = true;
          requestSent = false;

          requestTime = millis();
        }

        // Client 2 feedback
        else if (clientID[i] == CLIENT2_ID) {

          if (feedbackPending && requestSent) {

            client2Data = data;

            Serial.print("Client 2 Feedback: 0x");
            Serial.println(client2Data, HEX);

            feedbackPending = false;
            requestSent = false;

            // Compare both feedback values
            if (client1Data == VALID_DATA &&
                client2Data == VALID_DATA) {

              Serial.println("BOTH FEEDBACK MATCH");

              digitalWrite(LED_PIN, LOW); // LED ON
            }

            else {

              Serial.println("FEEDBACK NOT MATCHED");

              digitalWrite(LED_PIN, HIGH); // LED OFF
            }
          }
        }
      }
    }

    else if (clients[i]) {

      clients[i].stop();
      clientID[i] = 0;

      Serial.println("Client disconnected");
    }
  }

  // Request feedback from Client 2
  if (feedbackPending && !requestSent) {

    for (int i = 0; i < 2; i++) {

      if (clientID[i] == CLIENT2_ID &&
          clients[i].connected()) {

        clients[i].write(REQUEST_DATA);

        Serial.println("Request sent to Client 2");

        requestSent = true;
        break;
      }
    }
  }

  // Timeout check
  if (feedbackPending &&
      millis() - requestTime > TIMEOUT) {

    Serial.println("Client 2 feedback TIMEOUT");

    feedbackPending = false;
    requestSent = false;

    digitalWrite(LED_PIN, HIGH);
  }
}
