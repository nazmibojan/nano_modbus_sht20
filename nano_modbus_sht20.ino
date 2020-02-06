#include <SoftwareSerial.h>

SoftwareSerial sht20(8, 9);

byte tempRequest[8] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0a};
int lastRequest = 0;
char receivedByte;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) {
    ;
  }

  sht20.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastRequest > 10000) {
    for (int i = 0; i < 8; i++) {
      sht20.write(tempRequest[i]);
    }
    lastRequest = millis();
  }

  while (sht20.available()) {
    receivedByte = sht20.read();
    Serial.print(receivedByte, HEX);
  }
  Serial.println();
}

// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(byte * buf, int len)
{
  uint16_t crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}
