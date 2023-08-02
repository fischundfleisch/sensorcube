// Arduino Sensor Würfel
// Design, Code und Arbeit von: Marlene Kaser
// Verwendet: Arduino Uno Klon von AZ Delivery
// 0.91 inch Oled über I²C Bus
// 16x3 Zeichen. Cursor wird wie folgt angesprochen: u8x8.setCurosor([gewünschtes Zeichen als INT], [gewünschte Zeile als INT])

#include <Wire.h>
#include <U8g2lib.h>    // eine Bibliothek zur Kommunikation mit dem Display

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // hier wird der Typ des Displays bestimmt

const int TRIGGER_PIN = 2; //auf diesem Pin liegt der Ultraschall-Trigger
const int ECHO_PIN = 3;     // auf diesem Pin liegt der Ultraschall - Echo

long distance_ultra = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Marlenes fantastischer Sensor Cube");
  Serial.println("V0.1 vom 2. 8. 2023");
  Serial.println("booting...");

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "Hello World!");

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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

void loop() {
  long distance = get_Distance_Ultra();
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.setInverseFont(1);
  u8x8.println("Ultraschall:");
  u8x8.setCursor(0, 1);
  u8x8.println(distance);
  u8x8.setCursor(0, 2);
  u8x8.println("Zentimeter");

  delay(5000);
  
  

}
