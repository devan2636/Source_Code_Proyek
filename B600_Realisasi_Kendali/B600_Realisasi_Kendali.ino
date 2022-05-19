//ID Program 
/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : Proyek Mandiri - 
 *          Program Realisasi kendali (Revisi 1)
 * Tanggal: Minggu, 9 Januari 2022
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
      
//1d. Deklarasi Variabel 
      float SV, PV, MV; // mendefinisikan variabel RC,fc,a,SV,dan PV sebagai float 
      int start ;
      
//1e. Deklarasi perhitungan PID-control
      float Kp, et, et_1, Ti, Ki, edif, Td, Kd, P, I, D;
      float eint, eint_1, eint_update;
      
//1f. Deklarasi perhitungan filter 
      float PVf, PVf_1,fc,RC,a ; 
       
//1g. deklarasi variabel time sampling 
      unsigned long t ; 
      double t_1,Ts ;
      
//1h. Deklarasi untuk plotting 
      float interval_elapsed, interval_limit ;  

 //1i Deklarasi LCD I2C
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

//2d. Setup parameter filter yang digunakan 
      fc = 0.819 ; // frekuensi cut off 1 dekade
      RC = 1/(6.28*fc); 
      Ts = 0.01; // Diukur secara manual 
      a = RC/Ts ; 
      PVf_1 = 0 ; 

//2e. setting utnuk mengatur durasi tampilan 
      interval_elapsed = 0; // besarnya interval yang sdh dilewati
      interval_limit = 0.01 ; //setiap berapa lama data akan ditampilkan (dalam detik)
      t = 0;
      
//2f. setup untuk sistem start
      digitalWrite(pinON,HIGH);// sinyal dari pin ini akan dipakai untuk logika 1 di pin start 
      digitalWrite(pinOFF,LOW);//akan dipakai sbg titik nol/gnd 
      digitalWrite(EN_A,0);
       
 //2g. Set parameter kendali Kp
//       Kp = 9.995; //Kp hasil desain
//       Ti = 0.746;//Ti hasil desain
//       Td = 0.187;//Td hasil desain
       Kp = 10.5; //Kp hasil tunning
       Ti = 0.6;//Ti hasil tunning
       Td = 0.15;//Td hasil tunning
       //Menghindari nilai NAND
       if (Ti==0){
        Ki=0;}else{
          Ki=Kp/Ti;
       }
       Kd=Kp*Td;
       eint_1=0; //set awal untuk luasan

 //2h. Mengset, besaran dan satuan di LCD
      lcd.init(); //Menginit LCD
      lcd.begin(16,2);
      lcd.backlight(); //Menyalakan backlight LCD
      lcd.setCursor(0,0);
      lcd.print(" SISTEM KENDALI");
      lcd.setCursor(0,1);
      lcd.print(" PH HINDROPONIK");
      delay(10000);
      lcd.clear();
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
          SV = 3.30;
        }
        else if (start == 0){
          SV = 7.30 ;
          }
//      analogWrite(EN_A,0);
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
       
//3c. Melakukan filter     
      PVf = (PV + a*PVf_1)/(a+1);
      PVf_1 =PVf;
      
//3d. Hitung error
      float e = PVf - SV;
//3e. Menghing Kp
      P = Kp * e;
      
//3f. Menghitung bagian I
       eint_update = ((et+et_1)*Ts)/2;
       eint=eint_1+eint_update;
       I=Ki*eint;

 //3g. Menghitung bagian D
       edif = (et-et_1)/Ts;
       D = Kd * edif;
 //3h. Hitng keluaran PID      
       MV = P + I + D;

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
//[6]   Cek hasil penjumlahan Ts akan di cek apakah sudah sama/ lebih dari batas interval
         if (interval_elapsed >= interval_limit){
//[7]
//7a. Menampilkan Display plotter
        Serial.print(0); 
        Serial.print(" ");
        Serial.print(12); 
        Serial.print(" ");
        Serial.print(SV);      
        Serial.print(" ");
        Serial.println(PVf); 

        lcd.setCursor(0,0);
        lcd.print(Kp);
        lcd.setCursor(6,0);
        lcd.print(Ti);
        lcd.setCursor(11,0);
        lcd.print(Td);
        lcd.setCursor(3,1);
        lcd.print(SV);
        lcd.setCursor(12,1);
        lcd.print(PVf);
    
//7b. Mereset nilai IE 
        interval_elapsed =0;}
       // setelah ditampilkan agar perhitungan di ulang dari nol lagi 
        else{
          interval_elapsed = interval_elapsed;
 //8 Mengeset untuk perhitungan selanjutnya
        et_1=et;
        eint_1=eint;
         }
}
