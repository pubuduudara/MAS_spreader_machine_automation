
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
//#define WIFI_SSID "Dialog 4G"
//#define WIFI_PASSWORD "121212qq"
//
#define WIFI_SSID "Eng-Student"
#define WIFI_PASSWORD "3nG5tuDt"

//#define WIFI_SSID "RajeevAP"
//#define WIFI_PASSWORD "rajeev14"

//Define FirebaseESP32 data object
FirebaseData firebaseData;

FirebaseJson json;


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
  create_new_batch();
}



//##############################
String data_base = "MAS";
int total_plies = 0;
//int roll_id = 0;
int batch_number = 0;
void create_new_batch() {
  batch_number += 1;
  //roll_id = 0;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number), 0);
}

void user_input_no_of_plies(int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/user_input_number_of_plies", value);
}

void user_input_layer_length(int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/user_input_layer_length", value);
}

void user_input_number_of_rolls(int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/user_input_number_of_rolls", value);
}

void update_total_plies() {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/updated_total_plies", total_plies);
}

void create_new_roll(int roll_id) {
  //roll_id+=1;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id), 0);
}

void update_tktlen(int roll_id,int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/tktlen", value);
}

void update_plies(int roll_id,int value) {
  total_plies += value;
  update_total_plies();
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/plies", value);
}

void update_total_damage_length(int roll_id,int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/total_damage_length", value);
}

void update_total_overlapped_length(int roll_id,int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/total_overlapped_length", value);
}

void update_total_used_length(int roll_id,int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/update_total_used_length", value);
}

void update_ends(int roll_id,int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/ends", value);
}

void update_on_track(int roll_id,bool stat) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/on_track", stat);
}
//######################################

void loop()
{
  Serial.println("running loop");

  //Firebase.setDouble(firebaseData, path + "/Double2/Data1", 101);

  user_input_no_of_plies(50);
  user_input_layer_length(200);
  user_input_number_of_rolls(30);
  create_new_roll();
  update_tktlen(20);
//  update_plies(30);
//  update_total_damage_length(210);
//  update_total_overlapped_length(180);
//  update_total_used_length(90);
//  update_ends(2);
//  update_on_track(true);
  create_new_roll();
  update_tktlen(20);
  delay(300);
}
