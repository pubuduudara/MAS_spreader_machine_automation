int dir_pin_1 = 32;
int dir_pin_2 = 35;
void setup() {
  // put your setup code here, to run once:
  pinMode(dir_pin_1, INPUT);
  pinMode(dir_pin_2, INPUT);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  int a = digitalRead(dir_pin_1);
  int b = digitalRead(dir_pin_2);

  Serial.print(a);
  Serial.print(",");
  Serial.println(b);
}
