/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : PROYEK MANDIRI - Pengujian Komponen 
 *          Kalibrasi Sensor pH
 * Tanggal: Senin, 29 November 2021
 */

#include <Wire.h> //Library I2C
#include <LiquidCrystal_I2C.h> //Library LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0); // kolom x baris
  lcd.print("KALIBRASI pH");
  delay(2000);
}

void loop() {
  int nilaiSensor = analogRead(A2);
  float tegangan = nilaiSensor*(5.0/1023.0);
  lcd.setCursor(0,0); // kolom x baris
  lcd.print("Kalibrasi Sensor");
  lcd.setCursor(0,1);
  lcd.print("Teg : ");
  lcd.setCursor(7,1);
  lcd.print(tegangan);
  lcd.setCursor(14,1);
  lcd.print(tegangan);
  delay(1000);
}
