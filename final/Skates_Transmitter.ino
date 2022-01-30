#include<SPI.h>
#include<RF24.h>
#include<nRF24L01.h>
int joystickX=A0;
int joystickY=A1;
int joystickXPos;
int joystickYPos;
int xyArr[2];

const byte address[6]="00001";
RF24 radio(7,8);

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop()
{  
  joystickXPos=analogRead(joystickX);  
  joystickYPos=analogRead(joystickY);

  xyArr[0]=joystickXPos;
  xyArr[1]=joystickYPos;
  
  radio.write(&xyArr,sizeof(xyArr));
 
  Serial.print("X axis: ");
  Serial.print(xyArr[0]);
  Serial.print("|");
  Serial.print("Y axis: ");
  Serial.print(xyArr[1]);
  Serial.println("|");
}
