#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define BLUE 9
#define GREEN 10
#define RED 11

int Vo = A0;
int V_LED = 2;

float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(V_LED, OUTPUT);
  pinMode(Vo, INPUT);
  lcd.begin();

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);

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

  if ( dustDensity < 0 )
  {
    delay (1000);
    return;
  }

  if (dustDensity < 30)
  {
    greenValue = 20;
    blueValue = 255;
    redValue = 0;
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    analogWrite(RED, redValue);
  }
  else if (dustDensity >= 30 && dustDensity < 80)
  {
    greenValue = 255;
    blueValue = 20;
    redValue = 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
  else if (dustDensity >= 80 && dustDensity < 150)
  {
    redValue = 250;
    greenValue = 60;
    blueValue = 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
  else if (dustDensity > 150)
  {
    redValue = 220;
    greenValue = 0;
    blueValue = 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }


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
  lcd.setCursor(11, 1);
  lcd.print("ug/m3");
  
  delay(1000);

  
}
