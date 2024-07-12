#include <Arduino.h>

// Define pin numbers for motor control
int m1_EL_Start_Stop = 5;  // Pin to control motor start/stop
int m1_Signal_hall = 3;    // Pin connected to the Hall sensor (not used in the current code)
int m1_ZF_Direction = 4;   // Pin to control motor direction
int m1_VR_speed = 2;       // Pin to control motor speed

void setup() {
  Serial.begin(115200);    // Initialize serial communication at 115200 baud rate

  // Set pin modes
  pinMode(m1_EL_Start_Stop, OUTPUT);
  pinMode(m1_VR_speed, OUTPUT);
  pinMode(m1_ZF_Direction, OUTPUT);
}

int speed = 30;            // Initialize default motor speed
bool isLeft = true;        // Direction flag
int turnDelay = 1000;      // Initialize turn delay

// Function to update motor control based on received command
void updateWheels(String data) {
  if (data == "start") {
    // Start motor
    Serial.println("Start");
    digitalWrite(m1_EL_Start_Stop, LOW);               
    digitalWrite(m1_ZF_Direction, isLeft ? LOW : HIGH); 
    analogWrite(m1_VR_speed, speed);                  
    digitalWrite(m1_EL_Start_Stop, HIGH);            
  } else if (data == "stop") {
    // Stop motor
    Serial.println("Stop");
    digitalWrite(m1_ZF_Direction, isLeft ? LOW : HIGH); 
    analogWrite(m1_VR_speed, speed);                  
    digitalWrite(m1_EL_Start_Stop, LOW);              
  } else if (data == "left") {
    // Turn left
    digitalWrite(m1_EL_Start_Stop, LOW);               
    digitalWrite(m1_ZF_Direction, HIGH);               
    analogWrite(m1_VR_speed, isLeft ? 0 : speed);      
    digitalWrite(m1_EL_Start_Stop, HIGH);            

    delay(turnDelay);                                  // Wait for turn to complete

    // Resume normal operation
    Serial.println("Start");
    digitalWrite(m1_EL_Start_Stop, LOW);              
    digitalWrite(m1_ZF_Direction, isLeft ? LOW : HIGH);
    analogWrite(m1_VR_speed, speed);                  
    digitalWrite(m1_EL_Start_Stop, HIGH);            
  } else if (data == "right") {
    // Turn right
    digitalWrite(m1_EL_Start_Stop, LOW);           
    digitalWrite(m1_ZF_Direction, LOW);               
    analogWrite(m1_VR_speed, !isLeft ? 0 : speed);    
    digitalWrite(m1_EL_Start_Stop, HIGH);              

    delay(turnDelay);                                  // Wait for turn to complete

    // Resume normal operation
    Serial.println("Start");
    digitalWrite(m1_EL_Start_Stop, LOW);           
    digitalWrite(m1_ZF_Direction, isLeft ? LOW : HIGH); 
    analogWrite(m1_VR_speed, speed);                  
    digitalWrite(m1_EL_Start_Stop, HIGH);             
  } else if (data.startsWith("speed")) {
    // Change speed
    digitalWrite(m1_EL_Start_Stop, LOW);               
    speed = data.substring(5).toInt();                 // Extract and convert new speed from command
    Serial.println("Changing speed to");
    Serial.println(speed);
    digitalWrite(m1_ZF_Direction, isLeft ? LOW : HIGH); 
    analogWrite(m1_VR_speed, speed);                  
    digitalWrite(m1_EL_Start_Stop, HIGH);             
  } else if (data.startsWith("turn")) {
    // Change turn delay
    turnDelay = data.substring(4).toInt();             // Extract and convert new turn delay from command
    Serial.println("Changing turn delay to");
    Serial.println(turnDelay);
  } else {
    // Handle other commands or invalid input
  }
}

void loop() {
  // Check if there is any serial input
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');        // Read incoming serial data until newline character
    updateWheels(data);                               
  }
}