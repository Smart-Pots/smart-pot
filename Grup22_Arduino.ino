#include <Servo.h>
#include <virtuabotixRTC.h>
//#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
int pos_servo = 0;

Servo ServoNesnesi;
#define SU_SEVIYE A0
#define TOPRAK_NEM A1
#define HAVA_NEM 2
#define BUZZER 3
#define SERVO 4
#define MZ80 5
int RGB_R = 6;
int RGB_G = 7;
int RGB_B = 12;
#define SAAT_CLK 8
#define SAAT_DAT 9
#define SAAT_RST 10
#define DOKUNMA 11
String SELAMLA = "HOSGELDIN";
#define LDR A2
 

LiquidCrystal_I2C lcd(0x27,16,2);
//DS1307RTC myRTC(SAAT_CLK, SAAT_DAT, SAAT_RST);
virtuabotixRTC Saat(8, 9, 10);
#include <dht11.h>
dht11 dht_sensor;
int touch_count = 0;
void setup() {
  // put your setup code here, to run once:
  lcd.begin();
  Saat.setDS1302Time(15, 50, 10,2, 7, 06, 2022);
  ServoNesnesi.attach(SERVO);
  pinMode(SU_SEVIYE,INPUT);
  pinMode(TOPRAK_NEM,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(SERVO,OUTPUT);
  pinMode(MZ80,INPUT);
  pinMode(RGB_R,OUTPUT);
  pinMode(RGB_G,OUTPUT);
  pinMode(RGB_B,OUTPUT);
  pinMode(DOKUNMA, INPUT);
  pinMode(LDR, INPUT);
  Serial.begin(9600);
 
}

void loop() {

  // START CODE
  // Cisim var mı ?
  int MZ80_STATUS = digitalRead(MZ80);
  int su_seviyesi_deger = analogRead(SU_SEVIYE);
  int chk = dht_sensor.read(HAVA_NEM);
  int toprak_deger = analogRead(TOPRAK_NEM);  // Read the analog value form sensor
  int dokunma_check = digitalRead(DOKUNMA);
  int isik_degeri = analogRead(LDR);
  delay(500);
  if(MZ80_STATUS == 0) {
    lcd.clear();
    lcd.setCursor(0,0); // İlk satırın başlangıç noktası
    lcd.print(SELAMLA);
    delay(5000);
   
  } else {
    // SAAT YAZDIR
    lcd.clear();
    lcd.setCursor(0,0); // İlk satırın başlangıç noktası
    lcd.print("11:40");
    delay(3000);
  }
  if(dokunma_check == 1) {
    touch_count += 1;
    touch_count = touch_count % 4;
    Serial.print("ICERI : ");
    Serial.println(touch_count);
   
  }
 
  if(touch_count == 1) {
    lcd.clear();
    lcd.setCursor(0,0); // İlk satırın başlangıç noktası
    lcd.print("SU SEVIYESI: ");
    lcd.setCursor(0,1);
    lcd.print(su_seviyesi_deger);
    delay(3000);
    if (su_seviyesi_deger < 165 && MZ80_STATUS == 1){
        aciktim();
     }
  } else if (touch_count == 2) {
    // Sensörden okunan değerleri serial ekranda yazdırıyoruz.
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Nem Orani: ");
      lcd.print((float)dht_sensor.humidity, 2);
      lcd.setCursor(0,1);
      lcd.print("Sicaklik: ");
      lcd.print((float)dht_sensor.temperature, 2);
      delay(3000);
      if (su_seviyesi_deger < 165 && MZ80_STATUS == 1){
        aciktim();
     }
  } else if (touch_count == 3) {
    // GUN/AY/YIL YAZDIR
    lcd.clear();
    lcd.setCursor(0,0); // İlk satırın başlangıç noktası
    lcd.print("07.06.2022");
    delay(3000);
    if (su_seviyesi_deger < 165 && MZ80_STATUS == 1){
        aciktim();
     }
  }
  if (su_seviyesi_deger < 165 && MZ80_STATUS == 1){
        aciktim();
   }
 // Serial.print(toprak_deger);
  if(toprak_deger< 1500){
      ServoNesnesi.write(90);  /* Motorun mili 100. dereceye donuyor */
        delay(4000);
      ServoNesnesi.write(270);   /* Motor mili 20. dereceye donuyor */
        delay(1000);
  }
  Serial.print(isik_degeri);
  if(isik_degeri<200){
    renkAyarla(80, 0, 80); //mor
  } else {
   analogWrite(RGB_R, LOW);
   analogWrite(RGB_G, LOW);
   analogWrite(RGB_B, LOW);
  }
 
   
 }

 
 
 
 



void renkAyarla(int kirmizi, int yesil, int mavi){
  kirmizi = 255-kirmizi;
  yesil = 255-yesil;
  mavi = 255-mavi;
  analogWrite(RGB_R, kirmizi);
  analogWrite(RGB_G, yesil);
  analogWrite(RGB_B, mavi);
  }

void aciktim(){
//tone(BUZZER, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACIKTIM !!");
    //noTone(BUZZER);     // Stop sound...
    delay(3000);
}
