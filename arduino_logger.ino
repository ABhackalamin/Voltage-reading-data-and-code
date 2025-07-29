#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"

EnergyMonitor emon1;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD এর I2C ঠিকানা (0x27 বা 0x3F হতে পারে)

// সেটআপ ফাংশন
void setup() {
  Serial.begin(115200);  
  lcd.init();  // LCD ইনিশিয়ালাইজ করা
  lcd.backlight();  // LCD ব্যাকলাইট অন করা
  
  // ভোল্টেজ সেন্সর সেটআপ (A0 পিন, ক্যালিব্রেশন 169, ফেজ শিফট 1.7)
  emon1.voltage(A0, 169, 1.7);  

  // কারেন্ট সেন্সর সেটআপ (A1 পিন, ক্যালিব্রেশন 111.1) **ক্যালিব্রেশন মান ঠিক করে নিতে হবে**
  emon1.current(A1, 111.1);  

  // LCD তে প্রাথমিক মেসেজ দেখানো
  lcd.setCursor(0, 0);
  lcd.print("Voltage: 000V");
  lcd.setCursor(0, 1);
  lcd.print("Current: 0.00A");
  delay(2000);
  lcd.clear();
}

// লুপ ফাংশন
void loop() {
  emon1.calcVI(20, 2000);  // ভোল্টেজ ও কারেন্ট রিডিং নেওয়া
  float Voltage = emon1.Vrms;  // Vrms এক্সট্রাক্ট করা
  float Current = emon1.Irms;  // Irms এক্সট্রাক্ট করা

  // **সিরিয়াল মনিটরে প্রিন্ট করা**
  Serial.print("Voltage: ");
  Serial.print(Voltage);
  Serial.print(" V, ");

  Serial.print("Current: ");
  Serial.print(Current, 2);
  Serial.println(" A");

  // **I2C LCD-তে প্রিন্ট করা**
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.print(Voltage, 1);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("Current: ");
  lcd.print(Current, 2);
  lcd.print("A");

  delay(300);  // প্রতি 300ms পর আপডেট করা
}
