#include <Wire.h> // Importeert de Wire Library. Zorgt voor communicatie tussen arduino en I2C, zoals LCD
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Led pins
const int ledPins[4] = {8, 9, 10, 11};

// Button pins
const int buttonPins[4] = {2, 3, 4, 5};

// Buzzer 
const int buzzerPin = 6;

int sequence[100]; // Array om de lampjes die branden op te slaan
int level = 0; // Welk level je bent
int userIndex = 0; // Om bij te houden hoeveel knoppen de speler heeft gedrukt
bool waitingForInput = false; // Of het de beurt is van de speler om op de knoppen te drukken


void setup() {
  lcd.init(); // Initiaseert het LCD, maakt het klaar voor gebruik
  lcd.backlight(); // Zet de achtergrondverlichting aan van de LCD
  pinMode(buzzerPin, OUTPUT);

  // Zet LED pins als OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // Buttons met pull-up
  }

  // Tekst om te beginnen
  lcd.setCursor(0, 0);
  lcd.print("Druk op een knop");
  lcd.setCursor(0, 1);
  lcd.print("om het spel");
  lcd.setCursor(0, 2);
  lcd.print("te starten.");
  lcd.setCursor(0, 3);
  lcd.print("Als je durft! :)");
}

void loop() {
  if (level == 0) {
    // Wachten op eerste druk voordat het spel begint
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) { // checken of button word ingedrukt (INPUT_PULLUP)
        delay(200);
        startGame();
      }
    }
  } else if (waitingForInput) {
    checkUserInput();
  }
}

void startGame() {
  level = 1;
  userIndex = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level 1");
  delay(1000);
  addToSequence();
  playSequence();
  waitingForInput = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jij bent nu");
  lcd.setCursor(0, 1);
  lcd.print("aan de beurt!");
}

void addToSequence() {
  sequence[level - 1] = random(0, 4); // Voegt een random lampje toe
}

void playSequence() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kijk goed!");
  delay(1000);

  for (int i = 0; i < level; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(500);
    digitalWrite(ledPins[led], LOW);
    delay(300);
  }
}

void checkUserInput() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == LOW) { // checken of button word ingedrukt (INPUT_PULLUP)
      delay(200);
      flashLED(i); // Als je de knop indruk, brand de LED

      if (i == sequence[userIndex]) {
        userIndex++;
        if (userIndex == level) {
          // Volgend level
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Goed zo!");
          lcd.setCursor(0, 1);
          lcd.print("Level: " + String(level + 1));
          playSuccesSound(); // Hier maakt de buzzer het succes geluid
          delay(1500);
          level++; // Level omhoog
          userIndex = 0; // Reset de zetten weer op 0
          addToSequence();
          playSequence();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Jij bent nu");
          lcd.setCursor(0, 1);
          lcd.print("aan de beurt!");
        }
      } else {
        // Game over
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Game Over!");
        lcd.setCursor(0, 1);
        lcd.print("Level: " + String(level));
        lcd.setCursor(0, 2);
        lcd.print("Druk op een knop om");
        lcd.setCursor(0, 3);
        lcd.print("opnieuw te beginnen");
        playGameOverSound(); // Hier maakt de buzzer het game over geluid geluid
        level = 0;

        // Sequence word leeggehaald, om niet dezelfde sequence te krijgen
        for (int i = 0; i < 100; i++) {
          sequence[i] = 0;
        }
      }
    }
  }
}

void flashLED(int ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  delay(200);
  digitalWrite(ledPins[ledIndex], LOW);
}

void playGameOverSound() {
  tone(buzzerPin, 659); // 200Hz
  delay(300);
  tone(buzzerPin, 587); // 200Hz
  delay(300);
  tone(buzzerPin, 523); // 200Hz
  delay(300);
  tone(buzzerPin, 494); // 200Hz
  delay(600);
  noTone(buzzerPin);
}

void playSuccesSound() {
  tone(buzzerPin, 800);
  delay(200);
  tone(buzzerPin, 1000);
  delay(200);
  tone(buzzerPin, 800);
  delay(200);
  noTone(buzzerPin);
}
