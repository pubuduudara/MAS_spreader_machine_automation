#include <Keypad.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://masdemo-146c2.firebaseio.com/" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "8ppozIBTOCqqRdKMJluxljP5Aw5EhPeKljCOBzKb"

#define WIFI_SSID "Eng-Student"
#define WIFI_PASSWORD "3nG5tuDt"

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;


//Keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {15, 2, 4, 16};
byte colPins[COLS] = {17, 5, 18, 19};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//Display
const int rs = 13, en = 14, d4 = 27, d5 = 26, d6 = 25, d7 = 33;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//constants
float pi = 3.14;
int N = 24;
float R = 4.1; //in cm

// Declare encoder pins
int dir_pin_1 = 32;
int dir_pin_2 = 35;
int dis_pin = 34;

float t1 = millis();

int last = 0;
int count = 0;
float distance = 0;
int last_plies = 0;
int lastA = 0;
int dir = 1;

//variables
char text[10]; //for taking input as a string
int number_of_plies, plies = 0;
int number_of_rolls, roll_id = 0, rolls = 0;
int tkt_len = 0;

float layer_length;
float damage_length = 0;
float overlap_length = 0;
float used_length = 0;
float end_length = 0;

bool is_half_ply = false;
bool is_damage = false;
bool is_roll_finished = false;
bool is_on_track = false;
bool is_new_roll = true;
bool is_end = false;
bool status_running = false;

void setup() {
  Serial.begin(115200);
  wifi_connect();

  pinMode(dir_pin_1, INPUT);
  pinMode(dir_pin_2, INPUT);
  pinMode(dis_pin, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

}

void loop() {
  // Get user inputs
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Start laying?");
  waitForInput('#');

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Lay length(cm):");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%f", &layer_length);

  //******************************************************
  user_input_layer_length(layer_length);
  //******************************************************

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("No of plies:");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%d", &number_of_plies);

  //******************************************************
  user_input_no_of_plies(number_of_plies);
  //******************************************************

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("No of rolls:");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%d", &number_of_rolls);

  //******************************************************
  user_input_no_of_rolls(number_of_rolls);
  //******************************************************

  lcd.clear();
  lcd.setCursor(0, 0);

  // End: Get user inputs

  //Calculating Cycle time in seconds
  float CT = (247 + number_of_rolls * 9 + number_of_rolls * 70 + layer_length * number_of_plies * getSpeedPerMeter(layer_length) + 120 * number_of_rolls) * 60 * 5 / 6;

  //******************************************************
  upload_cycle_time(CT);
  //******************************************************

  t1 = millis();

  // Start measuring
  last = 0;
  count = 0;
  distance = 0;
  plies = 0;
  last_plies = 0;
  lastA = 0;
  dir = 1;

  is_half_ply = false;
  is_damage = false;
  is_roll_finished = false;
  is_end = false;
  is_new_roll = false;
  is_on_track = false;
  status_running = false;

  // Variables to submit
  damage_length = 0;
  overlap_length = 0;
  used_length = 0;
  end_length = 0;

  while (true) {



    //When number of plies changed, submit data to the server
    if (last_plies != plies) {
      //On track/Offtrack calculation
      float req_time = plies * CT / number_of_plies;
      float t2 = millis();

      float curr_time = (t2 - t1) / 1000;

      if (curr_time > req_time) {
        //Off track
        is_on_track = false;
      } else {
        //On track
        is_on_track = true;
      }
      last_plies = plies;

      //******************************************************
      update_plies(plies);
      update_on_track(is_on_track);
      //******************************************************

    }


    // Check user input
    char customKey = customKeypad.getKey();
    if (customKey == 'A' && !is_half_ply) {
      //half ply
      is_half_ply = true;

    } else if (customKey == 'B' && !is_damage) {
      //Damage
      is_damage = true;

    } else if (customKey == 'C') {
      //Roll finished
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Roll finished?");
      while (1) {
        char key = getInputChar();
        if (key == '#') { //continue
          is_roll_finished = true;
          break;
        } else if (key == '*') { //cancel
          is_roll_finished = false;
          lcd.clear();
          break;
        }
      }

    } else if (customKey == 'D') {
      //End
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("End?");
      while (1) {
        char key = getInputChar();
        if (key == '#') { //continue
          is_end = true;
          break;
        } else if (key == '*') { //cancel
          is_end = false;
          lcd.clear();
          break;
        }
      }
    }

    if (is_roll_finished) {
      rolls++;
      used_length = distance;

      //Submit data*****************************************
      update_used_length(used_length);
      //****************************************************

      is_roll_finished = false;
      is_new_roll = true;

      plies = 0;

    }

    if (is_end) {
      rolls++;


      lcd.clear();
      lcd.setCursor(0, 0);

      lcd.print("End length(cm):");
      lcd.setCursor(0, 1);
      getInputString('#', text);
      sscanf(text, "%f", &end_length);

      lcd.clear();
      lcd.setCursor(0, 0);

      float comment = tkt_len - (no_of_plies * lay_length) + overlap_length + damage_length + used_length + end_length;

      //Submit data*****************************************
      update_end_length(end_length);
      update_comnt(comnt);
      //****************************************************

      is_new_roll = true;
      is_end = false;
      plies = 0;

    }

    if (is_new_roll) {
      lcd.clear();
      lcd.setCursor(0, 0);

      lcd.print("Roll id:");
      lcd.setCursor(0, 1);
      getInputString('#', text);
      sscanf(text, "%d", &roll_id);

      lcd.clear();
      lcd.setCursor(0, 0);

      lcd.print("Tktlen(cm):");
      lcd.setCursor(0, 1);
      getInputString('#', text);
      sscanf(text, "%d", &tkt_len);

      is_new_roll = false;

      //************************************************
      create_new_roll(roll_id);
      update_tktlen(tkt_len);
      //************************************************

      lcd.clear();
      lcd.setCursor(0, 0);
    }

    if (plies >= number_of_plies) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Done !");
      delay(1000);

      //Submit data*****************************************
      //uploadData();
      //****************************************************

      lcd.clear();
      lcd.setCursor(0, 0);
      break;
    }


    if (!is_half_ply && !is_damage) {
      int a = digitalRead(dir_pin_1);
      int b = digitalRead(dir_pin_2);
      int now = digitalRead(dis_pin);

      // Direction
      if (a != lastA) { //When first signal changes
        if (b == a) { //If second signal has already changed
          dir = 1; // Forward direction
        } else {
          dir = -1; // Backward direction
        }
        lastA = a;
      }
      // Distance
      if (now != last && dir == 1) { // Rotating
        if (!status_running) {
          status_running = true;

        }
        count++;
        distance += pi * R / N;
        if (distance >= layer_length) {
          plies++;
          distance = 0;
        }
        last = now;
      }

      lcd.setCursor(0, 0);
      lcd.printf("L:");
      lcd.print(distance);
      lcd.printf("cm");

      lcd.setCursor(0, 1);
      lcd.printf("P:");
      lcd.print(plies);

      continue;
    }

    // Calculations
    if (is_half_ply) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Start overlap?");
      waitForInput('#');
      lcd.clear();

      float dis_with_overlap = 0;
      // Overlap
      while (1) {
        int a = digitalRead(dir_pin_1);
        int b = digitalRead(dir_pin_2);
        int now = digitalRead(dis_pin);

        // Direction
        if (a != lastA) { //When first signal changes
          if (b == a) { //If second signal has already changed
            dir = 1; // Forward direction
          } else {
            dir = -1; // Backward direction
          }
          lastA = a;
        }
        // Distance
        if (now != last && dir == 1) { // Rotating
          count++;
          dis_with_overlap += pi * R / N;
          last = now;
        }

        lcd.setCursor(0, 0);
        lcd.printf("Overlap L:");
        lcd.print(dis_with_overlap);
        lcd.printf("cm");
        lcd.setCursor(0, 1);
        lcd.printf("Hold # if done");

        char key = customKeypad.getKey();
        if (key == '#') {
          break;
        }
      }

      //*********************************************************
      overlap_length = distance + dis_with_overlap - layer_length;
      update_overlap_length(overlap_length);
      //*********************************************************

      plies++;
      distance = 0;
      lcd.clear();

      is_half_ply = false;

    } else if (is_damage) {
      lcd.clear();
      damage_length = distance;
      distance = 0;
      lcd.setCursor(0, 0);

      //**************************************************************
      update_damage_length(damage_length);
      //**************************************************************

      lcd.print("Continue laying?");
      waitForInput('#');
      lcd.clear();
      is_damage = false;
    }


  }
}

void waitForInput(char x) {
  while (1) {
    char customKey = customKeypad.getKey();
    if (customKey == x) {
      break;
    }
  }
}

char getInputChar() {
  while (1) {
    char customKey = customKeypad.getKey();
    if (customKey) {
      return customKey;
    }
  }
}

void getInputString(char delimiter, char str[10]) {

  int i = 0;
  while (1) {
    char customKey = customKeypad.getKey();

    if (customKey == delimiter || i == 10) {

      break;
    }
    if (customKey == 'D') {
      if (i > 0) {
        str[--i] = '\0';
        lcd.setCursor(i, 1);
        lcd.print(' ');
        lcd.setCursor(i, 1);
      }

    }
    //    else if (customKey == '*') {
    //      lcd.print('.');
    //      str[i] = (char)customKey;
    //      str[++i] = '\0';
    //
    //    }
    else if (customKey) {
      //Serial.println(customKey);
      lcd.print(customKey);
      str[i] = (char)customKey;
      str[++i] = '\0';

    }
  }
  //Serial.println(str);
}

float getSpeedPerMeter(float len) {
  if ((0 < len) && (len <= 1)) {
    return 10.625;
  } else if ((1 < len) && (len <= 1.5)) {
    return 8.625;
  } else if ((1.5 < len) && (len <= 2)) {
    return 6.875;
  } else if ((2 < len) && (len <= 2.5)) {
    return 5.6275;
  } else if ((2.5 < len) && (len <= 3)) {
    return 4.9;
  } else if ((3 < len) && (len <= 3.5)) {
    return 4.3;
  } else if ((3.5 < len) && (len <= 4)) {
    return 3.8;
  } else if ((4 < len) && (len <= 4.5)) {
    return 3.65;
  } else if ((4.5 < len) && (len <= 5)) {
    return 3.55;
  } else if ((5 < len) && (len <= 5.5)) {
    return 3.4;
  } else {
    return 3.25;
  }
}

void uploadCycleTime(float CT) {
  Serial.printf("CT:%f\n", CT);
}

void wifi_connect() {
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
int total_plies_count = 0;
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
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/updated_total_plies", total_plies_count);
}

void create_new_roll(int roll_id) {
  //roll_id+=1;
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id), 0);
}

void update_tktlen(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/tktlen", value);
}

void update_plies(int roll_id, int value) {
  total_plies_count += value;
  update_total_plies();
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/plies", value);
}

void update_total_damage_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/total_damage_length", value);
}

void update_total_overlapped_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/total_overlapped_length", value);
}

void update_total_used_length(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/update_total_used_length", value);
}

void update_ends(int roll_id, int value) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/ends", value);
}

void update_on_track(int roll_id, bool stat) {
  Firebase.setDouble(firebaseData, data_base + "/" + String(batch_number) + "/" + String(roll_id) + "/on_track", stat);
}
//######################################
