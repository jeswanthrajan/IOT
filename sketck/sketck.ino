#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "*****"; //your SSID
const char* password = "*****"; //your password 

const String apiKey = "*****";  // ThingSpeak Read API Key
const String channelID = "****"; // ThingSpeak Channel ID
const String fieldNumber = "**";             // We're reading field1

const int relay1 = 5; // GPIO pin connected to Relay 1
const int relay2 = 18; // GPIO pin connected to Relay 2

void setup() {
  Serial.begin(115200);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  // Start with both relays off
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/channels/" + channelID + "/fields/" + fieldNumber + "/last.txt";

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.print("ThingSpeak value: ");
      Serial.println(payload);

      int value = payload.toInt();

      switch (value) {
        case 1:
          digitalWrite(relay1, LOW);  // we are turning on light
          Serial.println("Relay 1 ON");
          break;
        case 2:
          digitalWrite(relay1, HIGH); // turning off the light
          Serial.println("Relay 1 OFF");
          break;
        case 3:
          digitalWrite(relay2, LOW);  // turning on the fan
          Serial.println("Relay 2 ON");
          break;
        case 4:
          digitalWrite(relay2, HIGH); // turning off the fan
          Serial.println("Relay 2 OFF");
          break;
        default:
          Serial.println("Invalid field1 value.");
          break;
      }
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpCode);
    }

    http.end();
  }

  delay(500); // Wait 10 seconds before next read
}