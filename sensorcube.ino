// Arduino Sensor Würfel
// Design, Code und Arbeit von: Marlene Kaser
// Verwendet: Arduino Uno Klon von AZ Delivery
// 0.91 inch Oled über I²C Bus
// 16x3 Zeichen. Cursor wird wie folgt angesprochen: u8x8.setCurosor([gewünschtes Zeichen als INT], [gewünschte Zeile als INT])

#include <Wire.h>
#include <U8g2lib.h>    // eine Bibliothek zur Kommunikation mit dem Display

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // hier wird der Typ des Displays bestimmt

const int TRIGGER_PIN = 6; //auf diesem Pin liegt der Ultraschall-Trigger
const int ECHO_PIN = 7;     // auf diesem Pin liegt der Ultraschall - Echo
const int ROTARY_A_PIN = 5;
const int ROTARY_B_PIN = 4;
const int ROTARY_SW = 2; // muss auf einem Interrupt fähigen Pin liegen, beim UNO Pin 2 oder 3
const int REED_PIN = 8;

long distance_ultra = 0;
int analog_light = 0;

int rotary_A_Last;
int encoderPosCount = 0;
int aVal;
boolean bCW; //clockwise, Uhrzeigersinn

int i = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Marlenes fantastischer Sensor Cube");
  Serial.println("V0.1 vom 2. 8. 2023");
  Serial.println("booting...");

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "Hello World!");

  pinMode(TRIGGER_PIN, OUTPUT);   // der Trigger-Pin muss vom Arduino High und Low gesetzt werden können
  pinMode(ECHO_PIN, INPUT);       // der Echo-Pin muss eingelesen werden können
  //  pinMode(ROTARY_SW, INPUT);      // das is der Drück-Pin vom Encoder

  pinMode(ROTARY_A_PIN, INPUT);
  pinMode(ROTARY_B_PIN, INPUT);
  rotary_A_Last = digitalRead(ROTARY_A_PIN);

  Serial.println("Booten abgeschlossen");

  delay(2000);
}

long get_Distance_Ultra() {
  // diese zwei Variablen brauchen wir für die Berechnung.
  long duration = 0;
  long distance = 0;

  digitalWrite(TRIGGER_PIN, LOW);
  digitalWrite(ECHO_PIN, LOW);
  delay(5);
  digitalWrite(TRIGGER_PIN, HIGH);
  delay(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = ((duration / 2.) * 0.03432) + 0.5;   // die Dauer des Signals durch 2 dividieren, der Kommapunkt wird gebraucht wegen des Typs long. Dann multipliziert
  return distance;                                // mit den Zentimetern pro Mikrosekunde Schallgeschwindigkeit und addieren +0,5 fürs korrekte runden.
}

void show_US() {
  long distance = get_Distance_Ultra();
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.setInverseFont(1);
  u8x8.println("Ultraschall:");
  u8x8.setCursor(0, 1);
  u8x8.println(distance);
  u8x8.setCursor(0, 2);
  u8x8.println("Zentimeter");
}

void show_Light() {
  analog_light = analogRead(A0);
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.setInverseFont(1);
  u8x8.println("Photoresistor: ");
  u8x8.setCursor(0, 1);
  u8x8.println(analog_light);

}

void show_Reed() {
  bool reed = digitalRead(REED_PIN);
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.setInverseFont(1);
  u8x8.println("Reedkontakt: ");
  u8x8.setCursor(0, 1);
  u8x8.println(reed);
  u8x8.setCursor(0, 2);
  if (reed) {
    u8x8.println("Magnet ist da");
  }
  else {
    u8x8.println("Magnet nicht da");
  }
}

void loop() {
  aVal = digitalRead(ROTARY_A_PIN);
  if (aVal != rotary_A_Last) {    // etwas hat sich geändert, der Knopf ist gedreht worden
    if (digitalRead(ROTARY_B_PIN) != aVal) {  // Pin A hat sich geändert, B noch nicht -> wir drehen im Uhrzeigersinn
      encoderPosCount++;
      bCW = true;
    } else {
      encoderPosCount--;
      bCW = false;
    }
    Serial.println(encoderPosCount);

    if (encoderPosCount < 0) {          // da wir nur 
      i = 0;
    }
    else if (encoderPosCount > 7) {
      i = 7;
    }
    else {
      i = encoderPosCount;
    }
    Serial.println(i);

    switch (i) {
      case 0:
        Serial.println("Case 0");
        show_US();
        break;
      case 1:
        Serial.println("Case 1");
        show_Light();
        break;
      case 2:
        Serial.println("Case 2");
        show_Reed();
        break;
      default:
        show_US();
        break;
    }
  }
  rotary_A_Last = aVal;
}
