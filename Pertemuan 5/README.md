# Laporan Praktikum Modul 5 - Real Time Operating System

## Pertanyaan Percobaan 5A: Multitasking

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! <br>
**Jawab:** Ketiga task berjalan secara concurrent (seolah-olah bersamaan), tetapi pada mikrokontroler dengan satu inti CPU seperti Arduino Arduino Uno sebenarnya task dieksekusi bergantian sangat cepat oleh scheduler dari FreeRTOS. Scheduler membagi waktu CPU menggunakan mekanisme task switching berdasarkan prioritas dan delay. Saat sebuah task menjalankan vTaskDelay(), task tersebut masuk status blocked sehingga CPU diberikan ke task lain. Karena pergantiannya sangat cepat, ketiga task terlihat berjalan bersamaan.


2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya! <br>
**Jawab:** Untuk menambahkan task keempat, pertama buat fungsi baru misalnya void TaskSensor(void *pvParameters) yang berisi kode task tambahan. Setelah itu, tambahkan xTaskCreate() di dalam setup() seperti task lainnya, misalnya xTaskCreate(TaskSensor, "task4", 128, NULL, 1, NULL);. Setelah scheduler dijalankan dengan vTaskStartScheduler(), task baru otomatis ikut dijadwalkan oleh FreeRTOS bersama task lain.

3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md. <br>
**Jawab:**
Source code modifikasi dengan potensiometer untuk mengatur kecepatan LED: <br>
``` cpp
#include <Arduino_FreeRTOS.h>

// Deklarasi task
void TaskBlink(void *pvParameters);
void TaskPot(void *pvParameters);

// Variabel global untuk delay LED
int ledDelay = 200;

void setup() {
  Serial.begin(9600);

  // Membuat task LED
  xTaskCreate(TaskBlink, "BlinkLED", 128, NULL, 1, NULL);

  // Membuat task pembaca potensiometer
  xTaskCreate(TaskPot, "ReadPot", 128, NULL, 1, NULL);

  // Menjalankan scheduler
  vTaskStartScheduler();
}

void loop() {}

// Task LED berkedip
void TaskBlink(void *pvParameters) {
  pinMode(8, OUTPUT);

  for (;;) {
    digitalWrite(8, HIGH);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);

    digitalWrite(8, LOW);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);
  }
}

// Task membaca potensiometer
void TaskPot(void *pvParameters) {
  int potValue;

  for (;;) {
    // Membaca nilai analog dari potensiometer
    potValue = analogRead(A0);

    // Mengubah nilai potensiometer menjadi delay LED
    // Semakin besar nilai pot, semakin lambat LED
    ledDelay = map(potValue, 0, 1023, 100, 1000);

    Serial.print("Pot Value: ");
    Serial.print(potValue);

    Serial.print(" | LED Delay: ");
    Serial.println(ledDelay);

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
```

## Pertanyaan Percobaan 5B: Komunikasi Task

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! <br>
**Jawab:** Kedua task pada program FreeRTOS berjalan secara multitasking, yaitu bergantian sangat cepat sesuai pengaturan scheduler RTOS sehingga terlihat seperti berjalan bersamaan (concurrent). Scheduler akan memberikan jatah waktu eksekusi (time slicing) kepada setiap task. Pada program ini, task read_data() mengirim data ke queue lalu masuk ke kondisi delay menggunakan vTaskDelay(), sehingga scheduler memberi kesempatan task display() untuk berjalan dan mengambil data dari queue menggunakan xQueueReceive().

2. Apakah program ini berpotensi mengalami race condition? Jelaskan! <br>
**Jawab:** Program ini tidak berpotensi mengalami race condition karena komunikasi data antar-task menggunakan queue FreeRTOS (xQueueSend dan xQueueReceive) yang sudah memiliki mekanisme sinkronisasi internal. Queue memastikan hanya satu task yang mengakses data tertentu pada satu waktu sehingga data tidak tertimpa atau dibaca secara bersamaan secara tidak aman. Jika kedua task langsung memakai variabel global tanpa proteksi, maka race condition bisa terjadi.

3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md. <br>
**Jawab:**
``` cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ================= DHT CONFIG =================
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ================= STRUCT =================
struct readings {
  float temp;
  float hum;
};

QueueHandle_t my_queue;

// ================= TASK =================
void read_data(void *pvParameters);
void display_data(void *pvParameters);

void setup() {
  Serial.begin(9600);

  dht.begin();

  // Membuat queue
  my_queue = xQueueCreate(3, sizeof(struct readings));

  // Membuat task
  xTaskCreate(read_data, "ReadSensor", 256, NULL, 1, NULL);
  xTaskCreate(display_data, "DisplayData", 256, NULL, 1, NULL);
}

void loop() {
  // Kosong karena FreeRTOS menggunakan scheduler
}

/*
 * Task membaca sensor DHT
 */
void read_data(void *pvParameters) {

  struct readings data;

  for (;;) {

    // Membaca suhu dan kelembapan
    data.temp = dht.readTemperature();
    data.hum = dht.readHumidity();

    // Cek apakah pembacaan gagal
    if (!isnan(data.temp) && !isnan(data.hum)) {

      // Kirim data ke queue
      xQueueSend(my_queue, &data, portMAX_DELAY);
    }

    // Delay 2 detik
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

/*
 * Task menampilkan data ke Serial Monitor
 */
void display_data(void *pvParameters) {

  struct readings data;

  for (;;) {

    // Ambil data dari queue
    if (xQueueReceive(my_queue, &data, portMAX_DELAY) == pdPASS) {

      Serial.print("Temperature : ");
      Serial.print(data.temp);
      Serial.println(" C");

      Serial.print("Humidity    : ");
      Serial.print(data.hum);
      Serial.println(" %");

      Serial.println("----------------------");
    }
  }
}
```