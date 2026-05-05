#include <Arduino.h>

const int potPin = A0;   // pin analog potensiometer
const int ledPin = 9;    // pin PWM LED

int nilaiADC = 0;  // nilai pembacaan ADC
int pwm = 0;       // nilai PWM

void setup() {

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  nilaiADC = analogRead(potPin);

  pwm = map(nilaiADC,
            0,     // ADC min
            1023,  // ADC max
            0,     // PWM min
            255    // PWM max
            );

  analogWrite(ledPin, pwm);

  Serial.print("ADC: ");
  Serial.print(nilaiADC);

  Serial.print(" | PWM: ");
  Serial.println(pwm);

  delay(50);
} 
