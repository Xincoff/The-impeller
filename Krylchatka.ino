#include <GyverOLED.h>
#include <charMap.h>
#include <icons_7x7.h>
#include <icons_8x8.h>
#include <TimerOne.h>
#define button 4
#define but_tmt 50 // для вычисления частоты
#define slp_time 100 // таймер для проверки реле
static uint32_t tim;

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // объявление OLED

const int inputPin = 5;  // T1 (D5) - вход счётчика
volatile unsigned long overflowCount = 0;
int frequency; // Частота 1
int frequency2; // частота 2
const int Relay = 13; // Реле на пине

const int Voltage = A1;
const int out = 10; // Шим пин
const int but = 9;
const int but2 = 8;
const int but3 = 7; // 3 кнопки

boolean flag = LOW;
boolean flag3 = LOW; // для кнопок

int a = 0;
int b;
int d;
boolean trigger = LOW; // для кнопок

boolean butState = HIGH;
boolean butState2 = HIGH; // Для кнопок


void setup() 
{
  Serial.begin(115200);
  pinMode(but, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  pinMode(Relay, OUTPUT);
  pinMode(out, OUTPUT);
   
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(1);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
  oled.print("Частота:");
  oled.setCursorXY(95, 0);
  oled.print("|");

  oled.setCursorXY(0, 35);
  oled.print("Усткавка:");

  oled.setCursorXY(0, 15);
  oled.print("Турбина:");
  
  // Настройка Timer1 в режиме счётчика
  TCCR1A = 0;  // Обычный режим
  TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10);  // Внешний источник (T1, D5), фронт
  TIMSK1 = (1 << TOIE1);  // Разрешить прерывание по переполнению
  TCNT1 = 0;  // Сброс счётчика
  
  attachInterrupt(0, countOverflow, RISING);  // Прерывание по переполнению
}

void countOverflow() 
{
  overflowCount++;  // +65536 импульсов
}

void loop() 
{
  b = constrain(b, 0, 10000);

  BUT3();
  BUT();
  BUT2(); // функции кнопок
  




  if (val == buftrigg)
  {
    buf[bufindex] = val;
    bufindex = (bufindex + 1);
  }
  else if (val == 0)
  {
    for (int i = 0; i < bufsize; i++) 
    {
      buf[i] = 0; // Заполняем буфер нулями
    }
  bufindex = 0;
  }

  if (bufindex >= bufsize && val == 0) 
  {
    for (int i = 0; i < bufsize; i++) 
    {
      buf[i] = 0; // Заполняем буфер нулями

    }
  bufindex = 0;

  }
  else if (bufindex < bufsize || val == 0)
  {
    // ?????????????
  }
  else if (bufindex = bufsize)
  {
    Serial.print("Готово");
  }
  Serial.print("Индекс: ");
  Serial.println(bufindex);
  printBuffer();







  if (frequency >> b) // Реле аварийной остановки
  {
    digitalWrite(Relay, HIGH);
    Serial.println("LIGHT");
  }
  else
  {
    digitalWrite(Relay, LOW);
    Serial.println("LOW");
  }
  
  oled.setCursorXY(55, 35);
  oled.print(a);
}


void Scale()
{
  if (trigger == HIGH) // выбор множителя частоты
  {
    Serial.println("Режим 2x");
    oled.setCursorXY(100, 0);
    oled.print("2x");
    Freq2();
  }
  else if (trigger == LOW)
  {
    Serial.println("Режим 1x");
    oled.setCursorXY(100, 0);
    oled.print("1x");
    Freq();
  }
}

void Freq() // функция вычисления частоты
{
  delay(1000);  // Ждём 1 секунду

  static uint32_t timer;

  if (millis() - timer > but_tmt)
  {
    timer = millis();
    static bool pState = false;
    bool state = !digitalRead(button);
    oled.clear(50, 0, 80, 0);
  }

  noInterrupts();
  frequency = (overflowCount << 16) + TCNT1;  // Общее количество импульсов
  overflowCount = 0;
  TCNT1 = 0;
  interrupts();
  
  Serial.print("Частота: ");
  Serial.print(frequency);
  Serial.println(" Гц");
  oled.setCursor(50, 0);
  oled.print(frequency);
  oled.setCursorXY(50, 15);
  oled.print(frequency * 33.3); // Думаю между *33 и *33.3

  d = map(frequency, 0, b, 205, 1023);
  d = constrain(d, 205, 1023);  // Ограничиваем 4–20 м
  analogWrite(out, d);
  Serial.print("Voltage: ");
  Serial.println(d);
}

void Freq2() // функция вычисления частоты с множителем 2
{
  delay(1000);  // Ждём 1 секунду

  static uint32_t timer;

  if (millis() - timer > but_tmt)
  {
    timer = millis();
    static bool pState = false;
    bool state = !digitalRead(button);
    oled.clear(50, 0, 80, 0);
  }

  noInterrupts();
  frequency = (overflowCount << 16) + TCNT1;  // Общее количество импульсов
  overflowCount = 0;
  TCNT1 = 0;
  interrupts();
  frequency2 = frequency * 2;
  Serial.print("Частота: ");
  Serial.print(frequency2);
  Serial.println(" Гц");
  oled.setCursor(50, 0);
  oled.print(frequency2);
  oled.setCursorXY(50, 15);
  oled.print(frequency2 * 33.3); // Думаю между *33 и *33.3

  d = map(frequency2, 0, b*2, 205, 1023);
  d = constrain(d, 205, 1023);  // Ограничиваем 4–20 м
  analogWrite(out, d);
}

void BUT() // кнопка с фиксацией и редакицей уставки
{
  int val = digitalRead(but);
  if (val != butState)
  {
    butState = val;
    if (val == HIGH)
    {
      a++;
      flag = !flag;
      b = a * 1000;
      Serial.println(b);
      oled.setCursorXY(55, 35);
      oled.print(b);
    }
  }
}

void BUT2() // кнопка с фиксацией и редакцией уставки 
{
  int val2 = digitalRead(but2);
  if (val2 != butState)
  {
    butState = val2;
    if (val2 == HIGH)
    {
      a--;
      flag = !flag;
      b = a * 1000;
      Serial.println(b);
      oled.setCursorXY(55, 35);
      oled.print(b);
    }
  }
  //butState = flag;
}



void BUT3() // кнопка с фиксацией и редакцией множителя
{
  int val3 = digitalRead(but3);
  if (val3 != butState2)
  {
    butState2 = val3;
    if (val3 == HIGH)
    {
      flag3 = !flag3;
    }
  }
  trigger = flag3;
}
