#include <Stepper.h>          // 스테핑모터 사용을 위한 헤더파일
#include <SoftwareSerial.h>  // 블루투스 사용을 위한 헤더파일
#include <LiquidCrystal_I2C.h>      // I2C LCD 사용을 위한 헤더파일
SoftwareSerial BTSerial(3,4);       // 블루투스 핀으로 TX = 3, RX = 4 설정
LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C LCD 핀 설정, 주소, 16칸X2줄

// 스텝모터에서 스텝을 조절
// 100 = 180도, 200 = 360도, 300 = 540도, 400 = 720도
const int stepsPerRevolution = 100;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11); // 스텝(현재 100), 모터랑 연결된 핀 8, 9, 10, 11

#define BLUE 5  // 5번핀 => BLUE
#define GREEN 6 // 6번핀 => GREEN
#define RED 7   // 7번핀 => RED

int Vo = A0;   // 미세먼지 센서의 아날로그 값을 받는 Vo = 아두이노 UNO의 Analog Pin 0번
int V_LED = 2; // 미세먼지 센서에 IRED (적외선 LED) = Digital Pin 2번

// float 형식의 변수들을 0으로 값 초기화
float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

int cnt1, cnt2;
int window = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

char temp = 0;
char data = 'a';

void setup()
{
  myStepper.setSpeed(60);   // 스텝모터 스피드 셋팅 (60)
  Serial.begin(115200);     // 시리얼 모니터 셋팅 (속도 : 115200 bps)
  BTSerial.begin(115200);   // Bluetooth인 JMOD-BT-1의 속도가 115200bps라서 속도를 하나로 맞춰야 함
  pinMode(V_LED, OUTPUT);
  pinMode(Vo, INPUT);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("MISEMISE Project");

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void loop()
{
  if (BTSerial.available())
    temp = BTSerial.read();

  digitalWrite(V_LED, LOW); // PNP 트랜지스터 때문에 LOW가 LED 켜짐상태
  delayMicroseconds(280);
  Vo_value = analogRead(Vo);
  delayMicroseconds(40);
  digitalWrite(V_LED, HIGH);
  delayMicroseconds(9680);
  
  Voltage = Vo_value * 5.0 / 1024.0;
  dustDensity = (Voltage - 0.6)/0.005;
  dustDensity = roundf(dustDensity * 100) / 100;
  String dustString = String (dustDensity);

  if ( dustDensity < 0 )
  {
    delay (1000);
    return;
  }

  Serial.print("Voltage : ");
  Serial.println(Voltage);
  Serial.print("Dust Density : ");
  Serial.println(dustDensity);

  BTSerial.print(dustString);

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
      if (temp == '1' && window == 0) // 수동 창문 열기
      {
        Serial.println("manual window open");
        myStepper.step(stepsPerRevolution);
        window = 1;
        delay(1000);
      }
      if (temp == '2' && window == 1) // 수동 창문 닫음
      {
        Serial.println("manual window close");
        myStepper.step(-stepsPerRevolution);
        window = 0;
        delay(1000);
      }
    }

    else if (data == 'a')
    {
      if (dustDensity < 35) // 35미만, 좋음, RGB = 파랑
      {
        redValue = 255;
        greenValue = 255;
        blueValue = 0;
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
        analogWrite(RED, redValue);
        cnt2++;
        }
        else if (dustDensity >= 35 && dustDensity < 75) // 35이상 75미만, 조금 나쁨, RGB = 노랑
        {
          redValue = 80;
          greenValue = 80;
          blueValue = 255;
          analogWrite(RED, redValue);
          analogWrite(GREEN, greenValue);
          analogWrite(BLUE, blueValue);
          cnt1++;
         }
         else if (dustDensity >= 75) // 75 이상, 나쁨, RGB = 빨강
         {
          redValue = 0;
          greenValue = 255;
          blueValue = 255;
          analogWrite(RED, redValue);
          analogWrite(GREEN, greenValue);
          analogWrite(BLUE, blueValue);
          cnt1++;
         }

          Serial.print("BAD dustDensity count : ");
          Serial.println(cnt1);
          Serial.print("GOOD dustDensity count : ");
          Serial.println(cnt2);

          if (cnt2 == 10 && window == 0) // 0 = 닫힘, 1 = 열림
          {
            Serial.println("auto window.open");
            myStepper.step(stepsPerRevolution);
            window = 1;
            cnt1 = 0;
            cnt2 = 0;
            delay(5000);
          }

          if (cnt1 == 10 && window == 1)
          {
            Serial.println("auto window close");
            myStepper.step(-stepsPerRevolution);
            window = 0;
            cnt1 = 0;
            cnt2 = 0;
            delay(5000);
          }

          if (cnt1 == 10 || cnt2 == 10)
          {
            cnt1 = 0;
            cnt2 = 0;
          }
    }
}
