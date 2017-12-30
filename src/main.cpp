#include <Arduino.h>
#include  <LiquidCrystal.h>

  const int rs = 12, e1 = 11, e2 = 10, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
  LiquidCrystal lcd1(rs, e1, d0, d1, d2, d3, d4, d5, d6, d7);
  LiquidCrystal lcd2(rs, e2, d0, d1, d2, d3, d4, d5, d6, d7);
void setup() {
	// put your setup code here, to run once:
  lcd1.begin(16, 2);
  lcd2.begin(16, 2);
  lcd1.print("Hello my love!");
  lcd2.print("I love you <3");
}

void loop() {
	// put your main code here, to run repeatedly:
}
