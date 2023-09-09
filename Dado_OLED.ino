/**************************************************************************
Realizado por: Juan Carlos Chuc
Dado digital v1.0
**************************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

unsigned long currentTime=0;
unsigned long previousTime=0;

void pantalla_inicial();
void seleccionar_dados();
uint8_t centrar_texto(String);
void lanzar_dado();
void imprimir_numeros(uint8_t, uint8_t);
void imprimir_resultados ();

uint8_t cantidad_tiros = 1;
uint8_t array_resultados[9];
bool comenzar_juego = false;
bool seleccion_tiros = false;
bool animacion1 = true;

#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64
#define DIRECCION_OLED 0x3C
#define btn_lanzar_dado 4
#define btn_aumentar 2
#define btn_disminuir 3

Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

const unsigned char logo_bmp[] PROGMEM = {
  0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x06, 0x00, 0x00, 0x30, 0x00, 
  0x0c, 0x20, 0x02, 0x10, 0x00, 0x0c, 0xf8, 0x0f, 0x98, 0x00, 0x09, 0xf8, 0x0f, 0x98, 0x00, 0x09, 
  0xf8, 0x1f, 0xd8, 0x00, 0x09, 0xf8, 0x1f, 0x98, 0x00, 0x08, 0xf8, 0x0f, 0x98, 0x00, 0x08, 0x60, 
  0x07, 0x18, 0x00, 0x08, 0x03, 0xe0, 0x18, 0x00, 0x00, 0x03, 0xe0, 0x18, 0x0f, 0xff, 0xf8, 0xe0, 
  0x18, 0x3f, 0xff, 0xfe, 0x60, 0x18, 0x7f, 0xff, 0xff, 0x60, 0x18, 0xfd, 0xff, 0xff, 0x80, 0x18, 
  0xf0, 0x7f, 0xff, 0x8f, 0x98, 0xf0, 0x7f, 0xff, 0x8f, 0x98, 0xe0, 0x3f, 0xff, 0x9f, 0xd8, 0xf0, 
  0x7f, 0xff, 0x9f, 0xd8, 0xf0, 0x7f, 0xff, 0x8f, 0x98, 0xff, 0xf7, 0xff, 0x87, 0x18, 0xff, 0xc1, 
  0xff, 0x80, 0x30, 0xff, 0xc1, 0xff, 0x80, 0xe0, 0xff, 0xc0, 0xff, 0xbf, 0xc0, 0xff, 0xc1, 0xff, 
  0x80, 0x00, 0xff, 0xc1, 0xff, 0x80, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0x87, 0x80, 
  0x00, 0xff, 0xff, 0x07, 0x80, 0x00, 0xff, 0xff, 0x03, 0x80, 0x00, 0xff, 0xff, 0x07, 0x80, 0x00, 
  0xff, 0xff, 0x07, 0x80, 0x00, 0xff, 0xff, 0xcf, 0x80, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x3f, 
  0xff, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00
};

void setup() {
  Serial.begin(9600);
  pinMode(btn_lanzar_dado, INPUT_PULLUP);
  pinMode(btn_aumentar, INPUT_PULLUP);
  pinMode(btn_disminuir, INPUT_PULLUP);
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIRECCION_OLED)) {
    Serial.println("No se encuentra la pantalla OLED");
    for(;;);
  }
  Serial.println("\nDado V1.0");
  display.clearDisplay();
  display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
  display.drawBitmap(78, 13, logo_bmp, 37, 37, WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,14);
  display.println("Dado");
  display.setCursor(10,36);
  display.println("v1.0");
  display.display();
  delay(2000);
}

void loop() {
  if (comenzar_juego == false) {
    pantalla_inicial();
  }
  if(digitalRead(btn_lanzar_dado)== LOW){
    comenzar_juego = true;
    seleccionar_dados();
  }
}

void pantalla_inicial () {
  display.clearDisplay();
  currentTime=millis();
  if((currentTime-previousTime)>600){
    previousTime=currentTime;
    animacion1 = !animacion1;
  }
  if (animacion1 == true) {
    display.fillRect(98, 40, 12, 2, WHITE);
  } else {
    display.fillRect(98, 37, 12, 5, WHITE);
  }
  display.fillRect(92, 44, 24, 6, WHITE);
  display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,14);
  display.println("Iniciar");
  display.setCursor(10,36);
  display.println("juego");
  display.display();
}

void seleccionar_dados () {
  delay(300);
  while (seleccion_tiros == false) {
    if(digitalRead(btn_aumentar)== LOW) {
      if (cantidad_tiros<10) cantidad_tiros++;
      delay(200);
    }
    if(digitalRead(btn_disminuir)== LOW) {
      if (cantidad_tiros>1) cantidad_tiros--;
      delay(200);
    }
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
    display.setCursor(10,14);
    display.println("Numero de");
    display.setCursor(10,36);
    display.print("dados: ");
    //display.setCursor(7,45);
    //display.setCursor(centrar_texto(String(cantidad_tiros)),45);
    display.println(cantidad_tiros);
    display.display();
    if(digitalRead(btn_lanzar_dado)== LOW) {
      seleccion_tiros = true;
      display.clearDisplay();
      display.display();
    }
  }
  lanzar_dado();
}

void lanzar_dado() {
  uint8_t num_aleatorio, num_tiro, cont=1;
  num_tiro = cantidad_tiros;
  while ((num_tiro) > 0) {
    for (uint8_t i=0; i<20; i++) {
      num_aleatorio = random(1,7);
      imprimir_numeros(num_aleatorio, cont);
      delay(50);
    }
    array_resultados[cont-1]=num_aleatorio;
    delay(1000);
    num_tiro--;
    cont ++;
  }
  imprimir_resultados();
}

void imprimir_numeros(uint8_t numero, uint8_t cont) {
  display.clearDisplay();
  display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
  display.drawRoundRect(71, 7, 50, 50, 5, SSD1306_WHITE);
  display.setCursor(10,14);
  display.println("Dado");
  display.setCursor(centrar_texto(String(cont)),36);
  display.println(cont);
  switch (numero){
    case 1:
    display.fillCircle(95, 32, 4, WHITE);
    break;
    case 2:
    display.fillCircle(81, 17, 4, WHITE);
    display.fillCircle(110, 47, 4, WHITE);
    break;
    case 3:
    display.fillCircle(81, 17, 4, WHITE);
    display.fillCircle(95, 32, 4, WHITE);
    display.fillCircle(110, 47, 4, WHITE);
    break;
    case 4:
    display.fillCircle(81, 17, 4, WHITE);
    display.fillCircle(110, 17, 4, WHITE);
    display.fillCircle(81, 47, 4, WHITE);
    display.fillCircle(110, 47, 4, WHITE);
    break;
    case 5:
    display.fillCircle(81, 17, 4, WHITE);
    display.fillCircle(110, 17, 4, WHITE);
    display.fillCircle(95, 32, 4, WHITE);
    display.fillCircle(81, 47, 4, WHITE);
    display.fillCircle(110, 47, 4, WHITE);
    break;
    case 6:
    display.fillCircle(81, 17, 4, WHITE);
    display.fillCircle(110, 17, 4, WHITE);
    display.fillCircle(81, 32, 4, WHITE);
    display.fillCircle(110, 32, 4, WHITE);
    display.fillCircle(81, 47, 4, WHITE);
    display.fillCircle(110, 47, 4, WHITE);
    break;
    default:
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(35,25);
    display.println("Error");
  }
  display.display();
}

void imprimir_resultados () {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
  display.setCursor(5,5);
  display.print("Resultados");
  display.setTextSize(2);
  display.setCursor(10,24);
  for (int i = 0; i < cantidad_tiros; i ++) {
    if (i == 5) {
      display.setCursor(10,43);
    }
    display.print(String(array_resultados[i]));
    if (((i+1) < cantidad_tiros) and (i != 4)) display.print(",");
  }
  display.display();
  delay(300);
  while (digitalRead(btn_lanzar_dado)== HIGH);
  comenzar_juego = false;
  seleccion_tiros = false;
  cantidad_tiros = 1;
  delay(300);
}

uint8_t centrar_texto(String texto) {
  int16_t x1, y1;
  int16_t width, height;
  display.getTextBounds(texto, 0, 0, &x1, &y1, &width, &height);
  return ((ANCHO_PANTALLA - 54) - width) / 2;
}
