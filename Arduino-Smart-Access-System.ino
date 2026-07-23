#include "arduino_secrets.h"


#include <Adafruit_BusIO_Register.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RST 9
#define SDA 10
MFRC522 sensor(SDA, RST);

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Servo meuservo;

#define VERDE 2
#define VERMELHO 3
#define PINO_SERVO 6
int buzzer = 4;


byte chaveiro []  = {0xD3, 0xC7, 0xC1, 0x2C};
byte cartao []  = {0xD3, 0xC7, 0xC1, 0x2C};

unsigned long ultimoLog = 0; 
bool compararUID(byte* lido, byte* autorizado, byte tamanho) {
for (byte i = 0; i < tamanho; i++) {
if (lido[i] != autorizado[i]) return false; }
return true;
}
void setup() {

Serial.begin(9600);
SPI.begin();
sensor.PCD_Init();
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
meuservo.attach(PINO_SERVO);
meuservo.write(0); 

pinMode(VERDE, OUTPUT);
pinMode(VERMELHO, OUTPUT);
  pinMode(buzzer, OUTPUT);

digitalWrite(VERMELHO, HIGH);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(10, 20);

display.println ("RFID Pronto");
display.display();
Serial.println ("Sistema pronto e aguardando cartao...");
delay(2000);
}

void acessoPermitido(){

digitalWrite(VERDE, HIGH);
digitalWrite(VERMELHO, LOW);
meuservo.write(180); //abriu

display.clearDisplay();
display.setCursor(0, 20);
display.setTextSize(2);
display.println("Acesso Liberado");

display.display();
 Serial.println ("Acesso permitido, OlÃ¡ MME.");
delay(500);
meuservo.write(0);
Serial.println ("fechando cancela.");
digitalWrite(VERDE, LOW);
}

void acessoNegado(){

digitalWrite(VERDE, LOW);
digitalWrite(VERMELHO, HIGH);
meuservo.write(0); //fechado
display.clearDisplay();
display.setCursor(0, 20);
display.setTextSize(2);
display.println ("Acesso Negado");
display.display();
Serial.println ("Acesso negado. Vaza!");
delay(2000);
digitalWrite(VERMELHO, LOW);
}

void loop() {

if(acessoPermitido){
  noTone(buzzer);
  delay(500);
  tone(buzzer, 800);
  delay(1000);
    noTone(buzzer);
}
  else{
    if(acessoNegado){
      noTone(buzzer);
    }

    else{
      noTone(buzzer);
    }
    
  }
  
  
  if(!sensor.PICC_IsNewCardPresent() || !sensor.PICC_ReadCardSerial()){
  if (millis() - ultimoLog >= 1000) {
  Serial.println ("Aguardando CartÃ£o...");
  ultimoLog = millis();
  }
  return;
  } 

  if (compararUID(sensor.uid.uidByte, cartao, sensor.uid.size)){
  acessoPermitido();
  } else if (compararUID(sensor.uid.uidByte, chaveiro, sensor.uid.size)){
  acessoPermitido();
  } else {
  acessoNegado();
  }

  sensor.PICC_HaltA();
}



