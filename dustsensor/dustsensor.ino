#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int Vo = A0;
int V_LED = 2;

float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(V_LED, OUTPUT);
  pinMode(Vo, INPUT);
  lcd.begin();
}

void loop()
{
  digitalWrite(V_LED, LOW);
  delayMicroseconds(280);
  Vo_value = analogRead(Vo);
  delayMicroseconds(40);
  digitalWrite(V_LED, HIGH);
  delayMicroseconds(9680);
  
  Voltage = Vo_value * 5.0 / 1024.0;
 dustDensity = (Voltage - 0.6)/0.005;

 //Voltage = Vo_value * (5.0/1024.0);
 //dustDensity = (0.17 *  Voltage - 0.1 ) * 1000;
  
  Serial.print(" Voltage : ");
  Serial.println(Voltage);
  Serial.print(" Dust Density : ");
  Serial.println(dustDensity);

  lcd.setCursor(0, 0);
  lcd.print("MISEMISE Project");
  lcd.setCursor(0, 1);
  lcd.print("dust : ");
  lcd.setCursor(7, 1);
  lcd.print(dustDensity);
  
  delay(1000);
}
