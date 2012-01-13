#include <Messenger.h>
#include <Servo.h>

#define SPEED_SERVO_PIN 2
#define SPEED_SERVO_0_US 2800
#define SPEED_SERVO_200_US 870

Servo hand;
Messenger message;

void sendVersion()
{
  Serial.println(F("AD 01.12 ArduDash"));
}

void sendOK()
{
  Serial.println(F("OK"));
}

void sendERROR()
{
  Serial.println(F("ERROR"));
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
    default:
      sendERROR();
      break;
  }
}

void serialEvent()
{
  message.process(Serial.read());
}

void setup()
{
  hand.attach(2);
  Serial.begin(19200);
  message.attach(onMessage);
  
  setSpeedServo(0);
}

void loop()
{
}
