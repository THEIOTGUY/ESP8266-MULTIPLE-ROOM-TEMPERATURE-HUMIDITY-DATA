#include <ESP8266WiFi.h>
#include <MQTT.h>
#include"DHT.h"
#define DHTTYPE DHT11
uint8_t DHTPin = D7;
const int relay =D5;
DHT dht(DHTPin,DHTTYPE);

float Temperature;
float Humidity;
char ch='°';


WiFiClient net;
MQTTClient client;

String ssid="Vaidande_2";
String pass="960647@vaidande";
unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("nodemcu_client")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  client.subscribe("nodemcu/Humidity1");
    client.subscribe("nodemcu/Temperature1");
    client.subscribe("nodemcu/Relay1");
 
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
    if(topic=="nodemcu/Relay1"){
        if(payload=="ON"){
     digitalWrite(relay,HIGH);
         
     Serial.print(payload);
            
        }
        }
    
    
   if(topic=="nodemcu/Relay1"){
        if(payload=="OFF"){
        digitalWrite(relay,LOW);
}
   
    }
    }
    
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
    pinMode(DHTPin, INPUT);
    pinMode(relay, OUTPUT);

  dht.begin();
  
    
   // attaches the servo on D8 to theservo object
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("192.168.137.152", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
    Temperature =dht.readTemperature();
  Serial.print(Temperature);
  Serial.print("°C");
  Serial.println(""); 
  Humidity =dht.readHumidity();
  Serial.print(Humidity);
  Serial.print("%");
  Serial.println(""); 
  
  
  client.loop();
  delay(200); // <- fixes some issues with WiFi stability
    client.publish("nodemcu/Temperature1",String(Temperature).c_str());
    delay(200);

    client.publish("nodemcu/Humidity1",String(Humidity).c_str());

  if (!client.connected()) {
    connect();
       }
    }
 
    
    
