#include <LiquidCrystal.h>
#include <Key.h> 
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}, 
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {
  0, 1, 2, 3
}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  4, 5, 6
}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd(11, 12, 10, 9, 8, 7);
int temp;
boolean correct = false;
String str = "";
int irsensor = 3;
int tempPin = 1;
int gasPin = 0;
int gasValue;
int motion;
int buzzPin = 13;
const String password = "1377";

void setup() {
  Serial.begin(9600);
  pinMode(buzzPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Pin: ");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    str += key;
    lcd.print('*');
    if (str.length() == 4) {
      if (str.equals(password)) {
        lcd.setCursor(0, 2);
        lcd.print("You're in!");
        correct = true;
        beep();
        str = "";
        delay(6000);
        lcd.clear();
      } else {
        lcd.setCursor(0, 2);
        lcd.print("Unauthorized");
        doubleBeep();
        str = "";
        delay(6000);
        lcd.clear();
        lcd.print("Pin: ");
      }
    }
  }
  if (correct == true) {
    security();
  }

}

void beep() {
  digitalWrite(buzzPin, HIGH);
  delay(500);
  digitalWrite(buzzPin, LOW);
}
void doubleBeep() {
  digitalWrite(buzzPin, HIGH);
  delay(1500);
  digitalWrite(buzzPin, LOW);
}

void security() {
  lcd.clear();
  motion = analogRead(irsensor);
  Serial.println(motion);
  temp = analogRead(tempPin);
  delay(10);
  temp = temp * 0.48828125;
  gasValue = analogRead(gasPin);
  delay(100);

  if (temp <= 50 && gasValue <= 500) {
    lcd.print("Welcome Home!");
    lcd.setCursor(0, 2);
    lcd.print("T=");
    lcd.print(temp);
    lcd.print("'C");
    lcd.print(" ");
    lcd.print("G=");
    lcd.print(gasValue);
    if (motion > 110) {
      lcd.print(" M");
      beep();
    } else {
      lcd.print(" S");
      delay(1000);
    }
  } else {
    lcd.clear();
    lcd.print("Warning!");
    doubleBeep();
  }
  delay(1000);
}
