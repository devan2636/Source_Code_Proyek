/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : PROYEK MANDIRI - Pengujian Komponen 
 *          Potensiometer
 * Tanggal: Senin, 29 November 2021
 */

//Deklarasi LCD dan I2C
#include <Wire.h> //Library I2C
#include <LiquidCrystal_I2C.h> //Library LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);

//Deklarasi untuk Potensiometer
float SV, PV, pot; //deklarasi SV dan PV
unsigned long t; //waktu sekarang
double t_1, Ts; //waktu sebelum , Ts = time sampling
float interval_elapsed, interval_limit;

void setup() {
  Serial.begin(9600);
  interval_elapsed = 0; //besarnya interval yang sudah dilewati
  interval_limit= 0.01; //waktu data akan ditampilkan;
  t = 0;
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0); // kolom x baris
  lcd.print("SV : ");
  lcd.setCursor(9,0); // kolom x baris
  lcd.print(" pH");
  lcd.setCursor(0,1); // kolom x baris
  lcd.print("PV : ");
  lcd.setCursor(9,1); // kolom x baris
  lcd.print(" pH");
}

void loop() {
  pot = analogRead(0);
  SV = f_map(pot,0,1023,1,14);
  
  PV = SV;
  t_1 = t; //Menghitung Ts
  t = millis();
  Ts = (t-t_1)/1000; //Proses perhitungan Ts dalam second
  //Menghitung waktu elapsed untuk menentukan kapan nilai di display
  interval_elapsed = interval_elapsed + Ts;

  if (interval_elapsed >= interval_limit){
    Serial.print(0);
    Serial.print(" ");
    Serial.print(15);
    Serial.print(" ");
    Serial.print(SV,2);
    Serial.print(" ");
    Serial.println(PV,2);

    //Menampilkan data ke LCD
    lcd.setCursor(4,0);
    lcd.print(SV,2);
    lcd.setCursor(4,1);
    lcd.print(PV,2);

    //Reset nilai untuk perhitungan selanjutnya
    interval_elapsed = 0;
  }else {
    interval_elapsed = interval_elapsed;
  }
  
}

//Membuat fungsi agar SV menjadi bentuk float
float f_map(float x, float x1, float x2, float y1, float y2){
  return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
}
