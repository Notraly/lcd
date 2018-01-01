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
// posX = position du coeur sur X et posY = position du coeur sur Y
float posX = 0;
//int posY = 4;
// dir = direction du deplacement
int dir = 1;

long oldMillis;
int deltaMillis;

//dplX = methode pour le déplacement en X et dplY = methode pour le déplacement en Y
void dplX(){
	if(posX <= 0) {
		dir = 1;
	}
	else if (posX >= 15) {
		dir = -1;
	}
	lcd2.setCursor(posX, 1);
	lcd2.print(" ");
	posX += dir;
	posX += deltaMillis * 0.01 * dir ;
	lcd2.setCursor(posX, 1);
	lcd2.write(byte(0));
}
/*void dplY() {

}*/
void setup() {
	lcd1.createChar(0, heart);
	lcd2.createChar(0, heart);
	lcd1.begin(16, 2);
	lcd2.begin(16, 2);
	oldMillis = millis();
}

void loop() {
long newMillis = millis();
deltaMillis = newMillis - oldMillis;
	//dplY();
	dplX();
	oldMillis = newMillis;
}
