#include <Arduino.h>
#include  <LiquidCrystal.h>

const int rs = 10, e1 = 12, e2 = 11, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
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
byte topBar[8] = {
	B11111,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
};
byte bottomBar[8] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B11111,
};
const float EFFET_FACTOR = 2;
const byte PIN_POTENTIOMETRE = A0;
const byte PIN_JOYSTICK1_X = A1;
const byte PIN_JOYSTICK1_Y = A2;
const byte PIN_JOYSTICK2_X = A3;
const byte PIN_JOYSTICK2_Y = A4;
// posX = position du coeur sur X et posY = position du coeur sur Y
float posX = 0;
float posY = 0;
float vitesseX = 1;
float vitesseY = 0.39;
long oldMillis;
int deltaMillis;
long counter = 0;
float vitesse;
// vX = vitesse en x et vY = vitesse en Y et x = position sur la ligne et y = position sur la colonne
struct Raquettes {
	float x, y, vX, vY;
	byte pinX, pinY;
	int row, column; // row = ligne de déplacement (x) et column = ligne de déplacement (y)
	float scalVX,scalVY; // row = ligne de déplacement (x) et column = ligne de déplacement (y)
};

Raquettes joueur1 = {8,2,0,0,PIN_JOYSTICK1_X, PIN_JOYSTICK1_Y,0,0, 0.005, 0.0025};
Raquettes joueur2 = {8,2,0,0,PIN_JOYSTICK2_X, PIN_JOYSTICK2_Y,3,15, 0.005, 0.0025};
void writeLcds(int posX, int posY, byte data ){
	if (posY < 2) {
		lcd1.setCursor(posX, posY);
		lcd1.write(data);
	} else {
		lcd2.setCursor(posX, posY - 2);
		lcd2.write(data);
	}

}

float convertValueJoystisk (int value){

	if (value < 25) {
		return -1;

	} else if (value < 475) {
		return (value-475)/450.;

	} else if (value < 550) {
		return 0;

	}else if (value < 1000) {
		return (value-550)/450.;

	}else if (value < 1024) {
		return 1;
	}
}

void dplJoystick(Raquettes &joueur){
	float valeurX = convertValueJoystisk(analogRead(joueur.pinX));
	float valeurY = convertValueJoystisk(analogRead(joueur.pinY));

	joueur.vX = valeurX * joueur.scalVX;
	joueur.vY = valeurY * joueur.scalVY;

	int oldPosX = joueur.x;
	int newPosX;
	int oldPosY = joueur.y;
	int newPosY;

	joueur.x += deltaMillis * joueur.vX;
	joueur.y += deltaMillis * joueur.vY;

	if (joueur.x < 0) {
		joueur.x = 0;
	} else if(joueur.x >= 16) {
		joueur.x = 15.99;
	}

	if (joueur.y < 0) {
		joueur.y = 0;
	} else if (joueur.y >= 4) {
		joueur.y = 3.99;
	}

	newPosY = joueur.y;
	newPosX = joueur.x;

	if (newPosX != oldPosX  ) {
		writeLcds(oldPosX, joueur.row, byte( 0x20 )); // 0x = ce qu'il y a derrière est en hexadécimale
		writeLcds(newPosX, joueur.row, byte(1));
	}
	if (newPosY != oldPosY) {
		writeLcds(joueur.column, oldPosY, byte( 0x20 )); // 0x = ce qu'il y a derrière est en hexadécimale
		writeLcds(joueur.column, newPosY, byte(0x7C));
	}
}

void testJ1() {
	float valeurX1 = -convertValueJoystisk(analogRead(PIN_JOYSTICK1_X));
	float valeurY1 = convertValueJoystisk(analogRead(PIN_JOYSTICK1_Y));


	lcd2.home();
	lcd2.print("X: ");
	lcd2.print(valeurX1);
	lcd2.print("   ");

	lcd2.setCursor(0,1);
	lcd2.print("Y: ");
	lcd2.print(valeurY1);
	lcd2.print("   ");


}
void testJ2() {
	float valeurX2 = convertValueJoystisk(analogRead(PIN_JOYSTICK2_X));
	float valeurY2 = -convertValueJoystisk(analogRead(PIN_JOYSTICK2_Y));

	lcd2.setCursor(9,0);
	lcd2.print("X: ");
	lcd2.print(valeurX2);
	lcd2.print("   ");

	lcd2.setCursor(9,1);
	lcd2.print("Y: ");
	lcd2.print(valeurY2);
	lcd2.print("   ");

}

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

void refresh(){
	lcd1.clear();
	lcd2.clear();
	writeLcds(joueur1.x, joueur1.row, byte(1));
	writeLcds(joueur1.column, joueur1.y, byte(0x7C));
	writeLcds(joueur2.x, joueur2.row, byte(1));
	writeLcds(joueur2.column, joueur2.y, byte(0x7C));
	writeLcds(posX, posY, byte(0));
}

//deplacementBalle = methode pour le déplacement de la balle
void deplacementBalle(){

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

void fps (){

	counter++;

	if((counter & 127) == 127) {
		long fps = counter * 1000 / millis();
		lcd1.home();
		lcd1.print(fps);
		lcd1.print(" fps ");
	}
}

void setup() {
	lcd1.createChar(0, heart);
	lcd2.createChar(0, heart);
	lcd1.createChar(1, topBar);
	lcd2.createChar(1, bottomBar);
	lcd1.begin(16, 2);
	lcd2.begin(16, 2);
	oldMillis = millis();
	Serial.begin(9600);
	refresh();

}
void debugBalle(){
	Serial.print("debugBalle 	");
	Serial.print(posX);
	Serial.print(" 	");
	Serial.print(posY);
	Serial.print(" 	");
	Serial.print(vitesse);
	Serial.print(" 	");
	Serial.print(vitesseX);
	Serial.print(" 	");
	Serial.print(vitesseY);
	Serial.print(" 	");
	Serial.println(analogRead(A0));
}
void debug(){
	debugBalle();
	delay(200);
}
void loop() {
	long newMillis = millis();
	deltaMillis = newMillis - oldMillis;
	// START LOOP
	updateV();
	deplacementBalle();
	dplJoystick(joueur1);
	dplJoystick(joueur2);
	debug();
	// END LOOP
	oldMillis = newMillis;

	//fps();


}
