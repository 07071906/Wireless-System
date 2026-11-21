//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define LED D0

const char* ssid = "VERTEX RESEARCH CENTRE";
const char* password =  "VERTxE!2023";

//const char* mqtt_server = "test.mosquitto.org";
//const char* mqtt_server = "mqtt.eclipseprojects.io";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode(D1,INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(MQTTcallback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp/test");
}
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  if (message == "1") 
  {
    digitalWrite(LED, HIGH);
  }
  else if (message == "2") 
  {
    digitalWrite(LED, LOW);
  }
  Serial.println();
  Serial.println("-----------------------");
}
void loop() 
{
  if(digitalRead(D1)==0)
  {
  client.publish("esp/test1", "Hello from ESP8266");
  delay(1000);
  }
  else;
  client.loop();
}