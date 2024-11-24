#include <OneWire.h>
#include <DS18B20.h>

// Definicje pinów
#define ONEWIRE_PIN 3
byte address[8] = {0x28, 0x8, 0xC, 0x79, 0x97, 0x2, 0x3, 0x84};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

void setup() {
  Serial.begin(9600); // Inicjalizacja UART
  sensors.begin();
  sensors.request(address);
}

float getTemperature() {
  sensors.request(address);
  delay(1000);
  return sensors.readTemperature(address);
}

void loop() {
  float temperatures[18];
  for (int i = 0; i < 18; i++) {
    temperatures[i] = getTemperature();
    delay(200);
  }

  // Usunięcie wartości skrajnych
  float minVal = temperatures[0], maxVal = temperatures[0];
  int minID = 0, maxID = 0;
  for (int i = 1; i < 18; i++) {
    if (temperatures[i] < minVal) {
      minVal = temperatures[i];
      minID = i;
    }
    if (temperatures[i] > maxVal) {
      maxVal = temperatures[i];
      maxID = i;
    }
  }

  float sum = 0;
  for (int i = 0; i < 18; i++) {
    if (i != minID && i != maxID) {
      sum += temperatures[i];
    }
  }
  float average = sum / 16.0;

  Serial.println(average);
  delay(1000);
}


import Adafruit_DHT
import sqlite3
import serial
from datetime import datetime

# Inicjalizacja UART
ser = serial.Serial(port="/dev/ttyS1", baudrate=9600, timeout=1)

# Funkcja inicjalizacji bazy danych
def initialize_database():
    conn = sqlite3.connect("sensor_data.db")
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS measurements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            temperature REAL NOT NULL
        )
    """)
    conn.commit()
    conn.close()

# Funkcja zapisu danych do bazy
def save_to_database(temperature):
    conn = sqlite3.connect("sensor_data.db")
    cursor = conn.cursor()
    timestamp = datetime.now().strftime("%H:%M:%S, %d-%m-%Y")
    cursor.execute("INSERT INTO measurements (timestamp, temperature) VALUES (?, ?)", (timestamp, temperature))
    conn.commit()
    conn.close()

# Odczyt danych z UART i zapis do bazy
def read_from_uart():
    if ser.isOpen():
        line = ser.readline().decode('utf-8').strip()
        if line:
            try:
                temperature = float(line)
                print(f"Received temperature: {temperature}°C")
                save_to_database(temperature)
            except ValueError:
                print("Invalid data received")

if __name__ == "__main__":
    initialize_database()
    print("UART initialized. Listening for data...")
    try:
        while True:
            read_from_uart()
    except KeyboardInterrupt:
        print("Terminating program.")
    finally:
        ser.close()
