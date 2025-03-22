#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define RX 5 // Pin RX del ESP (D1)
#define TX 4 // Pin TX del ESP (D2)
SoftwareSerial espSerial(RX, TX);

const char* ssid = "FTFT";
const char* password = "cordoba2016";
const char* mqtt_server = "rabbitmq";
const char* topic_consumo = "expendedora1/consumo";
const char* topic_ok = "expendedora1/ok";
const char* topic_bahias = "expendedora1/bahia_vacia";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    espSerial.println(message);  // Enviar al Arduino
}

void reconnect() {
    while (!client.connected()) {
        if (client.connect("ESP8266Client")) {
            client.subscribe(topic_consumo);
        } else {
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    espSerial.begin(9600);
    WiFi.begin(ssid, password);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) reconnect();
    client.loop();
    
    if (espSerial.available()) {
        String bahia = espSerial.readStringUntil('\n');
        client.publish(topic_bahias, bahia.c_str());
    }
}
