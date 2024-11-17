#include <LiquidCrystal_I2C.h>

// Initialiserer LCD-skjerm med adresse 0x27, 20 tegn, 4 rader
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Definerer pinnen knappen er koblet til
const int buttonPin = 15;  // GPIO 15 brukes til knappen
int buttonState = 0;       
int lastButtonState = 0;    

void setup() {
  // Initialiserer LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("I AM AUTOBOAT");

  pinMode(buttonPin, INPUT); // Setter knapp-pinnen som input
}

void loop() {
  // Les statusen til knappen
  buttonState = digitalRead(buttonPin);

  // Sjekker om knappen er trykket (fra ikke trykket til trykket)
  if (buttonState == HIGH && lastButtonState == LOW) {
    lcd.clear(); // Tømmer skjermen
    lcd.setCursor(0, 0);
    lcd.print("Vekt: 2kg");
    lcd.setCursor(0, 1);
    delay(200); // Debounce forsinkelse
  }

  if (buttonState == LOW && lastButtonState == HIGH) {
    lcd.clear(); // Tømmer skjermen
    lcd.setCursor(0, 0);
    lcd.print("I AM AUTOBOAT");
    lcd.setCursor(0, 1);
    delay(200); // Debounce forsinkelse
  }

  // Oppdaterer knappens status
  lastButtonState = buttonState;
}
