
/*
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt

   Copyright (c) 2019 mobizt

   This example is for FirebaseESP32 Arduino library v 3.5.0 and later

*/

//This example shows how to read, store and update database using get, set, push and update functions.

#include <WiFi.h>
#include <FirebaseESP32.h>


#define FIREBASE_HOST "https://masdemo-146c2.firebaseio.com/" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "8ppozIBTOCqqRdKMJluxljP5Aw5EhPeKljCOBzKb"
#define WIFI_SSID "Dialog 4G"
#define WIFI_PASSWORD "121212qq"


//Define FirebaseESP32 data object
FirebaseData firebaseData;

FirebaseJson json;

void printResult(FirebaseData &data);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

String data_base = "batches";
//String batch_number = "batch_number";
int batch_number = 0;
//String roll_number = "roll_number";
//int roll_id = 1;
String width = "width";
String Tklen = "Tklen";
String plies = "plies";
String damage = "damage";
String joint = "joint";
String ends = "ends";
String del = "del";

void create_new_batch() {
  batch_number += 1;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number), 0);
}

void create_new_roll(int roll_id ) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id), 0);
}

void update_plies(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/number_of_plies", value);
}

void update_layer_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/layer_length", value);
}
void update_layer_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/layer_length", value);
}
void update_total_damage_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/total_damage_length", value);
}

void update_total_overlapped_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/total_overlapped_length", value);
}

void loop()
{
  Serial.println("running loop");
  String path = "/Test1";
  //Firebase.setDouble(firebaseData, path + "/Double2/Data1", 101);
  create_new_batch();
  create_new_roll(1);
  update_plies(1, 10);
  update_layer_length(1, 20);
  create_new_roll(2);
  update_plies(2, 100);
  delay(300);
}
