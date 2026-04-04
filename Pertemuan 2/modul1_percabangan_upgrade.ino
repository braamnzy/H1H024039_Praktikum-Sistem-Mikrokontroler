const int ledPin = 6; 
int timeDelay = 1000;
int step = -100; // step perubahan delay

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, HIGH);
    delay(timeDelay);
    
    digitalWrite(ledPin, LOW);
    delay(timeDelay);
    
    timeDelay += step; // timeDelay dijumlahkan dengan step untuk mengubah delay

    if (timeDelay <= 100) {
        step = 100; // ketika mencapai timeDelay 100, step diubah menjadi positif untuk meningkatkan delay
    } 

    if (step == 100 && timeDelay >= 700) {
        digitalWrite(ledPin, LOW);
        delay(3000);
        step = -100; // ketika mencapai timeDelay 700, step diubah menjadi negatif untuk menurunkan delay
        timeDelay = 1000;
    }
}