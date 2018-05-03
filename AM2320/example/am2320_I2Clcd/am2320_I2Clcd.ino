// экран
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(63,16,2);  // Устанавливаем дисплей (Adr,Cols,Rows)

// датчик влажности и температуры AM2320
#include <Wire.h>
#include <AM2320.h>
AM2320 th;

// копка
#define BUTTON_PIN 6

int tab = 0;// номер текущего экрана
int bDown = 0;// как долго кнопка нажата

void setup() {
  Serial.begin(9600);
  // экран
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.clear();
  // кнопка
  pinMode(BUTTON_PIN, INPUT_PULLUP);//включаем внутренний pull-up резистор
}

void loop() {
  int ths = th.Read();
  switch(ths) {
    case 2:
      Serial.println("CRC failed");
      break;
    case 1:
      Serial.println("Sensor offline");
      break;
    case 0:
      Serial.print("humidity: ");
      Serial.print(th.h);
      Serial.print("%, temperature: ");
      Serial.print(th.t);
      Serial.println("*C");
      break;
  }

  // считываем состояние кнопки избегая дребезга
  if (digitalRead(BUTTON_PIN) == LOW) bDown++;
  if ((digitalRead(BUTTON_PIN) == HIGH) & (bDown > 0)) {
    tab++;
    if (tab >= 2) tab = 0;
    lcd.clear();
    bDown = 0;
  }
  // вывод на экран
  char line0[32];
  char line1[32];
  switch (tab){
    case 0:
      if (ths == 0) {
        sprintf(line0, "t=%2d", (int) th.t);
        sprintf(line1, "h=%2d", (int) th.h);
      } else {
        sprintf(line0, "t=!!'C");
        sprintf(line1, "h=!!%");
      }
      break;
    case 1:
      sprintf(line0, "                ");
      sprintf(line1, "                ");
      break;
  }
  lcd.setCursor(0, 0);
  lcd.print(line0);
  lcd.setCursor(0, 1);
  lcd.print(line1);

  delay(200);
}
