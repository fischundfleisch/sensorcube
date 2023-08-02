// Arduino Sensor Würfel
// Design, Code und Arbeit von: Marlene Kaser
// Verwendet: Arduino Uno Klon von AZ Delivery
// 0.91 inch Oled über I²C Bus
// 16x3 Zeichen. Cursor wird wie folgt angesprochen: u8x8.setCurosor([gewünschtes Zeichen als INT], [gewünschte Zeile als INT])

#include <Wire.h>
#include <Adafruit_GFX.h>  //core graphics library. Beinhaltet fast alle Standard Grafiken, Punkte, Linien, Farben usw
#include <U8g2lib.h>

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

void setup() {
  Serial.begin(9600);
  Serial.println("Marlenes fantastischer Sensor Cube");
  Serial.println("V0.1 vom 2. 8. 2023");
  Serial.println("booting...");

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "Hello World!");

  Serial.println("Booten abgeschlossen");

  delay(2000);
}

void loop() {
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.setInverseFont(1);
  u8x8.println("0");
  u8x8.setCursor(0, 1);
  u8x8.println("01");
  u8x8.setCursor(0, 2);
  u8x8.println("012");

  delay(1000);
  
  u8x8.setCursor(0, 0);
  u8x8.println("0123");
  u8x8.setCursor(0, 1);
  u8x8.println("01234");
  u8x8.setCursor(0, 2);
  u8x8.println("012345");

  delay(1000);

  u8x8.setCursor(0, 0);
  u8x8.println("0123456");
  u8x8.setCursor(0, 1);
  u8x8.println("01234567");
  u8x8.setCursor(0, 2);
  u8x8.println("012345678");

  delay(1000);
  u8x8.setCursor(0, 3);
  u8x8.println("0123456789012345");
  delay(1000);

}
