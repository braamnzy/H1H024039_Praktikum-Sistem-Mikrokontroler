const int ledPin = 6;
int timeDelay = 1000;

void setup() {
    pinMode(ledPin, OUTPUT); 
}

void loop() {
    digitalWrite(ledPin, HIGH);
    delay(timeDelay);
    
    digitalWrite(ledPin, LOW);
    delay(timeDelay);
    
    if (timeDelay <= 100) {
        delay(3000); 
        timeDelay = 1000; 
    } else {
        timeDelay -= 100; 
    }
}
