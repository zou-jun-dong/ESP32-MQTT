#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>

WiFiClient espClient;  //Creat a Wi-Fi client
PubSubClient client(espClient);  //Pass the WiFi client to the MQTT client

const char* mqtt_server="broker.emqx.io";  //Free public test broker provided by EMQX
const int mqtt_port=1883;   //Default non-encrypted port for MQTT

//Initialize WiFi
void setup_wifi(){
	WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
	Serial.print("Connecting to");
	Serial.println(WIFI_SSID);
	while (!WiFi.isConnected())
	{
		delay(500);
		Serial.print("."); 
	}
	Serial.println("");
	Serial.println("WiFi connected!");
	Serial.print("WiFI address:");
	Serial.println(WiFi.localIP());
}

void reconnect() {
	while (!client.connected())
	{
		Serial.println("Attempting MQTT connection...");
		//Create a random client ID
		String clientId="ESP32Client-";
		clientId+=String(random(0xffff),HEX);
		if (client.connect(clientId.c_str()))
		{
			/* code */
			Serial.println("connected!");
		}
		else{
            Serial.print("failed,rc=");
			Serial.print(client.state());
			Serial.println("try again 5 seconds");
			delay(5000);
		}
		
	}
	
}

void setup(){
	Serial.begin(115200);  
	setup_wifi();  //Start Wi-Fi
    client.setServer(mqtt_server,mqtt_port);  //Set the MQTT broker address and port
}

void loop(){
	if (!client.connected())
	{
		reconnect();
	}
	client.loop();  //Maintain MQTT client heartbeat and network processing
}