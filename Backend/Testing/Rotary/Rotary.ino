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

  // Start measuring
  int last = 0;
  int count = 0;
  float distance = 0;
  int lastA = 0;
  int dir = 1;
  
  while (true) {
    
    int a = digitalRead(dir_pin_1);
    int b = digitalRead(dir_pin_2);
    int now = digitalRead(dis_pin);
    
    if(a != lastA){ //When first signal changes
      if(b == a){ //If second signal has already changed
        dir = 1; // Forward direction
      }else{
        dir = -1; // Backward direction
      }
      lastA = a;
    }

    // Serial.printf("a:%d b:%d lastA:%d dir:%d \n", a, b,lastA, dir);

    if (now != last && dir == 1) { // Rotating
      count++;
      distance += pi*R/N;
      Serial.printf("%f cm\n", distance);
      last = now;
    }
  }


}

void loop(){
  
}
