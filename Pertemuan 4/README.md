# Laporan Praktikum Modul 4 - Analog to Digital Converter (ADC) dan Pulse Width Modulation (PWM)
Percobaan 4A berfokus pada pengendalian posisi mekanis motor servo melalui pembacaan tegangan analog potensiometer yang dikonversi menjadi data digital 10-bit ($0$–$1023$) dan dipetakan ke skala sudut $0$°–$180$°, sedangkan Percobaan 4B menerapkan teknik Pulse Width Modulation (PWM) untuk mengatur intensitas cahaya LED secara bertahap dengan memetakan nilai ADC ke dalam rentang duty cycle 8-bit ($0$–$255$). Kedua percobaan ini secara kolektif mendemonstrasikan bagaimana mikrokontroler menjembatani fenomena fisik kontinu dengan pemrosesan digital diskrit untuk menghasilkan aksi output yang presisi dan linier.

## Pertanyaan Percobaan 4A: Analog to Digital Converter

1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini? <br>
**Jawab:** Fungsi perintah analogRead() pada praktikum 4A adalah untuk membaca tegangan analog dari potensiometer melalui pin A0 dan mengonversinya menjadi nilai digital 10-bit dengan rentang 0 hingga 1023. Data digital ini mewakili posisi fisik putaran potensiometer yang kemudian dipetakan (mapping) menjadi instruksi sudut 0 hingga 180 derajat untuk menggerakkan motor servo secara presisi.


2. Mengapa diperlukan fungsi map() dalam program tersebut? <br>
**Jawab:** Fungsi perintah map() dalam program percobaan 4A adalah untuk mentransformasikan skala data dari hasil pembacaan input analog menjadi skala kendali yang sesuai untuk perangkat output. Karena unit ADC mikrokontroler menghasilkan nilai digital antara 0 hingga 1023, sedangkan library motor servo memerlukan instruksi dalam rentang sudut 0 hingga 180 derajat, fungsi ini diperlukan untuk melakukan normalisasi nilai secara linier. Dengan adanya fungsi map(), setiap perubahan posisi putaran potensiometer dapat diterjemahkan secara akurat menjadi posisi sudut poros motor servo yang sinkron dan presisi.

3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. <br>
**Jawab:**
``` cpp
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // pin analog input
const int servoPin = 9;       // pin PWM servo

// ===================== VARIABEL =====================
int pos = 0; // sudut servo
int val = 0; // nilai ADC

void setup() {
  // Hubungkan servo ke pin
  myservo.attach(servoPin);

  // Serial monitor
  Serial.begin(9600);
}

void loop() {
  // ===================== PEMBACAAN ADC =====================
  val = analogRead(potensioPin);

  // ===================== KONVERSI DATA (MODIFIKASI) =====================
  // Mengubah rentang output dari 0-180 menjadi 30-150
  pos = map(val,
            0,    // min ADC tetap
            1023, // max ADC tetap
            30,   // sudut min baru
            150   // sudut max baru
            );

  // ===================== OUTPUT SERVO =====================
  myservo.write(pos);

  // ===================== MONITORING =====================
  Serial.print("ADC Potensio: ");
  Serial.print(val);

  Serial.print(" | Sudut Servo (Terbatas): ");
  Serial.println(pos);

  // ===================== STABILISASI =====================
  delay(100);
}
```

## Pertanyaan Percobaan 4B: Pulse Width Modulation

1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()! <br>
**Jawab:** LED dapat diatur kecerahannya karena fungsi analogWrite() menerapkan teknik Pulse Width Modulation (PWM) yang memodulasi lebar pulsa digital (duty cycle) dalam satu periode waktu tertentu. Dengan mengatur rasio perbandingan antara durasi sinyal aktif (ON) dan non-aktif (OFF), mikrokontroler mampu menghasilkan tegangan rata-rata efektif yang berbeda-beda sehingga seolah-olah menghasilkan sinyal analog. Hal ini memungkinkan intensitas cahaya LED berubah secara bertahap dari redup ke terang sesuai dengan besar energi rata-rata yang disalurkan ke komponen tersebut.

2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)? <br>
**Jawab:** Hubungan antara keduanya adalah hubungan linear proporsional yang dijembatani melalui proses pemetaan (mapping) data. Nilai ADC merupakan representasi digital 10-bit dari tegangan input (0–5V), sedangkan nilai PWM adalah parameter 8-bit yang menentukan besar duty cycle output. Dalam program, rentang ADC 0–1023 dipetakan ke rentang PWM 0–255, yang berarti setiap kenaikan nilai pada potensiometer secara langsung akan meningkatkan nilai PWM dan memperbesar intensitas cahaya LED secara proporsional.

3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. <br>
**Jawab:**
``` cpp
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
            50,     // PWM min
            200    // PWM max
            );

  analogWrite(ledPin, pwm);

  Serial.print("ADC: ");
  Serial.print(nilaiADC);

  Serial.print(" | PWM: ");
  Serial.println(pwm);

  delay(50);
} 
```