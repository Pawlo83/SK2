// Definicja pinów sterujących
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Kolejność kroków dla pełnego obrotu silnika (4-fazy)
int steps[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

// Funkcja do sterowania silnikiem
void stepMotor(int step) {
  digitalWrite(IN1, steps[step][0]);
  digitalWrite(IN2, steps[step][1]);
  digitalWrite(IN3, steps[step][2]);
  digitalWrite(IN4, steps[step][3]);
}

void setup() {
  // Ustawienie pinów jako wyjścia
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Obrót silnika o pełen krok
  for (int i = 0; i < 8; i++) {
    stepMotor(i);
    delay(10);  // Opóźnienie pomiędzy krokami (można zmieniać prędkość)
  }
}
