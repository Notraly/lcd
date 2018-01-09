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
const float EFFET_FACTOR = 2;
const byte PIN_POTENTIOMETRE = A0;
// posX = position du coeur sur X et posY = position du coeur sur Y
float posX = 0;
float posY = 0;
float vitesseX = 1;
float vitesseY = 0.39;
long oldMillis;
int deltaMillis;
long counter = 0;
float vitesse;


float randFactor(float maxF){
	int r = random(-64,64);
	if(r<0) {
		return .6/(((0.-r)/64)*maxF/2+1);
	}else{
		return .6*((((r+1.)/64)*maxF/2)+1);
	}
}

void updateV(){
	vitesse = analogRead(PIN_POTENTIOMETRE);
	vitesse /= 100000;
}

void writeLcds(int posX, int posY, byte data ){
	if (posY < 2) {
		lcd1.setCursor(posX, posY);
		lcd1.write(data);
	} else {
		lcd2.setCursor(posX, posY - 2);
		lcd2.write(data);
	}

}

//dpl = methode pour le déplacement
void dpl(){

	int oldPosX = posX;
	int newPosX;

	int oldPosY = posY;
	int newPosY;

	posX += deltaMillis * vitesse * vitesseX;
	posY += deltaMillis * vitesse * vitesseY;

	if(posX <= 0) {
		float effet = randFactor(EFFET_FACTOR);
		vitesseX *= -effet;
		vitesseY /= effet;
		posX = -posX;
	}
	else if (posX >= 16) {
		float effet = randFactor(EFFET_FACTOR);
		vitesseX *= -effet;
		vitesseY /= effet;
		posX = 32 - posX;       // posY- 16 = surplus ; 16 - surplus <=> 16 - (posY - 16) = 16 - posY + 16 = 32 - posY
	}

	if(posY <= 0) {
		float effet = randFactor(EFFET_FACTOR);
		vitesseX /= effet;
		vitesseY *= -effet;
		posY = -posY;
	}
	else if (posY >= 4) {
		float effet = randFactor(EFFET_FACTOR);
		vitesseX /= effet;
		vitesseY *= -effet;
		posY = 8 - posY;        // posY- 4 = surplus ; 4 - surplus <=> 4 - (posY - 4) = 4 - posY + 4 = 8 - posY
	}
	newPosY = posY;
	newPosX = posX;

	if (newPosX != oldPosX || newPosY != oldPosY) {
		writeLcds(oldPosX,oldPosY,byte( 0x20 )); // 0x = ce qu'il y a derrière est en hexadécimale
		writeLcds(newPosX,newPosY,byte(0));
	}
}

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
	updateV();
	dpl();
	oldMillis = newMillis;
	counter++;

	if((counter & 127) == 127) {
		long fps = counter * 1000 / newMillis;
		lcd1.home();
		lcd1.print(fps);
		lcd1.print(" fps ");
	}
}
