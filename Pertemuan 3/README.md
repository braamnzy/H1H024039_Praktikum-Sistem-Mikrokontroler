# Laporan Praktikum Modul 2 - Pemrograman GPIO
## Pertanyaan Percobaan 2A: Seven Segment

1. Gambarkan Rangkaian Schematic pada Percobaan!
    **Jawab:** ![alt text](dokumentasi/image.png)

2. Apa yang terjadi jika nilai variabel `num` lebih dari 15 pada fungsi `displayDigit`?
   **Jawab:** Jika nilai `num` melebihi 15, program akan mengakses indeks memori di luar batas array `digitPattern` (yang hanya didefinisikan dari 0 hingga 15), sehingga Seven Segment akan menampilkan pola acak atau data sampah dari memori mikrokontroler yang tidak merepresentasikan karakter hexadesimal yang valid.

3. Apakah program ini menggunakan Seven Segment tipe Common Cathode atau Common Anode? Jelaskan alasannya berdasarkan kode!
   **Jawab:** Program ini menggunakan tipe **Common Anode** karena pada fungsi `displayDigit` terdapat operator NOT (`!`) pada perintah `digitalWrite(segmentPins[i], !digitPattern[num][i])`, yang berarti logika '1' pada pola diubah menjadi 'LOW' (0V) agar LED menyala, sesuai dengan prinsip kerja Common Anode yang membutuhkan sinyal LOW untuk mengaktifkan segmen.

4. Modifikasi Program Alur Tampilan F ke 0
```cpp
void loop() {
  // Melakukan perulangan mundur dari indeks 15 (F) hingga 0 (0)
  for(int i = 15; i >= 0; i--) { 
    displayDigit(i); // Memanggil fungsi untuk menampilkan karakter berdasarkan indeks i
    delay(1000);     // Memberikan jeda waktu 1 detik sebelum berganti karakter
  }
}
```

## Pertanyaan Percobaan 2B: Kontrol Counter

1. Gambarkan Rangkaian Schematic pada Percobaan!
    **Jawab:** ![alt text](dokumentasi/image1.png)

2. Mengapa pada push button digunakan mode `INPUT_PULLUP` pada Arduino Uno? Apa keuntungannya?
   **Jawab:** Penggunaan `INPUT_PULLUP` bertujuan untuk mengaktifkan resistor internal Arduino yang menarik tegangan pin ke posisi HIGH (5V) secara default, sehingga memberikan keuntungan berupa penghematan komponen karena tidak memerlukan resistor eksternal dan mencegah terjadinya kondisi *floating* (sinyal mengambang) saat tombol tidak ditekan.

3. Jika salah satu LED segmen tidak menyala saat counter berjalan, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?
   **Jawab:** Dari sisi hardware, kemungkinan penyebabnya adalah kabel jumper yang putus, resistor yang longgar, atau kerusakan fisik pada segmen LED tersebut, sedangkan dari sisi software, hal ini bisa disebabkan oleh kesalahan pemetaan pin pada array `segmentPins` atau kesalahan penulisan nilai '1' dan '0' pada array `digitPattern` untuk digit tertentu.

4. Modifikasi Increment & Decrement
```cpp
void loop() {
  bool upState = digitalRead(buttonUp); // Menyimpan kondisi logika terkini dari tombol penambah.
  bool downState = digitalRead(buttonDown); // Menyimpan kondisi logika terkini dari tombol pengurang.

  // Logika Increment apabila tombol penambah ditekan.
  if (lastUpState == HIGH && upState == LOW) { 
    counter++;
    if(counter > 15) counter = 0;
    displayDigit(counter);
    delay(200); 
  }
  
  // Logika Decrement apabila tombol pengurang ditekan.
  if (lastDownState == HIGH && downState == LOW) {
    counter--;
    if(counter < 0) counter = 15;
    displayDigit(counter);
    delay(200);
  }

  lastUpState = upState; // apabila tombol penambah ditekan, kondisi nya akan disimpan ke lastUpState
  lastDownState = downState; // apabila tombol pengurang ditekan, kondisi nya akan disimpan ke lastUpState
}