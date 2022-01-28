#include<SPI.h>
#include<RF24.h>
#include<nRF24L01.h>
const byte address[6]="00001";
RF24 radio(7,8);
//char receivedData[32]="";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  while(radio.available()){
    int number;
    radio.read(&number,sizeof(number));
    Serial.println(number);
    
    
  }

}
