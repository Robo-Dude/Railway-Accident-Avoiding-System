#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial sim(10, 11);

String number = "+8826885541";
String textMessage;
Servo myservo;

#define ledA1 12
#define ledA2 13
#define ledA3 A0

#define ledB1 8
#define ledB2 7
#define ledB3 6

#define faultSensor 4
int state1 = 0;
int state2 = 0;
int x, y, z;
int pos = 0;
void setup() {

  Serial.begin(9600);

  Serial.println("System Started.....");
  sim.begin(9600);
  delay(1000);

  sim.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);  
  myservo.attach(9);
  myservo.write(25);

  pinMode(faultSensor, INPUT);
  pinMode(ledA1, OUTPUT);
  pinMode(ledA2, OUTPUT);
  pinMode(ledA3, OUTPUT);

  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  
  pinMode(ledB1, OUTPUT);
  pinMode(ledB2, OUTPUT);
  pinMode(ledB3, OUTPUT);

  digitalWrite(ledA1, HIGH);
  digitalWrite(ledB3, HIGH);
}

void loop() {

  readSensor();

  if (state1 == 0 && x == 1)
  {
    servoUp();
    state1 = 1;
  }
  else if (state1 == 1 && y == 1)
  {
    servoDown();
    state1 = 0;
  }
  else if (state2 == 0 && z == 0)
  {
    faultON();
    sendMessage();
    state2 = 1;
  }
  else if (state2 == 1 && z == 1)
  {
    faultOFF();
    state2 = 0;
  }
}

void readSensor()
{
  z = digitalRead(faultSensor);
  Serial.println(z);
  x = analogRead(A4);
  y = analogRead(A5);

  if (x < 100) x = 1; else x = 0;
  if (y < 100) y = 1; else y = 0;
}

void servoUp()
{
  digitalWrite(ledA1, LOW);
  digitalWrite(ledA2, HIGH);
  delay(100);
  digitalWrite(ledA2, LOW);
  digitalWrite(ledA3, HIGH);
  for (pos = 25; pos <= 110; pos += 1)
  {
    myservo.write(pos);
    delay(5);
  }
}
void servoDown()
{
  digitalWrite(ledA3, LOW);
  digitalWrite(ledA2, HIGH);
  delay(200);
  digitalWrite(ledA2, LOW);
  digitalWrite(ledA1, HIGH);
  for (pos = 110; pos >= 25; pos -= 1)
  {
    myservo.write(pos);
    delay(5);
  }
}

void faultON()
{
  digitalWrite(ledB3, LOW);
  digitalWrite(ledB2, HIGH);
  delay(1000);
  digitalWrite(ledB2, LOW);
  digitalWrite(ledB1, HIGH);
}
void faultOFF()
{
  digitalWrite(ledB1, LOW);
  digitalWrite(ledB2, HIGH);
  delay(1000);
  digitalWrite(ledB2, LOW);
  digitalWrite(ledB3, HIGH);
}

void sendMessage()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number 
  
  
  
  + "\"\r");
  delay(1000);
  String SMS = "Rail line fault is detected near the New Delhi railway station";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}
