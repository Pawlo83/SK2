import Adafruit_DHT
import sqlite3
import time
from datetime import datetime

# Ustawienia czujnika
SENSOR = Adafruit_DHT.DHT11
GPIO_PIN = 'P9_11'  # Pin GPIO, do którego podłączony jest DHT11

# Funkcja do wykonania pomiarów i uśredniania wyników
def get_average_reading():
    measurements = []
    for _ in range(18):
        humidity, temperature = Adafruit_DHT.read_retry(SENSOR, GPIO_PIN)
        if temperature is not None:
            measurements.append(temperature)
        time.sleep(1)  # Odstęp między pomiarami

    if len(measurements) >= 3:
        measurements.remove(max(measurements))
        measurements.remove(min(measurements))
        average_temp = sum(measurements) / len(measurements)
        return round(average_temp, 2)
    else:
        return None

# Inicjalizacja bazy danych
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

# Zapis pomiaru do bazy danych
def save_to_database(temperature):
    conn = sqlite3.connect("sensor_data.db")
    cursor = conn.cursor()
    timestamp = datetime.now().strftime("%H:%M:%S, %d-%m-%Y")
    cursor.execute("INSERT INTO measurements (timestamp, temperature) VALUES (?, ?)", (timestamp, temperature))
    conn.commit()
    conn.close()

# Wyświetlenie danych z bazy
def display_data():
    conn = sqlite3.connect("sensor_data.db")
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM measurements")
    rows = cursor.fetchall()
    for row in rows:
        print(f"ID: {row[0]}, Timestamp: {row[1]}, Temperature: {row[2]}°C")
    conn.close()

# Główna część programu
if __name__ == "__main__":
    initialize_database()

    for _ in range(5):  # Powtarzamy algorytm 5 razy
        avg_temp = get_average_reading()
        if avg_temp is not None:
            print(f"Average Temperature: {avg_temp}°C")
            save_to_database(avg_temp)
        else:
            print("Failed to get valid reading.")
        time.sleep(2)  # Odstęp między cyklami

    print("\n")
    print("Data in Database:")
    display_data()
