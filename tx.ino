#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

int analogInput1;


const byte addresses[][6] = {"00001", "00002"};
struct Data_to_be_sent {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
};

Data_to_be_sent sent_data;

struct received_butt {
  byte b1;
  byte b2;
  byte b3;
};

received_butt buttonstate;

int button1 =0;
int button2 =0;
int button3 =0;

int relay1 = 8; //motor1
int relay2 = 7; //motor2
int relay3 = 6;  ///ac lamp



void setup() {
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  //Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  
  sent_data.ch1 = 0;
  sent_data.ch2 = 0;
  sent_data.ch3 = 0;
  sent_data.ch4 = 0;
}

void loop() {
  delay(100);
  sent_data.ch1 = CURRENT(A1);     //a1                               //map(analogRead(A0), 0, 1023, 0, 255);
  sent_data.ch2 = CURRENT(A0);     //a0                               //map(analogRead(A1), 0, 1023, 0, 255);
  sent_data.ch3 = voltage1();                                    // map(analogRead(A2), 0, 1023, 0, 255);
  sent_data.ch4 = voltage2();                                    //map(analogRead(A3), 0, 1023, 0, 255);
  radio.stopListening();
  radio.write(&sent_data, sizeof(Data_to_be_sent));
  
  delay(100);
  
  radio.startListening();
  if(radio.available()){
  while(radio.available()){
    radio.read(&buttonstate, sizeof(received_butt));
   button1 = buttonstate.b1;
  button2 = buttonstate.b2;
  button3 =buttonstate.b3;

  }
  }
     motor1();
     motor2();
     lamp();
  
  //Serial.print("Button 1: ");
  //Serial.print(button1);
  //Serial.print(", Button 2: ");
  //Serial.print(button2);
 // Serial.print(", Button 3: ");
 // Serial.println(button3);
  delay(100);
}


///////////////////function//////////////
float CURRENT(int analogInput1){
   int adc = analogRead(A0);
  float voltage = adc*5/1023.0;
  float current = (voltage-2.5)/0.185;

return current; 
}

float voltage1(){
  float adc_value1 = analogRead(A3);
   float adc_voltage1  = ((adc_value1 * 5.0) / 1024.0); 
   float volt1 = adc_voltage1 / (1.0/11.0) ; 
   //sent_data.ch3 = volt1;
   return volt1;
   
}

float voltage2(){
  float adc_value2 = analogRead(A2);
   float adc_voltage2  = ((adc_value2 * 5.0) / 1024.0); 
   float volt2 = adc_voltage2 / (1.0/11.0) ; 
   //sent_data.ch4 = volt2;
   return volt2;
   
}

void motor1(){
  if (button1 == 1){
    digitalWrite(relay1,LOW);
  }
  else{
    digitalWrite(relay1,HIGH);
  }
}

void motor2(){
  if (button2 == 1){
    digitalWrite(relay2,LOW);
  }
  else{
    digitalWrite(relay2,HIGH);
  }
}

void lamp(){
  if (button3 == 1){
    digitalWrite(relay3,LOW);
  }
  else{
    digitalWrite(relay3,HIGH);
  }
}
