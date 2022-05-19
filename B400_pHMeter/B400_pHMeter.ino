/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : PROYEK MANDIRI - Pengujian Komponen 
 *          Meter pH
 * Tanggal: Rabu, 1 Desember 2021
 */

#include <Wire.h> //Library I2C
#include <LiquidCrystal_I2C.h> //Library LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);


const int pinPH = A0;
int nilaiSensor = 0;
unsigned long int rataADCPH;
int bufADCPH[10], temp;

void setup (){
  Serial.begin(9600);
  //delay(10000);
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0); // kolom x baris
  lcd.print("pH Meter");
  delay(2000);
}


void loop(){
  for(int i = 0; i<10; i++){
    bufADCPH[i] = analogRead(pinPH);
    delay(10);
  }

  for (int i = 0; i<9; i++){
    for(int j=i+1;j<10;j++){
      if (bufADCPH[i]>bufADCPH[j]){
        temp =bufADCPH[i];
        bufADCPH[i] = bufADCPH[j];
        bufADCPH[j] = temp;
      }
    }
  }
  rataADCPH=0;
  for(int i = 2; i<8; i++){
    rataADCPH+=bufADCPH[i];
  }

  float teganganPH = (float)rataADCPH*5.0/1024/6;
  float nilaipH = 2.101628 * teganganPH + 2.917;
//  float nilaipH = 2.142857 * teganganPH + 2.89571436;

  Serial.print("Nilai pH = ");
  Serial.println(nilaipH);

  lcd.setCursor(0,0); // kolom x baris
  lcd.print("pH Meter");
  lcd.setCursor(0,1);
  lcd.print("pH : ");
  lcd.setCursor(7,1);
  lcd.print(nilaipH);
}
