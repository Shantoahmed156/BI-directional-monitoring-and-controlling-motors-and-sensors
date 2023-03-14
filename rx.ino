#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 10); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};

struct Data_send {
  byte button1;
  byte button2;
  byte button3;
};

Data_send send_data;

struct Potvalue {
  byte pot1;
  byte pot2;
  byte pot3;
  byte pot4;
};

Potvalue received_data;

int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4); // Initialize the display with the I2C address and dimensions

void setup() {
 // Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  lcd.init(); // Initialize the display
  lcd.backlight(); // Turn on the backlight
  lcd.clear();
}

void loop() {
  delay(100);
  radio.startListening();
  while (!radio.available());
  radio.read(&received_data, sizeof(Potvalue));
  sensor1 = received_data.pot1;
  sensor2 = received_data.pot2;
  sensor3 = received_data.pot3;
  sensor4 = received_data.pot4;


 // Serial.print("MOTOR1: ");
 // Serial.print(sensor1);
 // Serial.print("  VOLTAGE: ");
  //Serial.print(sensor3);
 // Serial.print("MOTOR2: ");
 // Serial.print(sensor2);
 // Serial.print("  VOLTAGE: ");
 // Serial.println(sensor4);
  delay(100);

  byte button1State = digitalRead(4);
  byte button2State = digitalRead(5);
  byte button3State = digitalRead(6);
  send_data.button1 = button1State;
  send_data.button2 = button2State;
  send_data.button3 = button3State;
  radio.stopListening();
  radio.write(&send_data, sizeof(Data_send));

  lcd.clear();
   lcd.setCursor(0, 3); // Set cursor to the first row, first column
  lcd.print(" UITS SCADA SYSTEM");
  lcd.setCursor(0, 1); // Set cursor to the first row, first column
  lcd.print("MOTOR1: ");
  //lcd.setCursor(8, 1);
  lcd.print(sensor2,1);
  lcd.setCursor(11, 1);
  lcd.print(" A ");
 // lcd.setCursor(14, 1);
  lcd.print(sensor3,1);
  lcd.setCursor(16, 1);
  lcd.print(" V");

  lcd.setCursor(0, 2); // Set cursor to the first row, first column
  lcd.print("MOTOR2: ");
  //lcd.setCursor(8, 2);
  lcd.print(sensor1,1);
  lcd.setCursor(11, 2);
  lcd.print(" A ");
 // lcd.setCursor(14, 2);
  lcd.print(sensor4,1);
  lcd.setCursor(16, 2);
  lcd.print(" V");;

  
  //delay(100);
}
