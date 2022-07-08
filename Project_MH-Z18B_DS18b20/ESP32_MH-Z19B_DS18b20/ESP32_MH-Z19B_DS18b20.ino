//for MH-Z19B and ESP
#include <MHZ19.h>
#include <Arduino.h>
#include "MHZ19.h"                 // MH-Z19 by Junatan Dempsey https://github.com/WifWaf/MH-Z19
#include <SoftwareSerial.h>        // для ESP необходимо скачать и установить EspSoftwareSerial
// for DS18b20
#include <OneWire.h>
#include <DallasTemperature.h>

#define RX_PIN 3                   // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 1                   // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600              // Device to MH-Z19 Serial baudrate (should not be changed)
#define ledPin 12

MHZ19 myMHZ19;                               // Constructor for library
SoftwareSerial mySerial(RX_PIN, TX_PIN);     // create device to MH-Z19 serial

const int oneWireBus = 19;    // GPIO where the DS18B20 is connected to     
int CO2_ppm;                  // концентрация СО2 
float tempDS10b20;            // температура от DS10b20
int8_t tempMH_Z19B;           // температура от MH-Z19B

OneWire oneWire(oneWireBus);            // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);    // Pass our oneWire reference to Dallas Temperature sensor 

void setup() {
  pinMode(ledPin, OUTPUT);
  
  sensors.begin();

  mySerial.begin(BAUDRATE);     // (Uno example) device to MH-Z19 serial start   
  myMHZ19.begin(mySerial);      // *Serial(Stream) refence must be passed to library begin()
  myMHZ19.autoCalibration();    // Turn auto calibration ON (OFF autoCalibration(false))

  //mySerial.end();             //если в бесконечно цикле первым идёт обращение именно к mySerial, а не к другому порту, то можно его не останавливать   
 
}

void loop() {

//запрос температуры у DS18b20  
  sensors.requestTemperatures(); 
  float tempDS10b20 = sensors.getTempCByIndex(0);  
  
// запрос концентрации CO2 и температуры у MH-Z19B
  mySerial.begin(BAUDRATE);                               // запуск mySerial для обращения по UART   
  CO2_ppm = myMHZ19.getCO2();                             // запрос концентрации CO2
  tempMH_Z19B = myMHZ19.getTemperature();                 // запрос температуры в градусах Цельсия
  mySerial.end();                                         // остановка mySerial для возможности работы Serial

//передача всех параметров
  Serial.begin(115200);       // запуск Serial  
  Serial.print("CO2: ");                      
  Serial.print(CO2_ppm);
  Serial.print(" ppm  ");                      
   
  Serial.print("Temperature_MH-Z19B: ");                  
  Serial.print(tempMH_Z19B);  
  Serial.print("ºC  ");
  
  Serial.print("Temperature_DS18b20: ");                  
  Serial.print(tempDS10b20); 
  Serial.println("ºC");
                                     
  Serial.end();             // остановка Serial

  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(9500);
}
