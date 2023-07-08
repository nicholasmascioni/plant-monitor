#define SENSOR 36
#define RED_LED 15
#define GREEN_LED 0

#define THRESHOLD 1700

#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = ""; // Network name
const char* password = ""; // Network password

String phoneNumber = "";  // +country code followed by number
String apiKey = ""; // CallMeBot API key
bool sentMessage = false;  // Save state of sent messages

// Reset state of all LEDs
void lightsOff() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

// Send a message over WhatsApp
void sendMessage(String message) {
  // Data to send
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.print("Message sent successfully");
  } else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SENSOR, INPUT);

  Serial.begin(115200);

  // Display WiFi connection status in serial monitor
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int moisture = analogRead(SENSOR);

  Serial.print("Soil moisture: ");
  Serial.println(moisture);

  if (moisture > THRESHOLD) {
    lightsOff();
    digitalWrite(RED_LED, HIGH);

    if (!sentMessage) { // Check if a message was already sent to avoid spamming messages
      sendMessage("Soil moisture is low! Water your plant!");
      sentMessage = true;
    }
  } else {
    lightsOff();
    digitalWrite(GREEN_LED, HIGH);
    sentMessage = false; // Reset sentMessage state to send another message later if necessary
  }
  delay(500);
}