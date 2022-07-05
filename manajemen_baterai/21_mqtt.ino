// Mqtt - Ghani
// MQTT_MAX_PACKET_SIZE 1024

const char* mqttServer = "iot.tf.itb.ac.id";
const int mqttPort = 1883;
const char* clientID = "Esp32-13319037";

const char* chPublish = "Q-02/Battery";

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

void mqttSetup() {
  client.setServer(mqttServer, mqttPort); // Connect to MQTT
  client.setCallback(callback);
  Serial.print("MQTT connecting to ");
  Serial.print(mqttServer);
  Serial.print(":");
  Serial.println(mqttPort);

  if (!client.connect(clientID)) {
    Serial.println("failed !!!");
  }
}

void mqttLoop() {
  if (!client.connected()) {
    mqttSetup();
  }

  client.loop();
}
