#include <OneWire.h>
#include <DS18B20.h>

#define ONEWIRE_PIN 2      // Pin do czujnika temperatury DS18B20
#define BUTTON1_PIN 4      // Pin pierwszego przycisku
#define BUTTON2_PIN 5      // Pin drugiego przycisku
#define BUTTON3_PIN 6      // Pin trzeciego przycisku
#define LED_PIN 13         // Pin do diody LED

byte address[8] = {0x28, 0xB1, 0x6D, 0xA1, 0x3, 0x0, 0x0, 0x11}; // Adres czujnika
OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

// Funkcja do pobrania temperatury
float getTemperature() {
  sensors.request(address);
  delay(1000); // Czas na odczyt z czujnika
  return sensors.readTemperature(address);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  sensors.begin();
  sensors.request(address);
}

void loop() {
  if (digitalRead(BUTTON1_PIN) == LOW) { // Naciśnięcie pierwszego przycisku
    float temperature = getTemperature();
    Serial.print("Jednorazowy pomiar temperatury: ");
    Serial.print(temperature);
    Serial.println(" 'C");
    delay(500); // Debouncing przycisku
  }

  if (digitalRead(BUTTON2_PIN) == LOW) { // Naciśnięcie drugiego przycisku
    float temperatures[18];
    unsigned long startTime = millis(); // Zapisanie czasu rozpoczęcia
    for (int i = 0; i < 18; i++) {
      temperatures[i] = getTemperature();
      digitalWrite(LED_PIN, HIGH); // Zapalenie diody na 2ms
      delay(2);
      digitalWrite(LED_PIN, LOW);
      delay(100); // Krótkie opóźnienie przed kolejnym pomiarem
    }

    // Usunięcie wartości skrajnych
    float minVal = temperatures[0];
    float maxVal = temperatures[0];
    for (int i = 1; i < 18; i++) {
      if (temperatures[i] < minVal) minVal = temperatures[i];
      if (temperatures[i] > maxVal) maxVal = temperatures[i];
    }

    // Uśrednianie po usunięciu skrajnych
    float sum = 0;
    for (int i = 0; i < 18; i++) {
      if (temperatures[i] != minVal && temperatures[i] != maxVal) {
        sum += temperatures[i];
      }
    }
    float average = sum / 16; // Usunięto 2 wartości (min i max)
    unsigned long elapsedTime = millis() - startTime; // Czas oczekiwania

    Serial.print("Uśredniona temperatura: ");
    Serial.print(average);
    Serial.println(" 'C");
    Serial.print("Czas oczekiwania: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");
    delay(500); // Debouncing przycisku
  }

  if (digitalRead(BUTTON3_PIN) == LOW) { // Naciśnięcie trzeciego przycisku
    float temperatures[18];
    unsigned long startTime = millis(); // Zapisanie czasu rozpoczęcia
    for (int i = 0; i < 18; i++) {
      temperatures[i] = getTemperature();
      digitalWrite(LED_PIN, HIGH); // Zapalenie diody na 2ms
      delay(2);
      digitalWrite(LED_PIN, LOW);
      delay(100); // Krótkie opóźnienie przed kolejnym pomiarem
    }

    // Usunięcie wartości skrajnych
    float minVal = temperatures[0];
    float maxVal = temperatures[0];
    for (int i = 1; i < 18; i++) {
      if (temperatures[i] < minVal) minVal = temperatures[i];
      if (temperatures[i] > maxVal) maxVal = temperatures[i];
    }

    // Uśrednianie z przesunięciem bitowym po usunięciu skrajnych
    long sum = 0;
    for (int i = 0; i < 18; i++) {
      if (temperatures[i] != minVal && temperatures[i] != maxVal) {
        sum += (long)(temperatures[i] * 100); // Mnożenie dla zachowania precyzji
      }
    }
    long average = sum >> 4; // Przesunięcie bitowe o 4 miejsca (dzielenie przez 16)
    float averageTemp = (float)average / 100.0; // Zamiana na wartość float
    unsigned long elapsedTime = millis() - startTime; // Czas oczekiwania

    Serial.print("Uśredniona temperatura (bitowo): ");
    Serial.print(averageTemp);
    Serial.println(" 'C");
    Serial.print("Czas oczekiwania: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");
    delay(500); // Debouncing przycisku
  }
}
