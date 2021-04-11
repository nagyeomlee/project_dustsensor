#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial BTSerial(3,4);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define BLUE 5
#define GREEN 6
#define RED 7

int Vo = A0;
int V_LED = 2;

float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

int cnt;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

char temp = 0;
char data = 'a';

void setup()
{
  Serial.begin(115200);
  BTSerial.begin(115200);
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
  if (BTSerial.available())
    temp = BTSerial.read();

  digitalWrite(V_LED, LOW);
  delayMicroseconds(280);
  Vo_value = analogRead(Vo);
  delayMicroseconds(40);
  digitalWrite(V_LED, HIGH);
  delayMicroseconds(9680);
  
  Voltage = Vo_value * 5.0 / 1024.0;
  dustDensity = (Voltage - 0.6)/0.005;
  String dustString = String (dustDensity);

  if ( dustDensity < 0 )
  {
    delay (1000);
    return;
  }

  Serial.print(" Voltage : ");
  Serial.println(Voltage);
  Serial.print(" Dust Density : ");
  Serial.println(dustDensity);

  BTSerial.println(dustString);

  lcd.setCursor(0, 0);
  lcd.print("MISEMISE Project");
  lcd.setCursor(0, 1);
  lcd.print("dust : ");
  lcd.setCursor(7, 1);
  lcd.print(dustDensity);
  lcd.setCursor(11, 1);
  lcd.print("ug/m3");

  Mode (temp);
  temp = '\0';
  
  delay(1000);
}

void Mode (char temp)
{
    if (temp == 'm')
    {
      data = temp;
    }
    else if (temp == 'a')
    {
      data = temp;
    }

    if (data == 'm')
    {
      switch(temp)
      {
        case '1' : // 문 열기
        break;
        case '2' : // 문 닫기
        break;
        default : break;
      }
    }

    else if (data == 'a')
    {
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
          cnt++;
         }
         else if (dustDensity >= 80 && dustDensity < 150)
         {
          redValue = 250;
          greenValue = 60;
          blueValue = 0;
          analogWrite(RED, redValue);
          analogWrite(GREEN, greenValue);
          analogWrite(BLUE, blueValue);
          cnt++;
         }
          else if (dustDensity > 150)
          {
          redValue = 220;
          greenValue = 0;
          blueValue = 0;
          analogWrite(RED, redValue);
          analogWrite(GREEN, greenValue);
          analogWrite(BLUE, blueValue);
          cnt++;
          }
    }
}
