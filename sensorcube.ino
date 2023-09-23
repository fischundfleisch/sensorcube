// Arduino Sensor Würfel
// Design, Code und Arbeit von: Marlene Kaser
// Verwendet: Arduino Uno Klon von AZ Delivery
// 0.91 inch Oled über I²C Bus
// 16x3 Zeichen. Cursor wird wie folgt angesprochen: u8x8.setCurosor([gewünschtes Zeichen als INT], [gewünschte Zeile als INT])

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8g2lib.h>    // eine Bibliothek zur Kommunikation mit dem Display
// Display Anschluss erfolgt über SDA - A4 und SCK - A5

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);    // wir erstellen eine OneWire Instanz um mit allen Temperatursensoren zu kommunizieren
DallasTemperature sensors(&oneWire);  // hier kann nämlich mehr als nur einer am selben Pin hängen

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // hier wird der Typ des Displays bestimmt

#define DHTPIN  10
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int TRIGGER_PIN = 6; //auf diesem Pin liegt der Ultraschall-Trigger
const int ECHO_PIN = 7;     // auf diesem Pin liegt der Ultraschall - Echo
const int ROTARY_A_PIN = 5; //DT-PIN
const int ROTARY_B_PIN = 4; // CLK-Pin
//const int ROTARY_SW = 2; // muss auf einem Interrupt fähigen Pin liegen, beim UNO Pin 2 oder 3
const int REED_PIN = 8;
const int FORCE_PIN = A1;
const int DHT_PIN = 10;

const int NUMBER_SENSORS = 5;

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

  pinMode (REED_PIN, INPUT);

  pinMode(DHT_PIN, INPUT);
  dht.begin();

  pinMode(DS18B20_PIN, INPUT_PULLUP);
  sensors.begin();

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

void show_Force() {
  int force = analogRead(FORCE_PIN);
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.setInverseFont(1);
  u8x8.println("Kraftsensor: ");
  u8x8.setCursor(0,1);              // mehr Kraft = weniger Widerstand, ca 1MOhm bei 0 Kraft
  u8x8.print(force);                // 1024 entspricht 2 kg, die Anzeige sind Analogwerte, hier müssten wir noch umrechnen
}

void show_dht22() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.setInverseFont(1);
  u8x8.print("Temp.: ");
  u8x8.print(t);
  u8x8.println(" °C");
  u8x8.setCursor(0,1);
  u8x8.print("Feucht: ");
  u8x8.print(h);
  u8x8.println(" %");
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
    
i = abs(encoderPosCount) % NUMBER_SENSORS;

    switch (i) {
      case 0:
        show_US();
        break;
      case 1:
        show_Light();
        break;
      case 2:
        show_Reed();
        break;
      case 3:
        show_Force();
        break;
      case 4:
        show_dht22();
        break;
      default:
        show_US();
        break;
    }
  }
  u8x8.setCursor(0,3);
  u8x8.setInverseFont(0);
  u8x8.print("Steps: ");
  u8x8.println(encoderPosCount);
  rotary_A_Last = aVal;
}
