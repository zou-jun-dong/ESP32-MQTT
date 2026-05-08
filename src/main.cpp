#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>
#include <ArduinoJson.h> //Add JSON library

WiFiClient espClient;  //Creat a Wi-Fi client
PubSubClient client(espClient);  //Pass the WiFi client to the MQTT client

const char* mqtt_server="broker.emqx.io";  //Free public test broker provided by EMQX
const int mqtt_port=1883;   //Default non-encrypted port for MQTT
const char* led_topic="foshan/ESP32/ledControl";
const char* sensor_topic="foshan/ESP32/sensorData";  //Publish topic
const int ledPin=2;

unsigned long lastMsgTime=0;  //Time tracking variable
unsigned long lastWiFiRetryTime=0;
unsigned long lastMqttRetryTime=0;
const unsigned long retryInterval=5000;

//Initialize WiFi
void setup_wifi(){
	if (WiFi.isConnected())
	{
		return;
	}
	Serial.print("Connecting to");
	Serial.println(WIFI_SSID);
	WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
}

void reconnect() {
	while (WiFi.isConnected()&&!client.connected())
	{
		Serial.println("Attempting MQTT connection...");
		//Create a random client ID
		String clientId="ESP32Client-";
		clientId+=String(random(0xffff),HEX);
		if (client.connect(clientId.c_str()))
		{
			/* code */
			Serial.println("connected!");
			client.subscribe(led_topic);
		}
		else{
            Serial.print("failed,rc=");
			Serial.print(client.state());
			Serial.println("try again 5 seconds");
			delay(5000);
		}
		
	}
	
}

void callback(char* topic,byte* palyload,unsigned int length) {
	String message;
	for (int i = 0; i < length; i++)
	{
		message+=(char)palyload[i];
	}
	Serial.println(message);
	if (message=="ON")
	{
		digitalWrite(ledPin,HIGH);
		Serial.println("LED is now ON");
	}else if (message=="OFF")
	{
		digitalWrite(ledPin,LOW);
		Serial.println("LED is now OFF");
	}
	
	
}

void setup(){
	Serial.begin(115200);  
	pinMode(ledPin,OUTPUT);
	setup_wifi();  //Start Wi-Fi
    client.setServer(mqtt_server,mqtt_port);  //Set the MQTT broker address and port
	client.setCallback(callback);
}

void loop(){
	unsigned long now=millis();  //Get current uptime in milliseconds
  if(!WiFi.isConnected())
  {
	if (now-lastWiFiRetryTime>=retryInterval)
	{
		lastWiFiRetryTime=now;
		Serial.println("[Network] Wi-Fi connection lost. Attempting to reconnect...");
		WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
	}
}
else if (!client.connected())
{
	if (now-lastMqttRetryTime>=retryInterval)
	{
		lastMqttRetryTime=now;
		reconnect();
	}
	
}
else
{  client.loop();
   if (now-lastMsgTime>5000)
  {
	lastMsgTime=now;
	float temp=random(200,350)/10;  //Generate a random float between 20.0 and 35.0 to simulate temperature
	//Convert float to C++ String
    JsonDocument doc;  //Instantiate a JSON document object

	//Populate data like a dictionary 
	doc["device"]="ESP32-S3";
	doc["temperrature"]=temp;
	doc["status"]="running";
	String JsonString;  //Used to store the final generated string

	//Serialize:Convert structured data in doc into text and put it into jsonString
	serializeJson(doc,JsonString);
	Serial.print("Publishing JSON:");
	Serial.println(JsonString);
	client.publish(sensor_topic,JsonString.c_str());}   //Publish the packet with JSON fortmat to the cloud
}

  
}