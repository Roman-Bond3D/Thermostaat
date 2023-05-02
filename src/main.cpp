#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD configuration
const int
    LCD_PIN_RS = 12,
    LCD_PIN_EN = 13,
    LCD_PIN_D4 = 8,
    LCD_PIN_D5 = 9,
    LCD_PIN_D6 = 10,
    LCD_PIN_D7 = 11;

LiquidCrystal lcd(
    LCD_PIN_RS,
    LCD_PIN_EN,
    LCD_PIN_D4,
    LCD_PIN_D5,
    LCD_PIN_D6,
    LCD_PIN_D7);

// NTC configuration
const int
    NTC_PIN = 1;

// Relais configuration
const int
    HEATER_PIN = 7;

// KeyPad buttons
int
    lcd_key = 0,
    adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

// Temperature
float temperature = 10.00;

// Setpoint
float setpoint = 10.00;

// Deviation
const float deviation = 1.0;

int readKeypad()
{
  adc_key_in = analogRead(0); // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1500)
    return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 195)
    return btnUP;
  if (adc_key_in < 380)
    return btnDOWN;
  if (adc_key_in < 500)
    return btnLEFT;
  if (adc_key_in < 700)
    return btnSELECT;
  return btnNONE; // when all others fail, return this...
}

float readTemperature()
{
  int Vo;
  float R1 = 10000;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  Vo = analogRead(NTC_PIN);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;

  return T;
}

void heaterOn(const bool request)
{
  if (request)
  {
    digitalWrite(HEATER_PIN, HIGH);
  }
  else
  {
    digitalWrite(HEATER_PIN, LOW);
  }
}

void temperatureControl(const float temperature, const float setpoint)
{
  if (temperature < setpoint - deviation)
  {
    heaterOn(true);
  }
  else if (temperature > setpoint)
  {
    heaterOn(false);
  }
}

void displayLCD(const int line, const String &text)
{
  lcd.setCursor(0, line);
  lcd.print(text);
}

void keypad()
{
  lcd_key = readKeypad(); // read the buttons

  switch (lcd_key) // depending on which button was pushed, we perform an action
  {
  case btnRIGHT:
  {

    break;
  }
  case btnLEFT:
  {

    break;
  }
  case btnUP:
  {
    setpoint = setpoint + 0.50;
    break;
  }
  case btnDOWN:
  {
    setpoint = setpoint - 0.50;
    break;
  }
  case btnSELECT:
  {

    break;
  }
  case btnNONE:
  {

    break;
  }
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(NTC_PIN, INPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
}

void loop()
{
  // put your main code here, to run repeatedly:
  temperature = readTemperature();
  displayLCD(0, "Temperatuur: " + String(temperature));
  displayLCD(1, "Instelling: " + String(setpoint));
  temperatureControl(temperature, setpoint);
  keypad();
  Serial.print("T" + String(temperature, 2));
  Serial.print("S" + String(setpoint, 2));
  delay(250);
}
