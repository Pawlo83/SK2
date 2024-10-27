#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Ustawienie adresu wyświetlacza (0x27) i rozmiaru (16x2)
Servo serwomechanizm;

const int buttonPin = 2; // Pin dla przycisku
volatile bool blocked = true; // Zmienna blokady, początkowo blokada aktywna
int drzwiStan = 0; // 0 = zamknięte, 1 = otwarte

void setup() {
    // Inicjalizacja wyświetlacza
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Closed");
    lcd.setCursor(0, 1);
    lcd.print("Blocked");

    // Inicjalizacja serwomechanizmu
    serwomechanizm.attach(3); // Serwomechanizm podłączony do pinu 3
    serwomechanizm.write(0); // Ustawienie pozycji początkowej na 0° (drzwi zamknięte)

    // Inicjalizacja przycisku z przerwaniem
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), toggleBlockedState, FALLING);

    // Inicjalizacja komunikacji szeregowej
    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();
        if (blocked) {
            lcd.setCursor(0, 1);
            lcd.print("Blocked      ");
        } else {
            lcd.setCursor(0, 1);
            lcd.print("Unblocked    ");
            if (cmd == 'o' && drzwiStan == 0) {
                openDoor();
            } else if (cmd == 'c' && drzwiStan == 1) {
                closeDoor();
            } else if (cmd == 'o' && drzwiStan == 1) {
                displayTemporaryMessage("Already open");
            } else if (cmd == 'c' && drzwiStan == 0) {
                displayTemporaryMessage("Already closed");
            } else {
                displayTemporaryMessage("Wrong cmd");
            }
        }
    }
}

void openDoor() {
    serwomechanizm.write(90); // Ruch serwa do pozycji 90° (otwarte drzwi)
    lcd.setCursor(0, 0);
    lcd.print("Open         ");
    drzwiStan = 1; // Aktualizacja stanu drzwi na otwarte
}

void closeDoor() {
    serwomechanizm.write(0); // Ruch serwa do pozycji 0° (zamknięte drzwi)
    lcd.setCursor(0, 0);
    lcd.print("Closed       ");
    drzwiStan = 0; // Aktualizacja stanu drzwi na zamknięte
}

void displayTemporaryMessage(const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    delay(3000); // Wyświetlenie wiadomości przez 3 sekundy
    lcd.clear();
    if (drzwiStan == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Closed");
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Open");
    }
}

void toggleBlockedState() {
    blocked = !blocked; // Przełączanie stanu blokady
}
