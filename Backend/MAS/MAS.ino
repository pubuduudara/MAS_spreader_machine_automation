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

  // Start measuring
  int last = 0;
  int count = 0;
  float distance = 0;
  int plies = 0;
  int lastA = 0;
  int dir = 1;
  bool is_half_ply = false;
  bool is_damage = false;

  while (true) {

    if (plies >= number_of_plies) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Done !");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("Submit? #");
      lcd.setCursor(0, 0);
      while (true) {
        lcd.setCursor(0, 0);
        lcd.print("Plies:");
        lcd.print(plies);
        char key = getInputChar();
        if (key == 'D') {
          plies--;
        } else if (key == 'C') {
          plies++;
        } else if (key == '#') {

          break;
        }
      }

      //Submit data*****************************************

      //****************************************************

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Submitted !");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      break;
    }

    // Check user input
    char customKey = customKeypad.getKey();
    if (customKey == 'A' && !is_half_ply) { // Half ply
      is_half_ply = true;
    } else if (customKey == 'B' && !is_damage) {
      is_damage = true;
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
      float overlap_length = dis_with_overlap - layer_length;

      lcd.clear();

      is_half_ply = false;

    } else if (is_damage) {
      lcd.setCursor(0, 0);
      lcd.print("Start damage?");
      waitForInput('#');
      lcd.clear();
      float damage_distance = 0;
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
          damage_distance += pi * R / N;
          last = now;
        }

        lcd.setCursor(0, 0);
        lcd.printf("Damage L:");
        lcd.print(damage_distance);
        lcd.printf("cm");
        lcd.setCursor(0, 1);
        lcd.printf("Hold # if done");

        char key = customKeypad.getKey();
        if (key == '#') {
          break;
        }
      }
      //*********************************************************

      lcd.clear();
      is_damage = false;
      is_half_ply = true;

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
