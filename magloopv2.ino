#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX

unsigned int raw=0;
float volt=0.0;
float ARef = 1.1;

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
#define movesteps 100
#define movesteps1 1

char lastValue;         
char blueToothVal; 

int Steps = 0;
boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=movesteps;
int steps_left1=movesteps1;

long previousMillis = 0;
long interval = 1000;

long time1;

void setup()
{
Serial.begin(9600);
pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT); 
motoroff();
pinMode(A0, INPUT);
BTserial.begin(9600); 

analogReference(INTERNAL);
 
}

void loop()
{
  if(BTserial.available())
  {
    blueToothVal=BTserial.read();
  }
  if (blueToothVal=='F')
  {
  stepforward();
  motoroff();

//    Serial.println("step forward");
  }

  if (blueToothVal=='B')
  {
  stepbackward();
  //  Serial.println("step back");
  motoroff();

  }
  
  if (blueToothVal=='G')
  {
  forward();
  //Serial.println("forward");
  motoroff();

  }
  
  if (blueToothVal=='N')
  {
  backward();
  //    Serial.println("backward");
  motoroff();

  }

  if (blueToothVal=='X')
  {
  motoroff();
  //  Serial.println("stop");
  }
    
 // delay(5);

unsigned long currentMillis = millis();
if(currentMillis - previousMillis > interval) {
 volts();
  previousMillis = currentMillis;
}
  }

void stepforward(){
    Direction=0;
  while(steps_left1>0){
      currentMillis = micros();
  
  if(currentMillis-last_time>=5000){
    stepper(1); 
    time1=time1+micros()-last_time;
    last_time=micros();
    steps_left1--;
    delay(100);
    }
  }
  steps_left1=movesteps1;
  // motoroff();
}


void stepbackward(){
    Direction=1;
  while(steps_left1>0){
     currentMillis = micros();
  
  if(currentMillis-last_time>=5000){
    stepper(1); 
    time1=time1+micros()-last_time;
    last_time=micros();
    steps_left1--;
    delay(100);
    }
  }
  steps_left1=movesteps1;
  //motoroff();
}

void forward(){
    Direction=0;
  while(steps_left>0){
      currentMillis = micros();
  
  if(currentMillis-last_time>=1600){
    stepper(1); 
    time1=time1+micros()-last_time;
    last_time=micros();
    steps_left--;
    delay(1);
//  Serial.println(steps_left);
    }
  }
  steps_left=movesteps;
   //motoroff();
}

void backward(){
   Direction=1;
  while(steps_left>0){
    
  currentMillis = micros();
  
  if(currentMillis-last_time>=1600){
    stepper(1); 
    time1=time1+micros()-last_time;
    last_time=micros();
    steps_left--;
    delay(1);
//  Serial.println(steps_left);

    }
  }
  steps_left=movesteps;
  //motoroff();
}

void motoroff(){
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
}



void stepper(int xw){
  for (int x=0;x<xw;x++){
switch(Steps){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 2:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
}
SetDirection();
}
}


void SetDirection(){
if(Direction==1){ Steps++;}
if(Direction==0){ Steps--; }
if(Steps>3){Steps=0;}
if(Steps<0){Steps=3; }
}

void volts(){
float raw = analogRead(A0);

float x = raw/1023;
float volt = x*ARef*4.26;
//String v=String(volt);// change float into string
BTserial.print(volt);
BTserial.println("V");

}
