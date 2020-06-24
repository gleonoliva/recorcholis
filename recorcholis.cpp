#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Estos son los numeros de pines en la tarjeta arduino
const byte BTN_1 = 7;
const byte BTN_2 = 8;
const byte BTN_3 = 9;

// Minimo numero de bolis a llenar y sellar
const int MIN_BOLIS = 1;

// Estado de execution general del programa
enum Estado {
	Inicial,
	Menu,
	Trabajando,
};

// Los pasos del menu
enum PasosMenu {
	Tipo,
	Cantidad,
	Confirmar
};

// Tipos de bolis, los pasos de trabajo dependeran de la opcion seleccionada
enum VariedadBolis {
	Sencillo,
	Chamoy,
	Frutas,
	Ultima = Frutas,
};

// Texto en pantalla para cada variedad de bolis
static const char* VariedadNombres[] = {
		"Sencillo        ",
		"Chamoy          ",
		"Frutas          "};


// # Variables del programa
// Arreglos para los botones, true si esta apretado en el momento de lectura
byte botones[3];
// Arreglos para los valores de los botones en la iteracion pasada
byte botones_antes[3];
// Estado actual de ejecucion del programa, siempre empieza en el estado Inicial
Estado estado = Inicial;
// Variedad de bolis seleccionada por el usuario
VariedadBolis variedad;
// Paso actual del menu
PasosMenu pasoMenu;
// Numero de bolis a sellar y llenar seleccionado por el usuario
int numeroDeBolis;

// Regresa true si el boton justamente acaba de dejar de ser presionado
bool estabaPresionado(int boton){
	return !botones[boton] && botones_antes[boton];
}

// Maneja el menu, regresa al estado que tiene que transicionar para la proxima iteracion
Estado manejaMenu() {
	Estado nuevoEstado = estado;
	switch(pasoMenu) {
		case Tipo:
			lcd.setCursor(0, 0);
			lcd.print("Selecciona tipo:");
			lcd.setCursor(0, 1);
			lcd.print(VariedadNombres[variedad]);

			// Variedad anterior
			if (estabaPresionado(0)) {
				variedad = max(0, variedad - 1);
			}

			// Variedad siguiente
			if (estabaPresionado(1)) {
				variedad = min(Ultima, variedad+1);
			}

			if (estabaPresionado(2)) {
				// Pasamos a la seleccion de cantidad
				pasoMenu = Cantidad;
				lcd.clear();
			}

			break;

		case Cantidad:

			lcd.setCursor(0, 0);
			lcd.print("Numero de Bolis");
			lcd.setCursor(0, 1);
			lcd.print(numeroDeBolis);
			lcd.print(" ");

			// Menos uno, respetando el minimo de bolis
			if (estabaPresionado(0)) {
				numeroDeBolis = max(MIN_BOLIS, numeroDeBolis - 1);
			}

			if (estabaPresionado(1)) {
				numeroDeBolis++; // Seria bueno poner un maximo?
			}

			if (estabaPresionado(2)) {
				lcd.clear();
				pasoMenu = Confirmar;
			}
			break;

		case Confirmar:

			lcd.setCursor(0, 0);
			lcd.print("Estas segur@?");
			lcd.setCursor(0, 1);
			lcd.print(numeroDeBolis);

			// Cualquiera de los otros botones para volver a empezar
			if (estabaPresionado(0) || estabaPresionado(1)) {
				pasoMenu = Tipo;
			}

			// Confirma y empieza el trabajo
			if (estabaPresionado(2)) {
				nuevoEstado = Trabajando;
			}
			break;

	}
	return nuevoEstado;
}



void trabajar(){
	lcd.print("Trabajando...");
	// Aqui iria todo el codigo para llenar y sellar
	delay(6000); // Simula el trabajo



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

	botones[0] = digitalRead(BTN_1) == HIGH; // -  Menos
	botones[1] = digitalRead(BTN_2) == HIGH; // +  Mas
	botones[2] = digitalRead(BTN_3) == HIGH; // [] Confirmar

	switch(estado) {
		case Inicial:
			variedad = Sencillo;
			pasoMenu = Tipo;
			numeroDeBolis = MIN_BOLIS;
			estado = Menu;
			break;
		case Menu:
			estado = manejaMenu();
			break;
		case Trabajando:
			trabajar();
			estado = Inicial;
			break;

	}

	delay(50); // Para refrescar la pantalla
	memcpy(botones_antes, botones, sizeof botones);
}
