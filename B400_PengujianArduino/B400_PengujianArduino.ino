/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Matkul : Sistem Kendali Digital
 * Judul  : PROYEK MANDIRI - Pengujian Komponen Arduino
 * Tanggal: Minggu, 28 November 2021
 */

void setup() {
  pinMode(13,OUTPUT);
}

void loop() {
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}
