#include <Arduino.h>
#include <WiFi.h>
#define USE_SERIAL Serial
#include <HTTPClient.h>
//const char* ssid     = "iPhone 8";
//const char* password = "featwithmeno1";
const char* ssid     = "Dialog 4G";
const char* password = "121212qq";

void connect_to_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void http_client(String d1, String d2) {
  HTTPClient http;

  USE_SERIAL.print("[HTTP] begin...\n");
  // configure traged server and url
  //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
  http.begin("http://192.168.8.103/mas.php?id=1&val_1=" + d1 + "&val_2=" + d2); //HTTP

  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      USE_SERIAL.println(payload);
    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(500);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connect_to_wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  http_client("100", "200");
  Serial.println("running loop");
  delay(300);

}
