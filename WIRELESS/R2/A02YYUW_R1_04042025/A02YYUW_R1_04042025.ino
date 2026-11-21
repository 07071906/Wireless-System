#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX 
// connect yellow wire to D10 and White wire to D11

unsigned char data[4] = {};
float distance;

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop()
{
  do {
    for (int i = 0; i < 4; i++)
    {
      data[i] = mySerial.read();
    }
  } while (mySerial.read() == 0xff);

  mySerial.flush();

  if (data[0] == 0xff)
  {
    int sum;
    sum = (data[0] + data[1] + data[2]) & 0x00FF;

    if (sum == data[3])
    {
      distance = (data[1] << 8) + data[2];
      distance = distance / 10;

      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
  }

  delay(100);
}






























/*#include <SoftwareSerial.h>

SoftwareSerial mySerial(D7, D6); // RX, TX (we only use RX)

uint8_t data[4];

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available() >= 4) {
    if (mySerial.read() == 0xFF) {
      data[0] = 0xFF;
      for (int i = 1; i < 4; i++) {
        data[i] = mySerial.read();
      }

      uint8_t checksum = (data[0] + data[1] + data[2]) & 0x00FF;

      if (checksum == data[3]) {
        int distance = (data[1] << 8) + data[2];
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
      } else {
        Serial.println("Checksum error");
      }
    }
  }
}*/