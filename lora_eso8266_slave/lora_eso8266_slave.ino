
#include <LoRa.h>
#define SS 15
#define RST 16
#define DIO0 2
#define led_1 0

void setup() {
  pinMode(led_1, OUTPUT);
  digitalWrite(led_1, LOW);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Receiver Host");
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Error");
    while (1);
  }
}
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Receiving Data: ");
    while (LoRa.available()) {
      String data = LoRa.readString();
      Serial.println(data);
      if (data.indexOf("N") == 1 && data.indexOf("1") == 2 ) {
        Serial.println("led 1 goes ON");
        digitalWrite(led_1, HIGH); 
      }else if (data.indexOf("F") == 1 && data.indexOf("1") == 3 ) {
        Serial.println("led 1 goes OFF");
        digitalWrite(led_1, LOW);
      }
    }
  }
}
