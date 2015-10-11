/*
  Nano relay usb
 Turns on d2,d3,d4,d5 for 4 seconds then off - simulates button on the front of a computer 
 Created by Bob Wickson
 2014/06/24 - Version 1.0
 
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int off_time = 4000;
int reset_time = 250;
int val = 0;

int button1 = 1; // this should be a0
int button2 = 2; // this should be a1
int button3 = 3; // this should be a2
int button4 = 4; // this should be a3
int button5 = 5; // this should be d10
int button6 = 6; // this should be MOSI
int button7 = 7; // this should be MISO
int button8 = 8; // this should be SCK

int relay1 = 2; // this should be d2
int relay2 = 3; // this should be d3
int relay3 = 4; // this should be d4
int relay4 = 5; // this should be d5
int relay5 = 6; // this should be d6
int relay6 = 7; // this should be d7
int relay7 = 8; // this should be d8
int relay8 = 9; // this should be d9

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
  pinMode(relay5,OUTPUT);
  pinMode(relay6,OUTPUT);
  pinMode(relay7,OUTPUT);
  pinMode(relay8,OUTPUT);
  
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  pinMode(button5,INPUT);
  pinMode(button6,INPUT);
  pinMode(button7,INPUT);
  pinMode(button8,INPUT);
  
  digitalWrite(led, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(relay5, LOW);
  digitalWrite(relay6, LOW);
  digitalWrite(relay7, LOW);
  digitalWrite(relay8, LOW);
  // initialize communication
  int n = 0;
  Serial.begin(9600);
  while(n != 58){ // this is the code for a ':'
    Serial.print(n);
    Serial.println("Cluster-CTRL");
    Serial.println("Relay board. Waiting for reply of ':'");
    if(Serial.available()){
      n = Serial.read();
    }
  }
}

// the loop routine runs over and over again forever:
void loop() {
  int n = 0;
  // read from the usb port
  if(Serial.available()){
    n = Serial.read();
    if ( n > 57 ) {
      // Serial.println(n); // uncomment for echo back to contol server
    }
  }
  

  // ok now we have the relay number turn on for 4 seconds  
  switch (n){
  case 65: // relay1 -shft a-
    Serial.println("Relay 1 - timing");
    digitalWrite(relay1, HIGH);
    delay(off_time);
    digitalWrite(relay1, LOW);
    break;
  case 97: // relay1 a
    Serial.println("Relay 1 - pulse");
    digitalWrite(relay1, HIGH);
    delay(reset_time);
    digitalWrite(relay1, LOW);
    break;
  case 83: // status #1 -shft s- ON LED
    val = digitalRead(button1);
    Serial.print("Status #1 is = ");
    Serial.println(val);
    break;

  case 66: // relay2 -shft b-
    Serial.println("Relay 2 - timing");
    digitalWrite(relay2, HIGH);
    delay(off_time);
    digitalWrite(relay2, LOW);
    break;
  case 98: // relay1 b
    Serial.println("Relay 2 - pulse");
    digitalWrite(relay2, HIGH);
    delay(reset_time);
    digitalWrite(relay2, LOW);
    break;
  case 84: // status #2 -shft t- ON LED
    val = digitalRead(button2);
    Serial.print("Status #2 is = ");
    Serial.println(val);
    break;

  case 67: // relay3 -shft c-
    Serial.println("Relay 3 - timing");
    digitalWrite(relay3, HIGH);
    delay(off_time);
    digitalWrite(relay3, LOW);
    break;
  case 99: // relay3 c
    Serial.println("Relay 3 - pulse");
    digitalWrite(relay3, HIGH);
    delay(reset_time);
    digitalWrite(relay3, LOW);
    break;
  case 85: // status #3 -shft u- ON LED
    val = digitalRead(button3);
    Serial.print("Status #3 is = ");
    Serial.println(val);
    break;

  case 68: // relay4 -shft d-
    Serial.println("Relay 4 - timing");
    digitalWrite(relay4, HIGH);
    delay(off_time);
    digitalWrite(relay4, LOW);
    break;
  case 100: // relay4 d
    Serial.println("Relay 4 - pulse");
    digitalWrite(relay4, HIGH);
    delay(reset_time);
    digitalWrite(relay4, LOW);
    break;
  case 86: // status #4 -shft v- ON LED
    val = digitalRead(button4);
    Serial.print("Status #4 is = ");
    Serial.println(val);
    break;

  case 69: // relay5 -shft e-
    Serial.println("Relay 5 - timing");
    digitalWrite(relay5, HIGH);
    delay(off_time);
    digitalWrite(relay5, LOW);
    break;
  case 101: // relay5 e
    Serial.println("Relay 5 - pulse");
    digitalWrite(relay5, HIGH);
    delay(reset_time);
    digitalWrite(relay5, LOW);
    break;
  case 87: // status #5 -shft w- ON LED
    val = digitalRead(button5);
    Serial.print("Status #5 is = ");
    Serial.println(val);
    break;

  case 70: // relay6 -shft f-
    Serial.println("Relay 6 - timing");
    digitalWrite(relay6, HIGH);
    delay(off_time);
    digitalWrite(relay6, LOW);
    break;
  case 102: // relay6 f
    Serial.println("Relay 6 - pulse");
    digitalWrite(relay6, HIGH);
    delay(reset_time);
    digitalWrite(relay6, LOW);
    break;
  case 88: // status #6 -shft x- ON LED
    val = digitalRead(button6);
    Serial.print("Status #6 is = ");
    Serial.println(val);
    break;

  case 71: // relay7 -shft g-
    Serial.println("Relay 7 - timing");
    digitalWrite(relay7, HIGH);
    delay(off_time);
    digitalWrite(relay7, LOW);
    break;
  case 103: // relay7 g
    Serial.println("Relay 7 - pulse");
    digitalWrite(relay7, HIGH);
    delay(reset_time);
    digitalWrite(relay7, LOW);
    break;
  case 89: // status #7 -shft y- ON LED
    val = digitalRead(button7);
    Serial.print("Status #7 is = ");
    Serial.println(val);
    break;

  case 72: // relay8 -shft h-
    Serial.println("Relay 8 - timing");
    digitalWrite(relay8, HIGH);
    delay(off_time);
    digitalWrite(relay8, LOW);
    break;
  case 104: // relay8 h
    Serial.println("Relay 8 - pulse");
    digitalWrite(relay8, HIGH);
    delay(reset_time);
    digitalWrite(relay8, LOW);
    break;
  case 90: // status #8 -shft z- ON LED
    val = digitalRead(button8);
    Serial.print("Status #8 is = ");
    Serial.println(val);
    break;

  case 58: // reply that we are  in the control loop
    Serial.println("Cluster-CTRL");
    break;
    
  case 63:
    Serial.println("-- Commands for Cluster control --");
    Serial.println(" ");
    Serial.println("Pulse relay #1 - a  #97");
    Serial.println("Pulse relay #2 - b  #98");
    Serial.println("Pulse relay #3 - c  #99");
    Serial.println("Pulse relay #4 - d #100");
    Serial.println("Pulse relay #5 - e #101");
    Serial.println("Pulse relay #6 - f #102");
    Serial.println("Pulse relay #7 - g #103");
    Serial.println("Pulse relay #8 - h #104");
    Serial.println(" ");
    Serial.println("Timing relay #1 - A #65");
    Serial.println("Timing relay #1 - B #66");
    Serial.println("Timing relay #1 - C #67");
    Serial.println("Timing relay #1 - D #68");
    Serial.println("Timing relay #1 - E #69");
    Serial.println("Timing relay #1 - F #70");
    Serial.println("Timing relay #1 - G #71");
    Serial.println("Timing relay #1 - H #72");
    Serial.println(" ");
    Serial.println("Status #1 - S #83 ");
    Serial.println("Status #1 - T #84");
    Serial.println("Status #1 - U #85");
    Serial.println("Status #1 - V #86");
    Serial.println("Status #1 - W #87");
    Serial.println("Status #1 - X #88");
    Serial.println("Status #1 - Y #89");
    Serial.println("Status #1 - Z #90");
    Serial.println(" ");
    
  
  default:
    //Serial.println(n);
    digitalWrite(led, HIGH); 
    delay(250);              
    digitalWrite(led, LOW);
    delay(250);
  }
}


