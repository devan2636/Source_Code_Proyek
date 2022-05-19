//ID Program 
/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : Proyek Mandiri - 
 *          Program Perancangan/Desain ZN1CL
 * Tanggal: Sabtu, 18 Desember 2021
 */

//[1]
//1a. Deklarasi Untuk Perhitungan pH
      const int pinPH = A2;
      int nilaiSensor = 0;
      unsigned long int rataADCPH;
      int bufADCPH[10], temp;

//1b. Deklarasi untuk driver pompa
      #define EN_A 11
      #define IN_1 10
      #define IN_2 9

//1c. Deklarasi pin untuk switch
      #define pinON 8
      #define pinSi 7 //pin input switch
      #define pinOFF 6
      
//1a. Deklarasi Variabel 
      float SV, PV, MV; // mendefinisikan variabel RC,fc,a,SV,dan PV sebagai float 
      int start ;
      
//1.b Deklarasi perhitungan P-control
      float Kp, e, P ; 
       
//1.c deklarasi variabel time sampling 
      unsigned long t ; 
      double t_1,Ts ;
      
//1.d Deklarasi untuk plotting 
      float interval_elapsed, interval_limit ;  

//1.e Deklarasi LCD I2C
      #include <Wire.h>
      #include <LiquidCrystal_I2C.h>
      LiquidCrystal_I2C lcd(0x27, 16, 2);
  
void setup() {
//[2]. Setup
//2a. Setup untuk Sistem  
      Serial.begin(9600); 
      
//2b. setup pin yang dipakai 
      //pin driver pompa
      pinMode(EN_A, OUTPUT);
      pinMode(IN_1, OUTPUT);
      pinMode(IN_2, OUTPUT);

      //pin untuk saklar
      pinMode(pinON,OUTPUT);
      pinMode(pinSi, INPUT );
      pinMode(pinOFF, OUTPUT);

//2c. Setup awal driver
      digitalWrite(IN_1,HIGH);
      digitalWrite(IN_2,LOW);

//2d. setting utnuk mengatur durasi tampilan 
      interval_elapsed = 0; // besarnya interval yang sdh dilewati
      interval_limit = 0.01 ; //setiap berapa lama data akan ditampilkan (dalam detik)
      
//2e. setup untuk sistem start, parameter Kp
      digitalWrite(pinON,HIGH);// sinyal dari pin ini akan dipakai untuk logika 1 di pin start 
      digitalWrite(pinOFF,LOW);//akan dipakai sbg titik nol/gnd 
      digitalWrite(EN_A,0);
      Kp = 1 ; 

//2.f Mengset, besaran dan satuan di LCD
      lcd.init(); //Menginit LCD
      lcd.begin(16,2);
      lcd.backlight(); //Menyalakan backlight LCD
      lcd.setCursor(0,0);
      lcd.print("K: ");
      lcd.setCursor(0,1);
      lcd.print("SV:");
      lcd.setCursor(9,1);
      lcd.print("PV:");
}


void loop() {
///[3]   
//3a. Membaca nilai SV
      start = digitalRead(pinSi);//membaca apakah pd posisi 1 atau 0 
        if (start ==1){
          SV = 3.00;
        }
        else if (start == 0){
          SV = 8.27 ;
          }
          
//3b. Membaca PV (Sensor pH)
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
        PV = nilaipH;
       

//3c. Hitung error
      float e = PV - SV;
//3d. Menghing Kp
      P = Kp * e;
      MV = P ; 
;
//[4]
//4a. Membatasi keluaran MV agar tidak lebih dari spesifikasi              
        if (MV> 255){
          MV = 255;}
        else if(MV<0){
         MV=0;}
        else{
          MV = MV;
          }
//4b.  Menuliskan MV ke pin out
        digitalWrite(EN_A, MV);
// [5]
//5a. Menghitung Ts
        t_1 = t;
        t = millis();
        Ts =(t - t_1)/1000;
        
//5b. Menghitung waktu elapsed untuk menentukan kapan nilai di display
        interval_elapsed= interval_elapsed +Ts;     
//[6] Cek hasil penjumlahan Ts akan di cek apakah sudah sama/ lebih dari batas interval
        if (interval_elapsed >= interval_limit){
//[7]
//7a. Menampilkan Display plotter
        Serial.print(0); 
        Serial.print(" ");
        Serial.print(12); 
        Serial.print(" ");
        Serial.print(SV);      
        Serial.print(" ");
        Serial.println(PV); 

        lcd.setCursor(3,0);
        lcd.print(Kp);
        lcd.setCursor(3,1);
        lcd.print(SV);
        lcd.setCursor(12,1);
        lcd.print(nilaipH);
    
//7b. Mereset nilai IE
        interval_elapsed =0;}
       // setelah ditampilkan agar perhitungan di ulang dari nol lagi 
        else{
          interval_elapsed = interval_elapsed;
         }
}
