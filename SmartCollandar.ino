#include <softspi.h>
#include <rfid1.h>
#include <Servo.h>
#include <softspi.h>
#include "rfid1.h"
#include <Wire.h>

#define relayPin 8

Servo myservo;
RFID1 rfid;
int ledPin = 10;

// RFID timing variables
int _secondsWithTag = 0;
int _secondsWithoutTag = 0;

uchar status;
uchar str[MAX_LEN];

uchar serNum[5];

void setup()
{
  Serial.begin(9600);

  rfid.begin(7, 5, 4, 3, 6, 2);

  delay(100);

  rfid.init();

  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, HIGH);

  delay(2000);

  myservo.attach(9);
  ServoAnimation(false);
  digitalWrite(ledPin, LOW);
}

void loop()
{
  status = rfid.request(PICC_REQIDL, str);

  if (status == MI_OK)
    status = rfid.anticoll(str);

  if (status == MI_OK)
    CompareTag();

  //delay(1000);

  _secondsWithoutTag++;

  if (_secondsWithoutTag == 3)
    _secondsWithTag = 0;
}

void CompareTag()
{
  memcpy(serNum, str, 5);

  uchar *id = serNum;

  if (id[0] == 0x33 && id[1] == 0x2A && id[2] == 0xA5 && id[3] == 0x75)
  { 
    delay(500);
    _secondsWithTag++;

    // Blink LED red
    digitalWrite(ledPin, HIGH);

    delay(500);

     digitalWrite(ledPin, LOW);

    _secondsWithoutTag = 0;

    Serial.println(_secondsWithTag);

    digitalWrite(relayPin, LOW);

    if (_secondsWithTag == 5)
    {
      _secondsWithTag = 0;
      OpenCollandar();
    }

    digitalWrite(relayPin, HIGH);
  }
}

void OpenCollandar()
{
  Serial.println("Opening");
  ServoAnimation(true);

  delay(6000);

  Serial.println("Closing");
  ServoAnimation(false);

}


void ServoAnimation(bool _open) {
  int pos = 0;

  if (_open == true) {
    for (pos = 0; pos <= 180; pos += 2) {
      // in steps of 1 degree
      myservo.write(pos);

      delay(15);
    }
  }
  else {
    for (pos = 180; pos >= 0; pos -= 2) {
      myservo.write(pos);
      delay(15);
    }
  }

}
