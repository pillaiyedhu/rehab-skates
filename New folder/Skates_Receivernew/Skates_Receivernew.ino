#include<SPI.h>
#include<RF24.h>
#include<nRF24L01.h>
#define enA 10
#define in1 9
#define in2 6
#define in3 5
#define in4 4
#define enB 3
int motorSpeedA=0;
int motorSpeedB=0;


const byte address[6]="00001";
RF24 radio(7,8);

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(enA,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enB,OUTPUT);
}

void loop() 
{
  if(radio.available())
  {
    int receivedDataX;
    radio.read(&receivedDataX, sizeof(receivedDataX));
    delay(250);
    int receivedDataY;
    radio.read(&receivedDataY,sizeof(receivedDataY));
    delay(250);
  

//    radio.read(&name,sizeof(name));
//    Serial.print(name);
    Serial.print("X axis: ");
    Serial.print(receivedDataX);
    Serial.print("|");
    
    Serial.print("Y axis: ");    
    Serial.print(receivedDataY);
    Serial.println("|");

    if(receivedDataY>564)
    {
      motorSpeedA=map(receivedDataY,564,1023,0,255);
      motorSpeedB=map(receivedDataY,564,1023,0,255);

      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
    }
     //Reverse Direction
    else if(receivedDataY<460)
    {
      receivedDataY=receivedDataY-460;
      receivedDataY=receivedDataY*-1;

      motorSpeedA=map(receivedDataY,0,460,0,255);
      motorSpeedB=map(receivedDataY,0,460,0,255);

      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      digitalWrite(in3,LOW);
      digitalWrite(in4,HIGH);      
    }
    else
    {
      motorSpeedA=0;
      motorSpeedB=0;
    } 

      //Right Direction
      
    if(receivedDataX<460)
    { 
      receivedDataX=receivedDataX-460;
      receivedDataX=receivedDataX *-1;

      receivedDataX=map(receivedDataX,0,460,0,255);

      //Left Motor(Motor A) faster than Right Motor(Motor B)
      motorSpeedA=motorSpeedA+receivedDataY;
      motorSpeedB=motorSpeedB-receivedDataX;

      if(motorSpeedA<0)
        motorSpeedA=0;
      if(motorSpeedB>255)
        motorSpeedB=255;
    }

      //Left Direction
    else if(receivedDataX>564)
    {
      receivedDataX=map(receivedDataX,564,1023,0,255);
    
      //Left Motor(Motor A) slower than Right Motor(Motor B)
      motorSpeedA=motorSpeedA-receivedDataX;
      motorSpeedB=motorSpeedB+receivedDataX;

      if(motorSpeedA>255)
        motorSpeedA=255;
      if(motorSpeedB<0)
        motorSpeedB=0;
      
    }
    //To remove buzzing sound of motors
    if(motorSpeedA<8)
      motorSpeedA=0;
    if(motorSpeedB<8)
      motorSpeedB=0;

   //Writing Speed to motors
    analogWrite(enA,motorSpeedA);
    analogWrite(enB,motorSpeedB); 
    
  }
}
