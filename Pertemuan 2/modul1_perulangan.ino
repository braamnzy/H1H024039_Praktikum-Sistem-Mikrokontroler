int timer = 100;           
void setup() { 

output: 
  for (int ledPin = 2; ledPin < 8; ledPin++) { 
    pinMode(ledPin, OUTPUT); 
  } 
} 
void loop() { 
  for (int ledPin = 2; ledPin < 8; ledPin++) { 
    digitalWrite(ledPin, HIGH); 
    delay(timer); 
    digitalWrite(ledPin, LOW); 
  } 

  for (int ledPin = 7; ledPin >= 2; ledPin--) { 
    digitalWrite(ledPin, HIGH); 
    delay(timer); 
    digitalWrite(ledPin, LOW); 
  } 
} 
