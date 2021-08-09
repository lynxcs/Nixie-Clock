#define QA1 2
#define QB1 3
#define QC1 4
#define QD1 5
#define QA2 6
#define QB2 7
#define QC2 8
#define QD2 9
#define QA3 10
#define QB3 11
#define QC3 12
#define QD3 13
#define QA4 A0
#define QB4 A1
#define QC4 A2
#define QD4 A3

#include <Wire.h>
#include "RTClib.h"
#include <String.h>
RTC_DS1307 rtc;

unsigned int digits[4][4] = {{QA1,QB1,QC1,QD1}, {QA2,QB2,QC2,QD2}, {QA3,QB3,QC3,QD3}, {QA4,QB4,QC4,QD4}};

int current_hour;
int current_minute;
int current_second;

void setup() {
  Serial.begin(9600);
  Serial.println(__TIME__);

  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }



  for(int i = 0; i < 4; i++)
  {
  pinMode(digits[i][0], OUTPUT);
  pinMode(digits[i][1], OUTPUT);
  pinMode(digits[i][2], OUTPUT);
  pinMode(digits[i][3], OUTPUT);
  }

  current_hour = 0;
  current_minute = 0;
}

void loop() {
  if (Serial.available() > 0) {
    String time = Serial.readString();
    Serial.println("Read string");
    rtc.adjust(DateTime(time.toInt()));
  }


  DateTime now = rtc.now();
  current_hour = now.hour();
  current_minute = now.minute();
  writenumber(current_hour/10, 0);
  writenumber(current_hour % 10, 1);
  writenumber(current_minute/10, 2);
  writenumber(current_minute%10, 3);
  Serial.print("Current time: ");
  Serial.print(current_hour);
  Serial.print(":");
  Serial.println(current_minute);
  delay(100);
}

void writenumber(unsigned int number, unsigned int digit)
{
  digitalWrite(digits[digit][0], number & (1 << 0));
  digitalWrite(digits[digit][1], number & (1 << 1));
  digitalWrite(digits[digit][2], number & (1 << 2));
  digitalWrite(digits[digit][3], number & (1 << 3));  
}

void scrolldown(unsigned int number, unsigned int digit)
{
  if (digit == 3)
  {
    Serial.println("Counting down last digit...");
    while(number != -1)
    {
      writenumber(number, digit);
      number--;
      delay(20);
    }
  }
  if (digit == 2)
  {
    Serial.println("Counting down third digit...");
    while(number != 0)
    {
      scrolldown(9, 3);
      number--;
      writenumber(number, digit);
      writenumber(9, 3);
      //delay(20);
    }
    scrolldown(9, 3);
  }
  if (digit == 1)
  {
    while(number != 0)
    {
      scrolldown(6, 2);
      number--;
      writenumber(number, digit);
      //delay(20);
    }
  }
  if (digit == 0)
  {
    while(number != 0)
    {
      scrolldown(4, 1);
      number--;
      writenumber(number, digit);
      //delay(20);
    }
  }
}

