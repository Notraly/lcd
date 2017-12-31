#include <Arduino.h>
#include  <LiquidCrystal.h>

const int rs = 12, e1 = 11, e2 = 10, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd1(rs, e1, d0, d1, d2, d3, d4, d5, d6, d7);
LiquidCrystal lcd2(rs, e2, d0, d1, d2, d3, d4, d5, d6, d7);
byte heart[8] = {
	B00000,
	B00000,
	B01010,
	B11111,
	B01110,
	B00100,
	B00000,
	B00000,
};
void setup() {
	lcd1.createChar(0, heart);
	lcd2.createChar(0, heart);
	lcd1.begin(16, 2);
	lcd2.begin(16, 2);


	lcd1.print("Hello my love!");
	lcd1.setCursor(0, 1);
	lcd1.print("I love you");
	lcd1.setCursor(11,1);
	lcd1.write(byte(0));

	lcd2.print("Forever");
	lcd2.setCursor(0, 1);
	lcd2.print("and ever");
/*	lcd2.setCursor(9, 1);
	lcd2.write(byte(0));*/

}

void loop() {
	for (int p = 9; p < 16; p++) {
		lcd2.setCursor(p, 1);
		lcd2.write(byte(0));
		delay(100);
		lcd2.setCursor(p, 1);
		lcd2.print(" ");
	}

	for (int p = 16; p > 9; p--) {
		lcd2.setCursor(p, 1);
		lcd2.write(byte(0));
		delay(100);
		lcd2.setCursor(p, 1);
		lcd2.print(" ");
	}
}
