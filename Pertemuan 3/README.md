# Laporan Praktikum Modul 3 - Protokol Komunikasi
## Pertanyaan Percobaan 3A: Komunikasi Serial

1. Jelaskan proses dari input keyboard hingga LED menyala/mati!<br>
    **Jawab:** Proses ini melibatkan beberapa tahapan transmisi data dan logika pemrograman:

        a. Input (User): Pengguna mengetik karakter (misal: '1') di Serial Monitor dan menekan Enter.

        b. Transmisi (PC): Komputer mengubah karakter tersebut menjadi kode biner (ASCII) dan mengirimkannya melalui kabel USB bit demi bit menggunakan protokol UART.

        c. Buffer (Arduino): Mikrokontroler menerima data tersebut melalui pin RX dan menyimpannya sementara di dalam Serial Receive Buffer.

        d. Pemrosesan (Code): Fungsi Serial.read() mengambil data dari buffer. Nilai karakter tersebut kemudian dibandingkan di dalam struktur kontrol (if atau switch-case).

        e. Output (Hardware): Jika karakter sesuai (misal '1'), mikrokontroler mengirim sinyal HIGH ke Pin 8. Arus mengalir ke LED, dan LED menyala.

2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?<br>
   **Jawab:** Fungsi ini digunakan untuk mengecek apakah ada data yang masuk di dalam buffer serial. Ini memastikan bahwa fungsi Serial.read() hanya bekerja jika ada data yang benar-benar siap dibaca. Jika baris ini tidak ada, fungsi Serial.read() akan terus dieksekusi oleh program pada setiap putaran loop(). Akibatnya, fungsi tersebut akan mengembalikan nilai -1 (karena tidak ada data), yang dapat mengganggu logika program dan menyebabkan perilaku LED yang tidak terduga atau pesan error yang muncul terus-menerus.

3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!<br>
   **Jawab:** 
``` cpp
#include <Arduino.h>

const int ledPin = 8;     // Mendefinisikan pin LED
char command;             // Variabel penyimpan perintah
int mode = 0;             // 0: OFF, 1: ON, 2: BLINK

void setup() {
  pinMode(ledPin, OUTPUT); // Mengatur pin 8 sebagai output
  Serial.begin(9600);      // Memulai komunikasi serial baud rate 9600
  Serial.println("Ketik 0:OFF, 1:ON, 2:BLINK");
}

void loop() {
  // Mengecek apakah ada data serial yang masuk
  if (Serial.available() > 0) {
    command = Serial.read(); // Membaca karakter input
    
    // Logika pemilihan mode berdasarkan input
    if (command == '1') mode = 1;
    else if (command == '0') mode = 0;
    else if (command == '2') mode = 2;
    else Serial.println("Error: Perintah tidak dikenal!");
  }

  // Eksekusi mode yang terpilih
  if (mode == 1) {
    digitalWrite(ledPin, HIGH); // LED menyala statis
  } 
  else if (mode == 0) {
    digitalWrite(ledPin, LOW);  // LED mati statis
  } 
  else if (mode == 2) {
    // Logika blink menggunakan millis agar tetap responsif
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink >= 500) {
      digitalWrite(ledPin, !digitalRead(ledPin)); // Toggle status LED
      lastBlink = millis();
    }
  }
}

```

4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap
sistem<br>
    **Jawab:** Menggunakan millis(). Jika menggunakan delay(500) untuk membuat LED berkedip, mikrokontroler akan "berhenti" bekerja selama durasi tersebut. Akibatnya, jika pengguna mengirim perintah '0' untuk mematikan LED saat sedang dalam jeda delay, mikrokontroler tidak akan bisa membaca input tersebut sampai delay selesai. Sistem menjadi tidak responsif. Dengan millis(), program hanya mengecek selisih waktu sambil terus menjalankan perulangan loop(). Ini memungkinkan mikrokontroler untuk tetap mengecek Serial.available() setiap saat. Hasilnya, sistem menjadi sangat adaptif dan responsif terhadap perubahan perintah meskipun LED sedang dalam kondisi berkedip.

## Pertanyaan Percobaan 3B: I2C

1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian
tersebut!<br>
    **Jawab:** Komunikasi I2C (Inter-Integrated Circuit) pada rangkaian ini bekerja dengan prinsip Master-Slave menggunakan dua jalur kabel utama:

      a. SDA (Serial Data): Jalur untuk mengirimkan paket data (karakter dan perintah instruksi) secara bergantian. <br>
      b. SCL (Serial Clock): Jalur detak yang dihasilkan oleh Arduino (Master) untuk menyinkronkan pengiriman data.

    Alurnya dimulai dengan Arduino mengirimkan data yang berisi alamat unik LCD (misal: 0x27). Modul I2C pada LCD menerima data serial tersebut, lalu IC PCF8574 di dalamnya mengubah data serial menjadi data paralel yang dipahami oleh kontroler LCD untuk menyalakan pixel tertentu sesuai karakter yang diperintahkan.

2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan
pin kanan tertukar!<br>
   **Jawab:** Ya, Pin tengah wajib ke pin Analog (A0) karena pin tengah adalah wiper yang nilai tegangannya berubah-ubah. Namun, pin kiri dan kanan adalah jalur daya (VCC dan GND). <br> Jika pin kiri dan kanan tertukar, Secara fungsional tidak akan merusak komponen, tetapi arah pembacaan akan terbalik. Jika sebelumnya diputar searah jarum jam nilai ADC bertambah (0 ke 1023), setelah tertukar, diputar searah jarum jam justru nilai ADC akan berkurang (1023 ke 0).

3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai
output) sehingga:
- Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor
- Adapun data yang ditampilkan pada Serial Monitor sesuai dengan table berikut:
ADC: 0 Volt: 0.00 V Persen: 0%
Tampilan jika potensiometer dalam kondisi diputar paling kiri
- ADC: 0 0% | setCursor(0, 0) dan Bar (level) | setCursor(0, 1)
- Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!<br>
   **Jawab:**
```cpp
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD pada alamat 0x27, ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600); // Memulai UART untuk Serial Monitor
  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Menyalakan lampu latar LCD
}

void loop() {
  int adcVal = analogRead(A0);           // Membaca nilai potensio (0-1023)
  float volt = adcVal * (5.0 / 1023.0);  // Konversi ke Volt
  int persen = map(adcVal, 0, 1023, 0, 100); // Konversi ke persen
  int barLen = map(adcVal, 0, 1023, 0, 16);  // Konversi ke panjang bar (16 kolom)

  // --- Output ke LCD ---
  lcd.setCursor(0, 0);
  lcd.print("ADC: "); lcd.print(adcVal);
  lcd.print(" "); lcd.print(persen); lcd.print("%   ");
  
  lcd.setCursor(0, 1);
  for(int i=0; i<16; i++){
    if(i < barLen) lcd.print((char)255); // Karakter blok penuh
    else lcd.print(" ");                 // Spasi kosong
  }

  // --- Output ke Serial Monitor ---
  Serial.print("ADC: "); Serial.print(adcVal);
  Serial.print(" Volt: "); Serial.print(volt); Serial.print(" V");
  Serial.print(" Persen: "); Serial.print(persen); Serial.println("%");

  delay(200); // Jeda pembaruan data
}
```

4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor <br>
**Jawab:** <br>

| No | Nilai ADC | Volt (V) | Persen (%) |
|:---:|:---------:|:--------:|:----------:|
| 1 | 1 | 0.00 V | 0% |
| 2 | 21 | 0.10 V | 2% |
| 3 | 49 | 0.24 V | 5% |
| 4 | 74 | 0.36 V | 7% |
| 5 | 96 | 0.47 V | 9% |