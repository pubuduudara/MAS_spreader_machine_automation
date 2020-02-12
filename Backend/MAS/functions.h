#include <Keypad.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
FirebaseData firebaseData;
FirebaseJson json;

#define FIREBASE_HOST "https://masdemo-146c2.firebaseio.com/" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "8ppozIBTOCqqRdKMJluxljP5Aw5EhPeKljCOBzKb"

#define WIFI_SSID "Eng-Student"
#define WIFI_PASSWORD "3nG5tuDt"



//##############################
String data_base = "MAS";
int total_plies_count = 0;
int batch_number = 0;

float batch_overlap_length = 0;
float batch_damaged_length = 0;
float batch_used_length = 0;
float batch_end_length = 0;


void create_new_batch() {
  batch_number += 1;
  //roll_id = 0;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number), 0);
}

void user_input_layer_length(float value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/user_input_layer_length", value);
}

void user_input_no_of_plies(int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/user_input_number_of_plies", value);
}

void user_input_no_of_rolls(int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/user_input_no_of_rolls", value);
}

void upload_cycle_time(float value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/batch_cycle_time", value);
}

void update_total_plies(int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/batch_total_plies", total_plies_count);
}

void update_plies(int roll_id, int value) {
  total_plies_count += 1;
  update_total_plies(total_plies_count);
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/plies", value);
}

void update_on_track(bool stat) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/on_track", stat);
}

void total_used_length(float value) {
  batch_used_length += value;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/batch_used_length", batch_used_length);
}

void update_used_length(int roll_id, float value) {
  total_used_length(value);
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/used_length", value);
}

void total_end_length(float value) {
  batch_end_length += value;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/batch_end_length", batch_end_length);
}

void update_end_length(int roll_id, float value) {
  total_end_length(value);
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/end_length", value);
}



void create_new_roll(int roll_id) {
  //roll_id+=1;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id), 0);
}

void update_tktlen(int roll_id, float value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/tktlen", value);
}

void total_overlap_length(float value) {
  batch_overlap_length += value;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/batch_overlap_length", batch_overlap_length);
}

void update_overlap_length(int roll_id, float value) {
  total_overlap_length(value);
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/overlap_length", value);
}

void total_damaged_lenght(float value) {
  batch_damaged_length += value;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/batch_damaged_length", batch_damaged_length);
}

void update_damage_length(int roll_id, float value) {
  total_damaged_lenght(value);
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/damage_length", value);
}

void update_comment(int roll_id, float value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/comment", value);
}
