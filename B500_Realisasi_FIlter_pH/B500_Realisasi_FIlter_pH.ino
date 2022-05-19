/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : PROYEK MANDIRI - Realisasi Filter
 *          IIR Sensor pH
 * Tanggal: Sabtu, 8 Januari 2022
 */

#include <Wire.h> //Library I2C
#include <LiquidCrystal_I2C.h> //Library LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);


const int pinPH = A2;
int nilaiSensor = 0;
unsigned long int rataADCPH;
int bufADCPH[10], temp;

//Deklarasi Variabel
 float SV, PV, PVf, PVf_1, fc, RC, fc1, RC1, a;
 int MV;
 unsigned long t;
 double t_1, Ts;

void setup (){
  Serial.begin(9600);
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0); // kolom x baris
  lcd.print("pH Meter");
  delay(2000);
  fc=0.819; //pergeseran 1 dekade
  RC=1/(6.28*fc);
  Ts=0.01; //Hasil Pengukuran
  a=RC/Ts;
  PVf_1=0;
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

   ///Mengecek nilai Ts
  t_1=t;
  t=millis();
  Ts=(t - t_1)/1000; //Ts dalam 
  Serial.println(Ts);
  //[5]. Filter IIR
  PVf=(nilaipH+a*PVf_1)/(a+1);
  
  
  Serial.print(1);
  Serial.print(" ");
  Serial.print(14);
  Serial.print(" ");
  Serial.print(nilaipH);
  Serial.print(" ");
  Serial.println(PVf);

  lcd.setCursor(0,0); // kolom x baris
  lcd.print("pH Meter");
  lcd.setCursor(0,1);
  lcd.print("pH : ");
  lcd.setCursor(7,1);
  lcd.print(PVf);

  PVf_1 = PVf;
}
