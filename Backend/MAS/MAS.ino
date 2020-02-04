#include <Keypad.h>
#include <LiquidCrystal.h>

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

//variables
int number_of_plies;
int number_of_rolls;
float layer_length;
char text[10]; //for taking input as a string

//constants
float pi = 3.14;
int N = 24;
float R = 4.1; //in cm

// Declare encoder pins
int dir_pin_1 = 32;
int dir_pin_2 = 35;
int dis_pin = 34;

float t1 = millis();

// Start measuring
int last = 0;
int count = 0;
float distance = 0;
int last_plies = 0;
int lastA = 0;
int dir = 1;

bool is_half_ply = false;
bool is_damage = false;
bool is_roll_finished = false;

// Variables to submit
int plies = 0;
int total_plies = 0;
int roll_id = 0;
int number_of_rolls_finished = 0;
float total_damage_length = 0;
float total_overlap_length = 0;
float total_used_length = 0;
bool on_track = false;


void setup() {

  pinMode(dir_pin_1, INPUT);
  pinMode(dir_pin_2, INPUT);
  pinMode(dis_pin, INPUT);


  Serial.begin(115200);
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

  lcd.print("No of rolls:");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%d", &number_of_rolls);

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Roll id:");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%d", &roll_id);

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("No of plies:");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%d", &number_of_plies);

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Layer length(cm):");
  lcd.setCursor(0, 1);
  getInputString('#', text);
  sscanf(text, "%f", &layer_length);

  lcd.clear();
  lcd.setCursor(0, 0);
  // End: Get user inputs

  //Calculating Cycle time in seconds
  float CT = (247 + number_of_rolls * 9 + number_of_rolls * 70 + layer_length * number_of_plies * getSpeedPerMeter(layer_length) + 120 * number_of_rolls) * 60 * 5 / 6;

  uploadCycleTime(CT);

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

  // Variables to submit
  total_damage_length = 0;
  total_overlap_length = 0;
  total_used_length = 0;
  on_track = false;

  while (true) {

    //When number of plies changed, submit data to the server
    if (last_plies != plies) {
      //On track/Offtrack calculation
      float req_time = plies * CT / number_of_plies;
      float t2 = millis();

      float curr_time = (t2 - t1) / 1000;

      if (curr_time > req_time) {
        //Off track
        on_track = false;
      } else {
        //On track
        on_track = true;
      }
      last_plies = plies;

      uploadData();

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

    }

    if (is_roll_finished) {
      number_of_rolls_finished++;
      total_used_length += distance;
      total_plies += plies;
      //Submit data*****************************************
      uploadData();
      //****************************************************
      is_roll_finished = false;

      lcd.clear();
      lcd.setCursor(0, 0);

      lcd.print("Roll id:");
      lcd.setCursor(0, 1);
      getInputString('#', text);
      sscanf(text, "%d", &roll_id);

      //************************************************
      uploadData();
      //************************************************

      lcd.clear();
      lcd.setCursor(0, 0);

      plies = 0;

    }

    if (plies >= number_of_plies) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Done !");
      delay(1000);
      //Submit data*****************************************
      uploadData();
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

      distance += dis_with_overlap;
      plies++;
      distance = 0;
      //*********************************************************
      total_overlap_length += layer_length - dis_with_overlap;
      //*********************************************************

      lcd.clear();

      is_half_ply = false;

    } else if (is_damage) {
      lcd.clear();
      total_damage_length += distance;
      distance = 0;
      lcd.setCursor(0, 0);
      //**************************************************************
      uploadData();
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

void uploadData() {

  Serial.printf("roll_id:%d, layer_length:%f, number_of_plies:%d, plies:%d\n", roll_id, layer_length, number_of_plies, plies);
  Serial.printf("total_plies:%d\n", total_plies);
  Serial.printf("total_damage_length:%f\n", total_damage_length);
  Serial.printf("total_overlap_length:%f\n", total_overlap_length);
  Serial.printf("total_used_length:%f\n", total_used_length);
  Serial.printf("on_track:%d\n\n", on_track);

  // int number_of_plies;
  // int plies;
  // int total_plies;
  // int roll_id;
  // float layer_length;
  // int number_of_plies;
  // float total_damage_length;
  // float total_overlap_length;
  // float total_used_length;
  // bool on_track = false;
}
