#include <DHT.h>  // Including library for DHT
#include <ESP8266WiFi.h>

String apiKey = "BUS6CPAUTXEYSJZG";     // Enter your Write API key from ThingSpeak
const char *ssid =  "Redmi Note 10S";     // replace with your wifi ssid and wpa2 key
const char *pass =  "chandraak123";
const char* server = "api.thingspeak.com";

#define DHTPIN D2          // Pin where the DHT11 is connected

DHT dht(DHTPIN, DHT11); // Menggunakan DHT11

WiFiClient client;

void setup() 
{
    Serial.begin(115200);
    delay(10);
    dht.begin();

    Serial.println("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void loop() 
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int potValue = analogRead(A0);  // Read value from potentiometer

    if (isnan(h) || isnan(t)) 
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    if (client.connect(server, 80)) 
    {  
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(t);
        postStr += "&field2=";
        postStr += String(h);
        postStr += "&field3=";
        postStr += String(potValue);  // Add potentiometer value as field3
        postStr += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);

        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print(" °C, Humidity: ");
        Serial.print(h);
        Serial.print("%, Potentiometer: ");
        Serial.print(potValue);
        Serial.println(" sent to ThingSpeak.");
    }
    client.stop();

    Serial.println("Waiting...");

    // ThingSpeak needs minimum 15 sec delay between updates
    delay(1000);
}