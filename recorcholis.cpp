//-------------PARA USAR CON ARDUINO MEGA DEBIDO A LA CANTIDAD DE ENTRADAS Y SALIDAS--------------------
// NO ME MARCA ERRORES DE COMPILACIÓN EL ARDUINO PERO NO LO HE PROBADO
#include <LiquidCrystal_I2C.h>     //Librería para el display
#include <Wire.h>                  //Librería para usar el bus I2C de comunicación con el display
#include <Servo.h>                 //Librería para el servomotor 

Servo servo1;                      // se crea el objeto servo1


 
// Seteamos la direccion LCD a 0x27 o 0x3F para un display de 20 caracteres 4 lineas
// 
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol   Seteamos los pines sobre el chip I2C usados para las conexiones del LCD:
 LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Seteamos la direccion I2C
 // define los pines y variables usados, el pin 8 y 9 van conectados al driver DRV8825 que controla el motor a pasos
  
 int sencillo = 2;              //pin 2 conección del boton modo 1
 int valsencillo;               //variable para guardar el estado (High - low) del boton sencillo
 int chamoy   = 3;              //pin 3 conección del boton modo 2  
 int valchamoy;                 //variable para guardar el estado (High - low) del boton chamoy
 int jarabe   = 4;              //pin 4 conección del boton modo 3
 int valjarabe;                 //variable para guardar el estado (High - low) del boton jarabe
 int inicio   = 5;              //pin 2 conección del boton "verde"
 int valorinicio;               //variable para guardar el estado (High - low) del boton inicio
 int paro     = 6;              //pin 3 conección del botón "rojo"
 int valorparo;                 //variable para guardar el estado (High - low) del boton paro
 int reset    = 7;              //pin 7 conección del botón "reset"
 int valorreset;                //variable para guardar el estado (High - low) del boton reset
 int sensor1  = 8;              //pin 8 conexión del sensor1 "bajo las válvulas"
 int valsensor1;                //variable para guardar el estado (High - low) del sensor1
 int sensor2  = 9;              //pin 9 conexión del sensor2 "bajo el sellado"
 int valsensor2;                //variable para guardar el estado (High - low) del sensor2
 int sensor3  = 10;             //pin 10 conexión del sensor3 "contador"
 int valsensor3 = 0;            //variable para guardar el estado (High - low) del sensor3
 #define valvula1 23            //pin 23 válvula del contenedor 1 
 #define valvula2 24            //pin 24 válvula del contenedor 2 
 #define stepPin  25            //pin 25 pone en movimiento el motor de la banda 
 #define dirPin   26            //pin 26 indica al motor el sentido de giro 
 #define rescont2 27            //pin 27 activa la resistencia del contenedor 2 para calentar el jarabe 
 #define resella  28            //pin 28 activa la resistencia del sellador de bolis
 
 void setup() {
  
  lcd.begin(20,4);              //Inicializamos el lcd para 20 caracteres 4 lineas, Encendemos el backlight
  servo1.attach(11);            //asigno el pin 11 como salida pwm
  pinMode(stepPin,OUTPUT);      //asigno el pin 25 como salida
  pinMode(dirPin,OUTPUT);       //asigno el pin 26 como salida
  pinMode(valvula1,OUTPUT);     //asigno el pin 23 como salida
  pinMode(valvula2, OUTPUT);    //asigno el pin 24 como salida
  pinMode(rescont2, OUTPUT);    //asigno el pin 27 como salida
  pinMode(resella, OUTPUT);     //asigno el pin 28 como salida
 
  

  //-------- Escribimos caracteres sobre el display ------------------
  // NOTA: Posicion del Cursor: Lineas y Caracteres empiezan en 0  
  lcd.setCursor(0,0); //Empiezo en la posicion 6 (caracter) sobre la linea 0
  lcd.print("****BIENVENIDOS*****");
  delay(2000);
  lcd.setCursor(0,2);
  lcd.print("   BOLIS CHISPITAS  ");
  delay(3000);  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELECCIONE EL MODO  ");
  lcd.setCursor(0,1);
  lcd.print("DE OPERACION:       ");
  lcd.setCursor(0,2);
  lcd.print("1:SENCILLO. 2:CHAMOY");
  lcd.setCursor(0,3);
  lcd.print("3:FRUTA O JARABE");
  delay(5000);  
  // NO TIENE QUE PASAR DE AQUÍ HASTA QUE RECIBA EL MODO DE OPERACIÓN
  // SE ME OCURRE QUE POR UN SWITCH-CASE SELECCIONE EL CODIGO A EJECUTAR
  // NO SE COMO PONER EL BOTON RESET, ÉSTE REGRESARÁ AL MENÚ PARA CAMBIAR EL MODO DE OPERACIÓN
  // EL SIGUIENTE CÓDIGO CORRESPONDE AL MODO SENCILLO:
 }
void loop() {
  
 valorinicio = digitalRead (5);   // Lee el boton inicio (botón verde) y asigna el estado a valorinicio
 valsensor1 =  digitalRead (8);   // Lee el sensor1 y asigna el estado a valsensor1
 
if ((valorinicio == HIGH) && (valsensor1 == LOW)){        // si el botón verde está presionado y el sensor1 no detecta nada... 
  digitalWrite(dirPin,HIGH);                              // especifica la dirección de giro del motor a pasos
   

// ---------------------------llenado del primer bolis----------------------------------

  while (valsensor1 == LOW){                              // mientras el sensor1 no detecte nada, el motor sigue girando, de lo contrario para
  digitalWrite(stepPin,HIGH);                             // crea una onda cuadrada que es ugual a 1 paso del motor
  delay(10);                                              //            ______  HIGH
  digitalWrite(stepPin,LOW);                              //            |     |
  delay(10);                                              //   LOW _____|10ms |_____  1 vuelta del motor (360°) = 400 pasos
  }
delay(1000);                                              // espera 1 segundo
digitalWrite (valvula1, HIGH);                            // activa la válvula 1 por 2 segundos
delay (2000);
digitalWrite (valvula1, LOW);                             // desactiva la válvula 1 por 2 segundos
delay (2000);
}
  else{                                                   // si el botón verde NO está presionado y el sensor1 detecta algo...
  digitalWrite(dirPin,HIGH);                              // mantiene la misma direccion de giro del motor
  digitalWrite(stepPin,LOW);                              // deshabilita el pin de "arranque" del motor
  delay (10);                                             // espera 10 milisegundos
  }

// llenado del segundo bolis.... repetiría el mismo codigo para el 3°, 4° 5° y 6° bolis? o se puede hacer un llamado a éste segmento del programa?

// se supone que aquí va un for para que con X vueltas del motor avance hasta la siguiente marca...
  valsensor1 =  digitalRead (11);                         // Lee el sensor1 y asigna el estado a valsensor1
 if( valsensor1 == HIGH ){;                                // Si el sensor1 ya detecta la segunda marca...
 digitalWrite (valvula1, HIGH);                           // activa la válvula 1 por 2 segundos
 delay (2000);
 digitalWrite (valvula1, LOW);                             // desactiva la válvula 1 por 2 segundos
 delay (2000);

}

  else{ 
    while (valsensor1 == LOW){                            // mientras el sensor1 no detecte nada, el motor sigue girando, de lo contrario para
  digitalWrite(stepPin,HIGH);                             // crea una onda cuadrada que es ugual a 1 paso del motor
  delay(10);                                              //            ______  HIGH
  digitalWrite(stepPin,LOW);                              //            |     |
  delay(10);                                              //   LOW _____|10ms |_____  1 vuelta del motor (360°) = 400 pasos
  }
  }

  // -----------------------sellado y cortado del 1° y 2° bolis----------------------------------------

  valsensor2 =  digitalRead (9);                          // Lee el sensor1 y asigna el estado a valsensor1
  while (valsensor2 == LOW ){                             // mientras el sensor 2 no detecte nada...
  digitalWrite(stepPin,HIGH);                             // crea una onda cuadrada que es ugual a 1 paso del motor
  delay(10);                                              //            ______  HIGH
  digitalWrite(stepPin,LOW);                              //            |     |
  delay(10);                                              //   LOW _____|10ms |_____  1 vuelta del motor (360°) = 400 pasos
  }
  if (valsensor2 == HIGH){                                // si el sensor 2 detecta algo...
    digitalWrite (resella, HIGH);                         // activa la resistencia del sellador
    delay(1500);                                          // espera 1.5 segundos para que la resistencia alcance su temperatura
    servo1.write (15);                                    // mueve el servomotor 15° para sellar el bolis
    delay(1000);                                          // espera 1 segundo mientras se sella el bolis
    digitalWrite (resella, LOW);                          // desactiva la resistencia del sellador
    servo1.write (0);                                     // regresa el servomotor a 0°
    delay(500);                                           // espera medio segundo
  }
 //-------------------------sellado y cortado del bolis 3-4, 5 y6. repito codigo?-------------------------------- 

 //-------------------------contador de bolis-----------------------------
 if (digitalRead(10) == HIGH){
 if (digitalRead(10) == LOW){
     valsensor3++;
     delay(100);
     lcd.setCursor(0,2);
     lcd.print("SE HAN HECHO:");
     lcd.setCursor(0,3);
     lcd.print(valsensor3);
     lcd.setCursor(3,3);
     lcd.print("BOLIS");
     
 }
 }
  }
