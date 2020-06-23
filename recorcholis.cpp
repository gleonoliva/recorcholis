#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte BTN_1 = 7;
const byte BTN_2 = 8;
const byte BTN_3 = 9;

const int MIN_BOLIS = 1;

enum Estado {
	Inicial,
	Menu,
	Trabajando,
};

enum PasosMenu {
	Tipo,
	Cantidad
};

enum VariedadBolis {
	Sencillo,
	Chamoy,
	Frutas,
	Ultima = Frutas,
};

static const char* VariedadNombres[] = {
		"Sencillo        ",
		"Chamoy          ",
		"Frutas          "};

byte botones[3];

Estado estado = Inicial;
VariedadBolis variedad;
PasosMenu pasoMenu;
int numeroDeBolis;

void manejaMenu() {
	switch(pasoMenu) {
		case Tipo:
			if (botones[0]) {
				variedad = max(0, variedad - 1);
			}

			if (botones[1]) {
				variedad = min(Ultima, variedad+1);
			}

			lcd.setCursor(0, 0);
			lcd.print("Selecciona tipo:");
			lcd.setCursor(0, 1);
			lcd.print(VariedadNombres[variedad]);

			if (botones[2]) {
				pasoMenu = Cantidad;
				lcd.clear();
			}

			delay(200);

			break;

		case Cantidad:

			if (botones[0]) {
				numeroDeBolis = max(MIN_BOLIS, numeroDeBolis - 1);
			}
			if (botones[1]) {
				numeroDeBolis++;
			}

			lcd.setCursor(0, 0);
			lcd.print("Numero de Bolis");
			lcd.setCursor(0, 1);
			lcd.print(numeroDeBolis);
			lcd.print(" ");

			delay(200);

			if (botones[2]) {
				estado = Trabajando;
				lcd.clear();
			}

			break;
	}
}

void setup() {
	Serial.begin(115200);
	lcd.begin(16, 2);
	pinMode(BTN_1, INPUT);
	pinMode(BTN_2, INPUT);
	pinMode(BTN_3, INPUT);
	estado = Inicial;
}

void loop() {

	botones[0] = digitalRead(BTN_1) == HIGH;
	botones[1] = digitalRead(BTN_2) == HIGH;
	botones[2] = digitalRead(BTN_3) == HIGH;

	switch(estado) {
		case Inicial:
			estado = Menu;
			variedad = Sencillo;
			pasoMenu = Tipo;
			numeroDeBolis = MIN_BOLIS;
			break;
		case Menu:
			manejaMenu();
			break;
		case Trabajando:
			lcd.print("Trabajando...");
			delay(numeroDeBolis * 1000);
			estado = Inicial;
			break;

	}
}
