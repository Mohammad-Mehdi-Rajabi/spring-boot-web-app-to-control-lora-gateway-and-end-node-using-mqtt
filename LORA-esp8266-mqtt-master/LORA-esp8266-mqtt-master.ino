#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LoRa.h>
// LORA
#define SS 15
#define RST 16
#define DIO0 2

// Wi-Fi credentials
const char* ssid = "POCO M3";
const char* password = "123456789";

// MQTT broker details
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttTopic = "SINARAZIPRJ";
const char* CLIENT_ID = "client12qw";

// Create an instance of WiFiClient
WiFiClient espClient;
// Create an instance of PubSubClient
PubSubClient client(espClient);

// MQTT callback function
void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT message
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  char* pl = new char[length];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    pl[i]=(char)payload[i];
  }
  Serial.println();
  Serial.print("Sending Data: ");
  Serial.println(pl);
  LoRa.beginPacket();
  LoRa.print(pl);
  LoRa.endPacket();
  delay(3000);
  delete[] pl;
}

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WiFi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Set the MQTT server and port
  client.setServer(mqttServer, mqttPort);
  // Set the MQTT callback function
  client.setCallback(callback);

  // Connect to MQTT broker
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect(CLIENT_ID)) {
      Serial.println("Connected to MQTT broker");
      // Subscribe to the MQTT topic
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
  while (!Serial);
  Serial.println("Sender Host");
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Error");
    delay(100);
    while (1);
  }
}

void loop() {
  // Maintain the MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// Reconnect to the MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Reconnecting to MQTT broker...");
    if (client.connect(CLIENT_ID)) {
      Serial.println("Connected to MQTT broker");
      // Subscribe to the MQTT topic
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
