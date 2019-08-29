/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>

// Update these with values suitable for your network.

const char* ssid = "DoESLiverpool";
const char* password = "decafbad00";
const char* mqtt_server = "10.0.100.1";

const int kRedPin = 23;
const int kGreenPin = 3;
const int kBluePin = 18;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

unsigned long gLastMotionTime = 0UL;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    // Motion detected
    gLastMotionTime = millis();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "DoorWatcher";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("dinky/motion");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  Serial.println("Let's go");

  pinMode(kRedPin, OUTPUT);
  pinMode(kGreenPin, OUTPUT);
  pinMode(kBluePin, OUTPUT);
  digitalWrite(kRedPin, HIGH);
  digitalWrite(kGreenPin, HIGH);
  digitalWrite(kBluePin, HIGH);

  Serial.println("Red");
  digitalWrite(kRedPin, LOW);
  delay(300);
  digitalWrite(kRedPin, HIGH);
  Serial.println("Green");
  digitalWrite(kGreenPin, LOW);
  delay(300);
  digitalWrite(kGreenPin, HIGH);
  Serial.println("Blue");
  digitalWrite(kBluePin, LOW);
  delay(300);
  digitalWrite(kBluePin, HIGH);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Work out what to display with the light
  unsigned long timeSinceMotion = millis() - gLastMotionTime;
  Serial.println(timeSinceMotion);
  if (timeSinceMotion < 150*1000UL)
  {
    digitalWrite(kRedPin, LOW);
    digitalWrite(kGreenPin, HIGH);
    digitalWrite(kBluePin, HIGH);
  } 
  else if (timeSinceMotion < 300*1000UL)
  {
    digitalWrite(kRedPin, LOW);
    digitalWrite(kGreenPin, LOW);
    digitalWrite(kBluePin, HIGH);
  }
  else if (timeSinceMotion < 450*1000UL)
  {
    digitalWrite(kRedPin, HIGH);
    digitalWrite(kGreenPin, HIGH);
    digitalWrite(kBluePin, LOW);
  }
  else if (timeSinceMotion < 600*1000UL)
  {
    digitalWrite(kRedPin, HIGH);
    digitalWrite(kGreenPin, LOW);
    digitalWrite(kBluePin, HIGH);
  }
  else
  {
    digitalWrite(kRedPin, HIGH);
    digitalWrite(kGreenPin, HIGH);
    digitalWrite(kBluePin, HIGH);
  }

  delay(500);
}
