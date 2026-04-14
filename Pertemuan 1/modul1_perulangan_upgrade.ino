int timer = 500;           
void setup() { 

output: 
for (int ledPin = 2; ledPin < 8; ledPin++) { 
    pinMode(ledPin, OUTPUT); 
} 
} 
void loop() { 
for (int i = 2; i <= 4; i++) { 
    digitalWrite(i, HIGH);      // indeks i(2,3,4) sebagai ledPin akan menyala
    digitalWrite(i + 3, LOW);   // indeks i+3(5,6,7) sebagai ledPin akan mati
} 
delay(timer); 

for (int i = 2; i <= 4; i++) { 
    digitalWrite(i, LOW);       // indeks i(2,3,4) sebagai ledPin akan mati
    digitalWrite(i + 3, HIGH);  // indeks i+3(5,6,7) sebagai ledPin akan menyala
} 
delay(timer); 
} 