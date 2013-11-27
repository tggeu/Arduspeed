#include <Messenger.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#define SPEED_SERVO_PIN 6
#define SPEED_SERVO_0_US 2800
#define SPEED_SERVO_200_US 870

Servo hand;
Messenger message;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void sendVersion()
{
  Serial.println(F("AD 02.12 ArduSpeed"));
}

void sendOK()
{
  Serial.println(F("OK"));
}

void sendERROR()
{
  Serial.println(F("ERROR"));
}

void lcd_rjust_number(int num, byte length)
{
  if (num < 10)
  {
    lcd.print(F(" "));
  }

  if (num < 100)
  {
    lcd.print(F(" "));
  }

  if (num < 1000 && length == 4)
  {
    lcd.print(F(" "));
  }

  lcd.print(num);
}

void setSpeedLCD(int speed)
{
  lcd.setCursor(0, 0);
  lcd_rjust_number(speed, 3);
  lcd.print(F(" km/h"));
}

void setSpeedServo(int speed)
{
  int microseconds = map(speed, 0, 200, SPEED_SERVO_0_US, SPEED_SERVO_200_US);
  hand.writeMicroseconds(microseconds);
}

void setSpeed()
{
  int speed = message.readInt();
  if (speed < 0 || speed > 230)
  {
    sendERROR();
    return;
  }

  setSpeedServo(speed);
  setSpeedLCD(speed);
  sendOK();
}

void setFuelLCD(float fuel)
{
  lcd.setCursor(11, 0);
  if (fuel >= 100.0)
  {
    lcd.print(F(" "));
    lcd.print(fuel, 0);
  }

  else if (fuel >= 10.0)
  {
    lcd.print(fuel, 1);
  }

  else
  {
    lcd.print(fuel, 2);
  }

  lcd.print(F("%"));
}

void setFuel()
{
  int fuel_int = message.readInt();
  if (fuel_int > 10000 || fuel_int < 0)
  {
    sendERROR();
    return;
  }

  float fuel = fuel_int / 100.0;

  setFuelLCD(fuel);
  sendOK();
}

void setGearLCD(int gear)
{
  lcd.setCursor(9, 0);
  if (gear == 0)
  {
    lcd.print(F("R"));
  }
  
  else if (gear == 1)
  {
    lcd.print(F("N"));
  }
  
  else
  {
    lcd.print(gear - 1);
  }
}

void setGear()
{
  int gear = message.readInt();
  if (gear > 7 || gear < 0)
  {
    sendERROR();
    return;
  }

  setGearLCD(gear);
  sendOK();
}

void setRPMLCD(int rpm)
{
  lcd.setCursor(0, 1);
  byte progress_width = map(rpm, 0, 10000, 0, 10);
  for (byte x=0; x<10; x++)
  {
    if (x < progress_width)
    {
      lcd.write((byte) 1);
    }

    else
    {
      lcd.write((byte) 0);
    }
  }

  lcd.print(" ");
  lcd_rjust_number(rpm, 4);
}

void setRPM()
{
  int rpm = message.readInt();
  if (rpm > 10000 || rpm < 0)
  {
    sendERROR();
    return;
  }

  setRPMLCD(rpm);
  sendOK();
}

void onMessage()
{
  switch (message.readChar())
  {
    case 'V':
      sendVersion();
      break;

    case 'S':
      setSpeed();
      break;

    case 'F':
      setFuel();
      break;

    case 'G':
      setGear();
      break;

    case 'R':
      setRPM();
      break;

    default:
      sendERROR();
      break;
  }
}

void createChars()
{
  byte circle_open[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B01110,
    B00000
  };

  byte circle_close[8] = {
    B01110,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B01110,
    B00000
  };

  lcd.createChar(0, circle_open);
  lcd.createChar(1, circle_close);
}

void serialEvent()
{
  message.process(Serial.read());
}

void setup()
{
  hand.attach(6);
  Serial.begin(19200);
  message.attach(onMessage);
lcd.begin(16,2);
  createChars();

  setSpeedServo(0);
}

void loop()
{
}
