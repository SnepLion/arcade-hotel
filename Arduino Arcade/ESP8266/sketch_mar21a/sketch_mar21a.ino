#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "FTFT";
const char* password = "cordoba2016";
const char* mqtt_server = "192.168.100.88";
const char* topic_consumo = "expendedora1/consumo";
const char* topic_ok = "expendedora1/ok";
const char* topic_bahias = "expendedora1/bahia_vacia";
const char* mqtt_user = "admin";  // Usuario RabbitMQ
const char* mqtt_password = "admin";  // Contraseña

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);  // Enviar al Arduino
}

void reconnect() {
    while (!client.connected()) {
        if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
            client.subscribe(topic_consumo);
        } else {
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    Serial.print("conectado a ssid: ");
    Serial.print(ssid);
    Serial.print(" Password: ");
    Serial.println(password);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) reconnect();
    client.loop();
    
    if (Serial.available()) {
        String bahia = Serial.readStringUntil('\n');
        client.publish(topic_bahias, bahia.c_str());
    }
}
