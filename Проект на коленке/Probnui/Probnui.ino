#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 1); // RX,TX
byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
unsigned char response[9];

int ppm = 0;

unsigned long CO2Millis = 600000;
unsigned long MillisLast = 0;

#define ledPin 12

void setup() {
  pinMode(ledPin, OUTPUT);
  //Serial.begin(115200);  
  //mySerial.begin(9600);  
}

void getCO2Data() {
  mySerial.write(cmd, 9);
  memset(response, 0, 9);
  mySerial.readBytes(response, 9);


// CRC check
  int i;
  byte crc = 0;
  for (i = 1; i < 8; i++) crc += response[i];
  crc = 255 - crc;
  crc++;

  if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
    
    char raw[32];
    sprintf(raw, "RAW: %02X %02X %02X %02X %02X %02X %02X %02X %02X", response[0], response[1], response[2], response[3], response[4], response[5], response[6], response[7], response[8]);    
    
    MillisLast = CO2Millis-10000;
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    ppm = (256 * responseHigh) + responseLow;
    int temp = response[4] - 20;
    char raw[32];
    sprintf(raw, "RAW: %02X %02X %02X %02X %02X %02X %02X %02X %02X", response[0], response[1], response[2], response[3], response[4], response[5], response[6], response[7], response[8]);    
    if (ppm <= 400 || ppm > 4900) {     
      MillisLast = CO2Millis;
    } else {      
      digitalWrite(ledPin, LOW);      
      } 
      digitalWrite(ledPin, HIGH);
    }
  }    


void loop() {
  unsigned long CurrentMillis = millis();
//  if (CurrentMillis - MillisLast > CO2Millis || MillisLast == 0) {
//    MillisLast = CurrentMillis;
//   // getCO2Data();
//  }

  mySerial.begin(9600);
  getCO2Data();
  mySerial.end();

  Serial.begin(115200);
  Serial.print("ppm :");
  Serial.println(ppm);
  Serial.end();
  //Serial.println(ppm);
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
  delay(5000);
}
