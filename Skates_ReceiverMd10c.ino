#include<SPI.h>
#include<RF24.h>
#include<nRF24L01.h>

const int pwmA = 5;
const int dirA = 6;

const int pwmB = 9;
const int dirB = 10;

int motorSpeedA=0;
int motorSpeedB=0;

const byte address[6]="00001";
RF24 radio(7,8);

int xyRecArr[2];

void setup() {
 Serial.begin(9600);
 radio.begin();
 radio.openReadingPipe(0,address);
 radio.setPALevel(RF24_PA_MIN);
 radio.startListening();

 pinMode(pwmA,OUTPUT);
 pinMode(dirA,OUTPUT);
 pinMode(pwmB,OUTPUT);
 pinMode(dirB,OUTPUT);

 
}

void loop() {
  if(radio.available()){
    int receivedDataX;
    int receivedDataY;
    radio.read(&xyRecArr,sizeof(xyRecArr));  

    receivedDataX = xyRecArr[0];
    receivedDataY = xyRecArr[1];

    Serial.print("X axis: ");
    Serial.print(receivedDataX);
    Serial.print("|");
    
    Serial.print("Y axis: ");    
    Serial.print(receivedDataY);
    Serial.println("|");

    //forward direction
    if(receivedDataY>510){
      motorSpeedA = map(receivedDataY,510,1023,0,255);
      motorSpeedB = map(receivedDataY,510,1023,0,255);

      digitalWrite(dirA,HIGH);
      digitalWrite(dirB,HIGH);

      
    }
    
    else if(receivedDataY<505){
      receivedDataY=receivedDataY-505;
      receivedDataY=receivedDataY*-1;

      motorSpeedA=map(receivedDataY,0,505,0,255);
      motorSpeedB=map(receivedDataY,0,505,0,255);

      digitalWrite(dirA,LOW);
      digitalWrite(dirB,LOW);

      
    }
    else
    {
      motorSpeedA=0;
      motorSpeedB=0;
    } 

   //Right Direction
    if(receivedDataX<490){ 
      receivedDataX=receivedDataX-490;
      receivedDataX=receivedDataX *-1;

      receivedDataX=map(receivedDataX,0,490,0,255);

      //Left Motor(Motor A) faster than Right Motor(Motor B)
      motorSpeedA=motorSpeedA+receivedDataY;
      motorSpeedB=motorSpeedB-receivedDataX;

      if(motorSpeedA<0)
        motorSpeedA=0;
      if(motorSpeedB>255)
        motorSpeedB=255;
     }
     else if(receivedDataX>495)
     {
      receivedDataX=map(receivedDataX,495,1023,0,255);
    
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
    analogWrite(pwmA,motorSpeedA);
    analogWrite(pwmB,motorSpeedB);    

        
  }

}
